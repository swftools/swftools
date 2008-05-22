/* Libart_LGPL - library of basic graphic primitives
 * Copyright (C) 2001 Raph Levien
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* This file contains a testbed implementation of the new intersection
   code.
*/

#include <stdio.h>
#include <assert.h>
#include "config.h"
#include "art_svp_intersect.h"

#include <math.h> /* for sqrt */

/* Sanitychecking verifies the main invariant on every priority queue
   point. Do not use in production, as it slows things down way too
   much. */
#define noSANITYCHECK

/* This can be used in production, to prevent hangs. Eventually, it
   should not be necessary. */
#define CHEAP_SANITYCHECK

#define noVERBOSE

#define noABORT_ON_ERROR

#include "art_misc.h"

/* A priority queue - perhaps move to a separate file if it becomes
   needed somewhere else */

#define ART_PRIQ_USE_HEAP

typedef struct _ArtPriQ ArtPriQ;
typedef struct _ArtPriPoint ArtPriPoint;

struct _ArtPriQ {
  int n_items;
  int n_items_max;
  ArtPriPoint **items;
};

struct _ArtPriPoint {
  double x;
  double y;
  void *user_data;
};

static ArtPriQ *
art_pri_new (void)
{
  ArtPriQ *result = art_new (ArtPriQ, 1);

  result->n_items = 0;
  result->n_items_max = 16;
  result->items = art_new (ArtPriPoint *, result->n_items_max);
  return result;
}

static void
art_pri_free (ArtPriQ *pq)
{
  art_free (pq->items);
  art_free (pq);
}

static art_boolean
art_pri_empty (ArtPriQ *pq)
{
  return pq->n_items == 0;
}

/* This heap implementation is based on Vasek Chvatal's course notes:
   http://www.cs.rutgers.edu/~chvatal/notes/pq.html#heap */

static void
art_pri_bubble_up (ArtPriQ *pq, int vacant, ArtPriPoint *missing)
{
  ArtPriPoint **items = pq->items;
  int parent;

  parent = (vacant - 1) >> 1;
  while (vacant > 0 && (missing->y < items[parent]->y ||
			(missing->y == items[parent]->y &&
			 missing->x < items[parent]->x)))
    {
      items[vacant] = items[parent];
      vacant = parent;
      parent = (vacant - 1) >> 1;
    }

  items[vacant] = missing;
}

static void
art_pri_insert (ArtPriQ *pq, ArtPriPoint *point)
{
#ifdef VERBOSE
  art_dprint("insert into pq %08x: %.16f,%.16f %08x\n", pq, point->x, point->y, point->user_data);
#endif
  if (pq->n_items == pq->n_items_max)
    art_expand (pq->items, ArtPriPoint *, pq->n_items_max);

  art_pri_bubble_up (pq, pq->n_items++, point);
}

static void
art_pri_sift_down_from_root (ArtPriQ *pq, ArtPriPoint *missing)
{
  ArtPriPoint **items = pq->items;
  int vacant = 0, child = 2;
  int n = pq->n_items;

  while (child < n)
    {
      if (items[child - 1]->y < items[child]->y ||
	  (items[child - 1]->y == items[child]->y &&
	   items[child - 1]->x < items[child]->x))
	child--;
      items[vacant] = items[child];
      vacant = child;
      child = (vacant + 1) << 1;
    }
  if (child == n)
    {
      items[vacant] = items[n - 1];
      vacant = n - 1;
    }

  art_pri_bubble_up (pq, vacant, missing);
}

static ArtPriPoint *
art_pri_choose (ArtPriQ *pq)
{
  ArtPriPoint *result = pq->items[0];

  art_pri_sift_down_from_root (pq, pq->items[--pq->n_items]);
  return result;
}

/* TODO: this is *not* thread save */
const ArtSVP* current_svp = 0;
int art_error_in_intersector=0;

void art_report_error()
{
    art_error_in_intersector = 1;
#ifdef ABORT_ON_ERROR
    if(!current_svp) {
        fprintf(stderr, "internal error: no current polygon\n");
        exit(1);
    }
    const ArtSVP*svp = current_svp;
    FILE*fi = fopen("svp.ps", "wb");
    int i, j;
    fprintf(fi, "%% begin\n");
    for (i = 0; i < svp->n_segs; i++)
    {
        fprintf(fi, "%g setgray\n", svp->segs[i].dir ? 0.7 : 0);
        for (j = 0; j < svp->segs[i].n_points; j++)
          {
            fprintf(fi, "%.32f %.32f %s\n",
                    svp->segs[i].points[j].x,
                    svp->segs[i].points[j].y,
                    j ? "lineto" : "moveto");
          }
        fprintf(fi, "stroke\n");
    }
    fprintf(fi, "showpage\n");
    fclose(fi);

    fprintf(stderr, "There was an error during polygon processing.\n");
    fprintf(stderr, "I saved a debug file, called svp.ps, to the current directory.\n");
    fprintf(stderr, "Please help making this tool more stable by mailing\n");
    fprintf(stderr, "this file to debug@swftools.org. Thank you!\n");
    exit(1);
#endif
}

/* A virtual class for an "svp writer". A client of this object creates an
   SVP by repeatedly calling "add segment" and "add point" methods on it.
*/

typedef struct _ArtSvpWriterRewind ArtSvpWriterRewind;

/* An implementation of the svp writer virtual class that applies the
   winding rule. */

struct _ArtSvpWriterRewind {
  ArtSvpWriter super;
  ArtWindRule rule;
  ArtSVP *svp;
  int n_segs_max;
  int *n_points_max;
};

static int
art_svp_writer_rewind_add_segment (ArtSvpWriter *self, int wind_left,
				   int delta_wind, double x, double y)
{
  ArtSvpWriterRewind *swr = (ArtSvpWriterRewind *)self;
  ArtSVP *svp;
  ArtSVPSeg *seg;
  art_boolean left_filled, right_filled;
  int wind_right = wind_left + delta_wind;
  int seg_num;
  const int init_n_points_max = 4;

  switch (swr->rule)
    {
    case ART_WIND_RULE_NONZERO:
      left_filled = (wind_left != 0);
      right_filled = (wind_right != 0);
      break;
    case ART_WIND_RULE_INTERSECT:
      left_filled = (wind_left > 1);
      right_filled = (wind_right > 1);
      break;
    case ART_WIND_RULE_ODDEVEN:
      left_filled = (wind_left & 1);
      right_filled = (wind_right & 1);
      break;
    case ART_WIND_RULE_POSITIVE:
      left_filled = (wind_left > 0);
      right_filled = (wind_right > 0);
      break;
    default:
      art_die ("Unknown wind rule %d\n", swr->rule);
    }

#ifdef VERBOSE
  art_dprint("New svp segment %d: %.32f %.32f left_filled=%d, right_filled=%d\n", swr->svp->n_segs, x, y, left_filled, right_filled);
#endif

  if (left_filled == right_filled)
    {
      /* discard segment now */
#ifdef VERBOSE
      art_dprint ("  discarding segment: %d += %d (%.16f, %.16f)\n",
	      wind_left, delta_wind, x, y);
#endif
      return -1;
   }

  svp = swr->svp;
  seg_num = svp->n_segs++;
  if (swr->n_segs_max == seg_num)
    {
      swr->n_segs_max += 10;
      svp = (ArtSVP *)art_realloc (svp, sizeof(ArtSVP) +
				   (swr->n_segs_max - 1) *
				   sizeof(ArtSVPSeg));
      swr->svp = svp;
      swr->n_points_max = art_renew (swr->n_points_max, int,
				     swr->n_segs_max);
    }
  seg = &svp->segs[seg_num];
  seg->n_points = 1;
  seg->dir = right_filled;
  swr->n_points_max[seg_num] = init_n_points_max;
  seg->bbox.x0 = x;
  seg->bbox.y0 = y;
  seg->bbox.x1 = x;
  seg->bbox.y1 = y;
  seg->points = art_new (ArtPoint, init_n_points_max);
  seg->points[0].x = x;
  seg->points[0].y = y;
#ifdef VERBOSE
    art_dprint ("swr add_segment: %d += %d (%.16f, %.16f) --> %d (%s)\n",
	    wind_left, delta_wind, x, y, seg_num,
	    seg->dir ? "filled" : "non-filled");
#endif
  return seg_num;
}

static void
art_svp_writer_rewind_add_point (ArtSvpWriter *self, int seg_id,
				 double x, double y)
{

  ArtSvpWriterRewind *swr = (ArtSvpWriterRewind *)self;
  ArtSVPSeg *seg;
  int n_points;

#ifdef VERBOSE
  art_dprint ("swr add_point: %d (%.16f, %.16f)\n", seg_id, x, y);
#endif
  if (seg_id < 0)
    /* omitted segment */
    return;

  seg = &swr->svp->segs[seg_id];

  if(n_points &&
        seg->points[seg->n_points-1].x == x &&
        seg->points[seg->n_points-1].y == y) {
      //art_warn("duplicate point %.16f,%.16f in segment %08x\n", 
      //        x, y, seg_id);
      return;
  }

  n_points = seg->n_points++;
  if (swr->n_points_max[seg_id] == n_points)
    art_expand (seg->points, ArtPoint, swr->n_points_max[seg_id]);

  if(0 && n_points>=2) {
      double dx1 = seg->points[n_points-1].x - seg->points[n_points-2].x;
      double dy1 = seg->points[n_points-1].y - seg->points[n_points-2].y;
      double dx2 = x - seg->points[n_points-2].x;
      double dy2 = y - seg->points[n_points-2].y;
      if(fabs(dx1*dy2 - dx2*dy1) < 1e-10) {
          seg->n_points--;
          n_points--; // remove previous point pointing in the same direction

         //art_warn("redundant point %.16f,%.16f in segment %08x\n", 
         //        seg->points[n_points-1].x, seg->points[n_points-1].y,
         //        seg_id);
      }
  }
  
  if(n_points && seg->points[n_points-1].y > y) {
      art_warn("non-increasing segment (%.16f -> %.16f)\n", seg->points[n_points-1].y, y);
      art_report_error();
  }

  seg->points[n_points].x = x;
  seg->points[n_points].y = y;
  if (x < seg->bbox.x0)
    seg->bbox.x0 = x;
  if (x > seg->bbox.x1)
    seg->bbox.x1 = x;
  seg->bbox.y1 = y;
}

static void
art_svp_writer_rewind_close_segment (ArtSvpWriter *self, int seg_id)
{
  /* Not needed for this simple implementation. A potential future
     optimization is to merge segments that can be merged safely. */
#ifdef CHEAP_SANITYCHECK
  ArtSvpWriterRewind *swr = (ArtSvpWriterRewind *)self;
  ArtSVPSeg *seg;

  if (seg_id >= 0)
    {
      seg = &swr->svp->segs[seg_id];
      if (seg->n_points < 2)
	art_warn ("*** closing segment %d with only %d point%s\n",
		  seg_id, seg->n_points, seg->n_points == 1 ? "" : "s");
    }
#endif

#ifdef VERBOSE
  art_dprint ("swr close_segment: %d\n", seg_id);
#endif
}

ArtSVP *
art_svp_writer_rewind_reap (ArtSvpWriter *self)
{
  ArtSvpWriterRewind *swr = (ArtSvpWriterRewind *)self;
  ArtSVP *result = swr->svp;

  art_free (swr->n_points_max);
  art_free (swr);
  return result;
}

ArtSvpWriter *
art_svp_writer_rewind_new (ArtWindRule rule)
{
  ArtSvpWriterRewind *result = art_new (ArtSvpWriterRewind, 1);

  result->super.add_segment = art_svp_writer_rewind_add_segment;
  result->super.add_point = art_svp_writer_rewind_add_point;
  result->super.close_segment = art_svp_writer_rewind_close_segment;

  result->rule = rule;
  result->n_segs_max = 16;
  result->svp = (ArtSVP*)art_alloc (sizeof(ArtSVP) + 
			   (result->n_segs_max - 1) * sizeof(ArtSVPSeg));
  result->svp->n_segs = 0;
  result->n_points_max = (int*)art_new (int, result->n_segs_max);

  return &result->super;
}

/* Now, data structures for the active list.

   signs:
               /  |
        b<0   /   |   \ b>0, c<0
        c>0  /\   |   /\
            /  \  |  /  \
           /    \ | /     
                 \|/
     -------------+--------------------
                 /|\ 
             \  / | \  /
              \/  |  \/ b<0, c<0
        b>0    \  |  /
        c>0     \ | /
 */

typedef struct _ArtActiveSeg ArtActiveSeg;

/* Note: BNEG is 1 for \ lines, and 0 for /. Thus,
   x[(flags & BNEG) ^ 1] <= x[flags & BNEG] */
#define ART_ACTIVE_FLAGS_BNEG 1

/* This flag is set if the segment has been inserted into the active
   list. */
#define ART_ACTIVE_FLAGS_IN_ACTIVE 2

/* This flag is set when the segment is to be deleted in the
   horiz commit process. */
#define ART_ACTIVE_FLAGS_DEL 4

/* This flag is set if the seg_id is a valid output segment. */
#define ART_ACTIVE_FLAGS_OUT 8

/* This flag is set if the segment is in the horiz list. */
#define ART_ACTIVE_FLAGS_IN_HORIZ 16

struct _ArtActiveSeg {
  int flags;
  int wind_left, delta_wind;
  ArtActiveSeg *left, *right; /* doubly linked list structure */

  const ArtSVPSeg *in_seg;
  int in_curs;

  double x[2];
  double y0, y1;
  double a, b, c; /* line equation; ax+by+c = 0 for the line, a^2 + b^2 = 1,
		     and a>0 */

  /* bottom point and intersection point stack */
  int n_stack;
  int n_stack_max;
  ArtPoint *stack;

  /* horiz commit list */
  ArtActiveSeg *horiz_left, *horiz_right;
  double horiz_x;
  int horiz_delta_wind;
  int seg_id;
};

typedef struct _ArtIntersectCtx ArtIntersectCtx;

struct _ArtIntersectCtx {
  const ArtSVP *in;
  ArtSvpWriter *out;

  ArtPriQ *pq;

  ArtActiveSeg *active_head;

  double y;
  ArtActiveSeg *horiz_first;
  ArtActiveSeg *horiz_last;

  /* segment index of next input segment to be added to pri q */
  int in_curs;
};

#define EPSILON_A 1e-5 /* Threshold for breaking lines at point insertions */

/**
 * art_svp_intersect_setup_seg: Set up an active segment from input segment.
 * @seg: Active segment.
 * @pri_pt: Priority queue point to initialize.
 *
 * Sets the x[], a, b, c, flags, and stack fields according to the
 * line from the current cursor value. Sets the priority queue point
 * to the bottom point of this line. Also advances the input segment
 * cursor.
 **/
static void
art_svp_intersect_setup_seg (ArtActiveSeg *seg, ArtPriPoint *pri_pt)
{
  const ArtSVPSeg *in_seg = seg->in_seg;
  int in_curs = 0;
  double x0, y0, x1, y1;
  double dx, dy, s;
  double a, b, r2;

  //do {
    in_curs = seg->in_curs++;
  //} while(in_seg->points[in_curs].x == in_seg->points[in_curs + 1].x &&
  //        in_seg->points[in_curs].y == in_seg->points[in_curs + 1].y &&
  //        in_curs < in_seg->n_points-1
  //       );

  x0 = in_seg->points[in_curs].x;
  y0 = in_seg->points[in_curs].y;
  x1 = in_seg->points[in_curs + 1].x;
  y1 = in_seg->points[in_curs + 1].y;
  pri_pt->x = x1;
  pri_pt->y = y1;
  dx = x1 - x0;
  dy = y1 - y0;
  r2 = dx * dx + dy * dy;
  if(r2 == 0) {
      //art_warn("segment %08x has zero length\n", seg);
  }
  s = r2 == 0 ? 1 : 1 / sqrt (r2);
  seg->a = a = dy * s;
  seg->b = b = -dx * s;
  seg->c = -(a * x0 + b * y0);
  seg->flags = (seg->flags & ~ART_ACTIVE_FLAGS_BNEG) | (dx > 0);
  seg->x[0] = x0;
  seg->x[1] = x1;
  seg->y0 = y0;
  seg->y1 = y1;
  seg->n_stack = 1;
  seg->stack[0].x = x1;
  seg->stack[0].y = y1;

  if(y1 < y0) {
      art_warn("art_svp_intersect.c: bad input polygon %08x, contains decreasing y values\n", seg->in_seg);
      art_report_error();
  }
#ifdef VERBOSE
  art_dprint("segment %08x (top: %.16f,%.16f) starts with endpoint at %.16f,%.16f\n", seg, 
          x0, y0,
          x1, y1);
#endif
}

/**
 * art_svp_intersect_add_horiz: Add point to horizontal list.
 * @ctx: Intersector context.
 * @seg: Segment with point to insert into horizontal list.
 *
 * Inserts @seg into horizontal list, keeping it in ascending horiz_x
 * order.
 *
 * Note: the horiz_commit routine processes "clusters" of segs in the
 * horiz list, all sharing the same horiz_x value. The cluster is
 * processed in active list order, rather than horiz list order. Thus,
 * the order of segs in the horiz list sharing the same horiz_x
 * _should_ be irrelevant. Even so, we use b as a secondary sorting key,
 * as a "belt and suspenders" defensive coding tactic.
 **/
static void
art_svp_intersect_add_horiz (ArtIntersectCtx *ctx, ArtActiveSeg *seg)
{
  ArtActiveSeg **pp = &ctx->horiz_last;
  ArtActiveSeg *place;
  ArtActiveSeg *place_right = NULL;

#ifdef CHEAP_SANITYCHECK
  if (seg->flags & ART_ACTIVE_FLAGS_IN_HORIZ)
    {
      double dx = seg->x[1] - seg->x[0];
      double dy = seg->y1 - seg->y0;
      double len = sqrt(dx*dx+dy*dy);
      art_warn("attempt to put segment %08x %.16f,%.16f (len:%.16f) in horiz list twice\n", seg, seg->x[0], seg->y0, len);
      return;
    }
  seg->flags |= ART_ACTIVE_FLAGS_IN_HORIZ;
#endif

#ifdef VERBOSE
  art_dprint ("add_horiz %08x, x = %.16f\n", (unsigned long) seg, seg->horiz_x);
#endif
  for (place = *pp; place != NULL && (place->horiz_x > seg->horiz_x ||
				      (place->horiz_x == seg->horiz_x &&
				       place->b < seg->b));
       place = *pp)
    {
      place_right = place;
      pp = &place->horiz_left;
    }
  *pp = seg;
  seg->horiz_left = place;
  seg->horiz_right = place_right;

  if (place == NULL)
    ctx->horiz_first = seg;
  else
    place->horiz_right = seg;

#ifdef VERBOSE
  art_dprint("horiz_list:\n");
  ArtActiveSeg*s = ctx->horiz_first;
  while(s) {
      art_dprint("  %08x x=%.16f wind_left=%d delta_wind=%d horiz_delta_wind=%d\n", s, s->horiz_x, 
              s->wind_left, s->delta_wind, s->horiz_delta_wind);
      s = s->horiz_right;
  }
#endif
}

static void
art_svp_intersect_push_pt (ArtIntersectCtx *ctx, ArtActiveSeg *seg,
			   double x, double y)
{
  ArtPriPoint *pri_pt;
  int n_stack = seg->n_stack;

  if (n_stack == seg->n_stack_max)
    art_expand (seg->stack, ArtPoint, seg->n_stack_max);


  seg->stack[n_stack].x = x;
  seg->stack[n_stack].y = y;
  seg->n_stack++;

#ifdef VERBOSE
  int t;
  art_dprint("art_svp_intersect_push_pt %08x |top %.16f,%.16f\n", seg, seg->x[0], seg->y0);
  for(t=seg->n_stack-1;t>=0;t--) {
      if(t!=seg->n_stack-1)
          art_dprint("art_svp_intersect_push_pt %08x |pt  %.16f,%.16f\n", seg, seg->stack[t].x, seg->stack[t].y);
      else
          art_dprint("art_svp_intersect_push_pt %08x |new %.16f,%.16f\n", seg, seg->stack[t].x, seg->stack[t].y);
  }
#endif
#ifdef VERBOSE
  art_dprint("(shortening segment %08x to %.16f,%.16f)\n", seg, x, y);
#endif
  
  if(seg->stack[seg->n_stack-1].y == seg->y0) {
      art_warn("duplicate y coordinate (=y0) in point stack\n");
  }

  if(n_stack) {
      if(seg->stack[seg->n_stack-2].y < seg->stack[seg->n_stack-1].y) {
          art_warn("bad shortening- segment got *longer*\n");
          art_report_error();
      }
  }


  seg->x[1] = x;
  seg->y1 = y;

  pri_pt = art_new (ArtPriPoint, 1);
  pri_pt->x = x;
  pri_pt->y = y;
  pri_pt->user_data = seg;
  art_pri_insert (ctx->pq, pri_pt);
}

#define ART_BREAK_LEFT 1
#define ART_BREAK_RIGHT 2

/**
 * art_svp_intersect_break: Break an active segment.
 *
 * Note: y must be greater than the top point's y, and less than
 * the bottom's.
 *
 * Return value: x coordinate of break point.
 */
static double
art_svp_intersect_break (ArtIntersectCtx *ctx, ArtActiveSeg *seg,
			 double x_ref, double y, int break_flags)
{
  double x0, y0, x1, y1;
  const ArtSVPSeg *in_seg = seg->in_seg;
  int in_curs = seg->in_curs;
  double x;

  x0 = in_seg->points[in_curs - 1].x;
  y0 = in_seg->points[in_curs - 1].y;
  x1 = in_seg->points[in_curs].x;
  y1 = in_seg->points[in_curs].y;

  x = x0 + (x1 - x0) * ((y - y0) / (y1 - y0));

  //printf("%d %.16f %.16f %d\n", break_flags&ART_BREAK_LEFT, x, x_ref, x > x_ref);
  //printf("%d %.16f %.16f %d\n", break_flags&ART_BREAK_RIGHT, x, x_ref, x < x_ref);

  if ((break_flags == ART_BREAK_LEFT && x > x_ref) ||
      (break_flags == ART_BREAK_RIGHT && x < x_ref))
    {
#ifdef VERBOSE
      art_dprint ("art_svp_intersect_break %08x: limiting x to %.16f, was %.16f, %s\n", seg,
		  x_ref, x, break_flags == ART_BREAK_LEFT ? "left" : "right");
#endif
      //x = x_ref; //used to be *within* the VERBOSE comment
    }
  
  if(y < y0 || y > y1) {
      art_warn("!! bad break %.16f of %.16f-%.16f\n", y, y0, y1);
      art_report_error();
      return x;
  }

  /* I think we can count on min(x0, x1) <= x <= max(x0, x1) with sane
     arithmetic, but it might be worthwhile to check just in case. */

  /* TODO: should we check seg instead of in_seg ? */
  if(x0<x1) {
      if(x<x0 || x>x1) {
          art_warn("bad x value %.16f in intersect_break: not between %.16f and %.16f\n", x, x0, x1);
	  art_report_error();
      } 
  } else {
      if(x<x1 || x>x0) {
          art_warn("bad x value %.16f in intersect_break:  not between %.16f and %.16f\n", x, x1, x0);
	  art_report_error();
      } 
  }


  if (y > ctx->y)
    art_svp_intersect_push_pt (ctx, seg, x, y);
  else
    {
      if(y < ctx->y) {
          art_warn("intersect_break at a y above the current scanline (%.16f < %.16f)", y, ctx->y);
	  art_report_error();
      }
      seg->x[0] = x;
      seg->y0 = y;
      seg->horiz_x = x;
      art_svp_intersect_add_horiz (ctx, seg);
    }

  return x;
}

/**
 * art_svp_intersect_add_point: Add a point, breaking nearby neighbors.
 * @ctx: Intersector context.
 * @x: X coordinate of point to add.
 * @y: Y coordinate of point to add.
 * @seg: "nearby" segment, or NULL if leftmost.
 *
 * Return value: Segment immediately to the left of the new point, or
 * NULL if the new point is leftmost.
 **/
static ArtActiveSeg *
art_svp_intersect_add_point (ArtIntersectCtx *ctx, double x, double y,
			     ArtActiveSeg *seg, int break_flags)
{
  ArtActiveSeg *left, *right;
  double x_min = x, x_max = x;
  art_boolean left_live, right_live;
  double d;
  double new_x;
  ArtActiveSeg *test, *result = NULL;
  double x_test;

  left = seg;
  if (left == NULL)
    right = ctx->active_head;
  else
    right = left->right; 
  left_live = (break_flags & ART_BREAK_LEFT) && (left != NULL);
  right_live = (break_flags & ART_BREAK_RIGHT) && (right != NULL);

#ifdef VERBOSE
  double dd = 0;
  if(seg)
      dd = seg->a*x + seg->b*y + seg->c;
  art_dprint("add_point seg=%08x %f,%f%s%s (left=%08x, right=%08x) d=%f\n", seg, x, y, 
          break_flags&ART_BREAK_LEFT?" BREAK_LEFT":"",
          break_flags&ART_BREAK_LEFT?" BREAK_RIGHT":"",
          seg?seg->left:0, seg?seg->right:0, dd);
#endif
  /* add_point relies on the fact that the active list is ascending-
     no checks are done for lines which are not in strict order.

     a point position (x,y) is tested against left (if break_flags&ART_BREAK_LEFT)
     and right (if break_flags&ART_BREAK_RIGHT) neighboring segments which are
     within EPSILON_A distance of the point. If they are, they are split at y. 
     For ART_BREAK_LEFT, the "intersection point" on the line (which is to the left
     of (x,y) will never be to the right of "our" (x,y)- new_x will be shifted
     by _break to make sure of that. (Which should only happen for horizontal
     line segments) Likewise for ART_BREAK_RIGHT. 

     The horizontal area around (x,y) (x_min, x_max) will be extended into the
     break direction for every cut we make.
   */
		  
  //new_x = art_svp_intersect_break (ctx, left, x_min, y, ART_BREAK_LEFT);

  while (left_live || right_live)
    {
#ifdef VERBOSE
      art_dprint("  left: %08x (%d) right: %08x (%d)\n", left, left_live, right, right_live);
#endif
      if (left_live)
	{
	  if (x <= left->x[left->flags & ART_ACTIVE_FLAGS_BNEG] &&
	      /* It may be that one of these conjuncts turns out to be always
		 true. We test both anyway, to be defensive. */
	      y != left->y0 && y < left->y1)
	    {
	      d = x_min * left->a + y * left->b + left->c;
              if (d < EPSILON_A)
		{
		  new_x = art_svp_intersect_break (ctx, left, x_min, y,
						   ART_BREAK_LEFT);
#ifdef VERBOSE
                  art_dprint("break %08x at x<=%.16f,y=%.16f, new_x=%f\n", left, x_min, y, new_x);
#endif
		  if (new_x > x_max)
		    {
		      x_max = new_x;
		      right_live = (right != NULL);
		    }
		  else if (new_x < x_min)
		    x_min = new_x;
		  left = left->left;
		  left_live = (left != NULL);
		}
	      else
		left_live = ART_FALSE;
	    }
	  else
	    left_live = ART_FALSE;
	}
      else if (right_live)
	{
	  if (x >= right->x[(right->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1] &&
	      /* It may be that one of these conjuncts turns out to be always
		 true. We test both anyway, to be defensive. */
	      y != right->y0 && y < right->y1)
	    {
	      d = x_max * right->a + y * right->b + right->c;
              if (d > -EPSILON_A)
		{
		  new_x = art_svp_intersect_break (ctx, right, x_max, y,
						   ART_BREAK_RIGHT);
#ifdef VERBOSE
                  art_dprint("break %08x at x>=%.16f,y=%.16f, new_x=%f\n", right, x_max, y, new_x);
#endif
		  if (new_x < x_min)
		    {
		      x_min = new_x;
		      left_live = (left != NULL);
		    }
		  else if (new_x >= x_max)
		    x_max = new_x;
		  right = right->right;
		  right_live = (right != NULL);
		}
	      else
		right_live = ART_FALSE;
	    }
	  else
	    right_live = ART_FALSE;
	}
    }

  /* Ascending order is guaranteed by break_flags. Thus, we don't need
     to actually fix up non-ascending pairs. */

  /* Now, (left, right) defines an interval of segments broken. Sort
     into ascending x order. (find segment to the left of the new point) */
  test = left == NULL ? ctx->active_head : left->right;
  result = left;
  if (test != NULL && test != right)
    {
      if (y == test->y0)
	x_test = test->x[0];
      else if(y == test->y1)
	x_test = test->x[1];
      else {
        art_warn ("internal error in add_point: y=%.16f is between %.16f and %.16f\n", y, test->y0, test->y1);
	x_test = test->x[1];
        art_report_error();
      }

      for (;;)
	{
	  if (x_test <= x)
	    result = test;
	  test = test->right;
	  if (test == right)
	    break;

          /* FIXME the following code doesn't do anything (?) */
	  new_x = x_test;
	  if (new_x < x_test)
	    {
	      art_warn ("art_svp_intersect_add_point: non-ascending x\n");
	    }
	  x_test = new_x;
	}
    }
  return result;
}

static void
art_svp_intersect_swap_active (ArtIntersectCtx *ctx,
			       ArtActiveSeg *left_seg, ArtActiveSeg *right_seg)
{
  if((left_seg && left_seg->right != right_seg) ||
     (right_seg && right_seg->left != left_seg)) {
      art_warn("error: active list in disarray\n");
      art_report_error();
  }

  right_seg->left = left_seg->left;
  if (right_seg->left != NULL)
    right_seg->left->right = right_seg;
  else
    ctx->active_head = right_seg;
  left_seg->right = right_seg->right;
  if (left_seg->right != NULL)
    left_seg->right->left = left_seg;
  left_seg->left = right_seg;
  right_seg->right = left_seg;
}

volatile char add0 = 0;
static double double_precision(double x)
{
    /* make sure x has exactly 11 bits exponent and 52 bit mantissa-
       there is probably a more elegant (or faster) way to trick the compiler
       into doing this, but this works for now. */
    unsigned char xx[8];
    *(double*)xx = x;
    xx[0]+=add0; xx[1]+=add0; xx[2]+=add0; xx[3]+=add0;
    xx[4]+=add0; xx[5]+=add0; xx[6]+=add0; xx[7]+=add0;
    return *(double*)xx;
}

/**
 * art_svp_intersect_test_cross: Test crossing of a pair of active segments.
 * @ctx: Intersector context.
 * @left_seg: Left segment of the pair.
 * @right_seg: Right segment of the pair.
 * @break_flags: Flags indicating whether to break neighbors.
 *
 * Tests crossing of @left_seg and @right_seg. If there is a crossing,
 * inserts the intersection point into both segments.
 *
 * Return value: True if the intersection took place at the current
 * scan line, indicating further iteration is needed.
 **/
static art_boolean
art_svp_intersect_test_cross (ArtIntersectCtx *ctx,
			      ArtActiveSeg *left_seg, ArtActiveSeg *right_seg,
			      int break_flags)
{
  double left_x0, left_y0, left_x1;
  double left_y1 = left_seg->y1;
  double right_y1 = right_seg->y1;
  double d;

  const ArtSVPSeg *in_seg;
  int in_curs;
  double d0, d1, t;
  double x, y; /* intersection point */

#ifdef VERBOSE 
  static int count = 0;

  art_dprint ("art_svp_intersect_test_cross %08x <-> %08x: count=%d\n",
	  (unsigned long)left_seg, (unsigned long)right_seg, count++);
  art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", left_seg, 
          left_seg->x[0], left_seg->y0,
          left_seg->x[1], left_seg->y1);
  art_dprint ("(full:     %.16f,%.16f -> %.16f %.16f)\n", 
          left_seg->in_seg->points[left_seg->in_curs - 1].x, left_seg->in_seg->points[left_seg->in_curs - 1].y,
          left_seg->in_seg->points[left_seg->in_curs].x, left_seg->in_seg->points[left_seg->in_curs].y
          );

  art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", right_seg, 
          right_seg->x[0], right_seg->y0,
          right_seg->x[1], right_seg->y1);
  art_dprint ("(full:     %.16f,%.16f -> %.16f %.16f)\n", 
          right_seg->in_seg->points[right_seg->in_curs - 1].x, right_seg->in_seg->points[right_seg->in_curs - 1].y,
          right_seg->in_seg->points[right_seg->in_curs].x, right_seg->in_seg->points[right_seg->in_curs].y
          );
#endif

#ifdef CHEAP_SANITYCHECK
  if (right_seg->x[0] < left_seg->x[(left_seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1]) {
      /* notice: if we test *only* the line equation here, dd might be < 0, even though
         right_seg was inserted to the right of left_seg correctly, due to numerical
         instabilities */
      double dd = right_seg->x[0] * left_seg->a + right_seg->y0 * left_seg->b + left_seg->c;
      if(dd < 0) {
          //art_warn ("segment %08x[%d] isn't to the left of %08x[%d]. d=%.16f\n", 
          //        left_seg, left_seg->n_stack,
          //        right_seg, right_seg->n_stack,
          //        dd);
      }
  }
#endif

  if (left_seg->y0 == right_seg->y0 && left_seg->x[0] == right_seg->x[0])
    {
      /* Top points of left and right segments coincide. This case
	 feels like a bit of duplication - we may want to merge it
	 with the cases below. However, this way, we're sure that this
	 logic makes only localized changes. */

      if (left_y1 < right_y1)
	{
	  /* Test left (x1, y1) against right segment */
	  double left_x1 = left_seg->x[1];

	  if (left_x1 <
	      right_seg->x[(right_seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1] ||
	      left_y1 == right_seg->y0)
	    return ART_FALSE;
	  d = left_x1 * right_seg->a + left_y1 * right_seg->b + right_seg->c;
	  if (d < -EPSILON_A)
	    
              return ART_FALSE;
	  else if (d < EPSILON_A) /* should we use zero here? */
	    {
#ifdef VERBOSE 
                art_dprint("break %08x because of common top point, left_y1 < right_y1\n", right_seg);
#endif
	      /* I'm unsure about the break flags here. */
	      double right_x1 = art_svp_intersect_break (ctx, right_seg,
							 left_x1, left_y1,
							 ART_BREAK_RIGHT);
              
              /* this is always true due to ART_BREAK_RIGHT right_x1>=left_x1 if 
                 _break uses x_ref clipping */
	      if (left_x1 <= right_x1) {
		return ART_FALSE;
              }
	    }
	}
      else if (left_y1 > right_y1)
	{
	  /* Test right (x1, y1) against left segment */
	  double right_x1 = right_seg->x[1];

	  if (right_x1 > left_seg->x[left_seg->flags & ART_ACTIVE_FLAGS_BNEG] ||
	      right_y1 == left_seg->y0)
	    
              return ART_FALSE;
	  d = right_x1 * left_seg->a + right_y1 * left_seg->b + left_seg->c;
	  if (d > EPSILON_A)
	    return ART_FALSE;
	  else if (d > -EPSILON_A) /* should we use zero here? */
	    {
#ifdef VERBOSE 
              art_dprint("break %08x because of common top point, left_y1 > right_y1\n", left_seg);
#endif
	      /* See above regarding break flags. */
	      double left_x1 = art_svp_intersect_break (ctx, left_seg,
							right_x1, right_y1,
							ART_BREAK_LEFT);

              /* this is always true, due to ART_BREAK_LEFT left_x1<=right_x1, if
                 _break uses x_ref clipping
               */
	      if (left_x1 <= right_x1) {
		return ART_FALSE;
              }
	    }
	}
      else /* left_y1 == right_y1 */
	{
	  double left_x1 = left_seg->x[1];
	  double right_x1 = right_seg->x[1];

	  
          if (left_x1 <= right_x1)
	    return ART_FALSE;
	}

      //int wind_left = left_seg->wind_left;
      //int wind_right = right_seg->wind_left;
      art_svp_intersect_swap_active (ctx, left_seg, right_seg);
      //left_seg->wind_left = wind_right;
      //right_seg->wind_left = wind_left;

      return ART_TRUE;
    }

  if (left_y1 < right_y1)
    {
      /* Test left (x1, y1) against right segment */
      double left_x1 = left_seg->x[1];

      if (left_x1 <
	  right_seg->x[(right_seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1] ||
	  left_y1 == right_seg->y0)
	return ART_FALSE;

      if(left_y1 < right_seg->y0) {
          art_warn("left_y1 < right_seg->y0\n");
          return ART_FALSE;
      }

      /* we might want to output a warning for left_y1 < right_seg->y0 */

      d = left_x1 * right_seg->a + left_y1 * right_seg->b + right_seg->c;
      if (d < -EPSILON_A)
	return ART_FALSE;
      else if (d < EPSILON_A)
	{
#ifdef VERBOSE 
          art_dprint("break %08x at %.16f because left_y1 < right_y1\n", right_seg, left_y1);
          art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", right_seg, 
                  right_seg->x[0], right_seg->y0,
                  right_seg->x[1], right_seg->y1);
#endif
	  double right_x1 = art_svp_intersect_break (ctx, right_seg,
						     left_x1, left_y1,
						     ART_BREAK_RIGHT);
#ifdef VERBOSE 
          art_dprint("after break:\n", right_seg);
          art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", right_seg, 
                  right_seg->x[0], right_seg->y0,
                  right_seg->x[1], right_seg->y1);
#endif
          /* this is always true if _break uses x_ref clipping */
	  if (left_x1 <= right_x1)
	    return ART_FALSE;
	}
    }
  else if (left_y1 > right_y1)
    {
      /* Test right (x1, y1) against left segment */
      double right_x1 = right_seg->x[1];

      if (right_x1 > left_seg->x[left_seg->flags & ART_ACTIVE_FLAGS_BNEG] ||
	  right_y1 == left_seg->y0)
	return ART_FALSE;
      
      if(right_y1 < left_seg->y0) {
          art_warn("left_y1 < right_seg->y0\n");
          return ART_FALSE;
      }
      
      /* we might want to output a warning for right_y1 < left_seg->y0 */

      d = right_x1 * left_seg->a + right_y1 * left_seg->b + left_seg->c;
      if (d > EPSILON_A)
	return ART_FALSE;
      else if (d > -EPSILON_A)
	{
#ifdef VERBOSE 
          art_dprint("break %08x because left_y1 > right_y1\n", right_seg);
#endif
	  double left_x1 = art_svp_intersect_break (ctx, left_seg,
						    right_x1, right_y1,
						    ART_BREAK_LEFT);
          /* this is always true if _break uses x_ref clipping */
	  if (left_x1 <= right_x1)
	    return ART_FALSE;
	}
    }
  else /* left_y1 == right_y1 */
    { 
      double left_x1 = left_seg->x[1];
      double right_x1 = right_seg->x[1];

      if (left_x1 <= right_x1)
	return ART_FALSE;
    }


  /* The segments cross. Find the intersection point. */

  in_seg = left_seg->in_seg;
  in_curs = left_seg->in_curs;
  left_x0 = in_seg->points[in_curs - 1].x;
  left_y0 = in_seg->points[in_curs - 1].y;
  left_x1 = in_seg->points[in_curs].x;
  left_y1 = in_seg->points[in_curs].y;

#if 0
  /* check for endpoint = firstpoint cases */
  if(left_seg->y0 == right_seg->y1 && left_seg->x[0] == right_seg->x[1])
      return ART_FALSE;
  if(left_seg->y1 == right_seg->y0 && left_seg->x[1] == right_seg->x[0])
      return ART_FALSE;
#endif

  d0 = left_x0 * right_seg->a + left_y0 * right_seg->b + right_seg->c;
  d1 = left_x1 * right_seg->a + left_y1 * right_seg->b + right_seg->c;
  if (d0 == d1)
    {
      x = left_x0;
      y = left_y0;
    }
  else
    {
      /* Is this division always safe? It could possibly overflow. */
      t = d0 / (d0 - d1);
      if (t <= 0)
	{
	  x = left_x0;
	  y = left_y0;
	}
      else if (t >= 1)
	{
	  x = left_x1;
	  y = left_y1;
	}
      else
	{
	  x = left_x0 + t * (left_x1 - left_x0);
	  y = left_y0 + t * (left_y1 - left_y0);
	}
    }

  /* Make sure intersection point is within bounds of right seg. */
  if (y < right_seg->y0)
    {
      x = right_seg->x[0];
      y = right_seg->y0;
    }
  else if (y > right_seg->y1)
    {
      x = right_seg->x[1];
      y = right_seg->y1;
    }
  else if (x < right_seg->x[(right_seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1])
    x = right_seg->x[(right_seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1];
  else if (x > right_seg->x[right_seg->flags & ART_ACTIVE_FLAGS_BNEG])
    x = right_seg->x[right_seg->flags & ART_ACTIVE_FLAGS_BNEG];

  x = double_precision(x);
  y = double_precision(y);

  assert(ctx->y >= left_seg->y0);
#ifdef VERBOSE
  art_dprint ("intersect at %.16f %.16f\n", x, y);
#endif


  if(y < ctx->y) {
      /* as we use the full segment length (not just the subsegment currently
         under evaluation), intersection points may be above the current scanline.
         As we're not able to process these anymore, we also don't need to add
         anything to the active list or pq.

         Intersection points above the current scanline happen if an
         intersection is handled twice- once when the line is inserted, and
         once when e.g. some other intersection point triggers insert_cross.
      */
      art_warn ("previously unhandled intersection point %.16f %.16f (dy=%.16f)\n", x, y, ctx->y - y);
      return ART_FALSE;
  }
  
  if(y > left_seg->y1) {
      /* not within the subsegment we're currently looking into- this is not an intersection */
      return ART_FALSE;
  }

  if (y == left_seg->y0)
    {
      if (y != right_seg->y0)
	{
#ifdef VERBOSE
	  art_dprint ("art_svp_intersect_test_cross: intersection (%.16f, %.16f) matches former y0 of %08x, [%08x]\n",
		    x, y, (unsigned long)left_seg, (unsigned long)right_seg);
#endif
	  art_svp_intersect_push_pt (ctx, right_seg, x, y);
	  if ((break_flags & ART_BREAK_RIGHT) && right_seg->right != NULL)
	    art_svp_intersect_add_point (ctx, x, y, right_seg->right,
					 break_flags);
	}
      else
	{
#ifdef VERBOSE
	  art_dprint ("art_svp_intersect_test_cross: intersection (%.16f, %.16f) at current scan line (y=ly0=ry0)\n",
		    x, y, (unsigned long)left_seg, (unsigned long)right_seg);
#endif
	  /* Intersection takes place at current scan line, with
             left->x0 <= x <= right->x0, left->x0 != right->x0.

             This happens if one of the lines is horizontal, or very near
             horizontal. (true horizontal lines are processed by _horiz())
             
             Process immediately rather than queueing intersection point into
	     priq. */
	  ArtActiveSeg *winner, *loser;

	  /* Choose "most vertical" segement */
	  if (left_seg->a > right_seg->a)
	    {
	      winner = left_seg;
	      loser = right_seg;
	    }
	  else
	    {
	      winner = right_seg;
	      loser = left_seg;
	    }
#ifdef VERBOSE
          art_dprint ("           x = %.16f\n", x);
          art_dprint (" loser->x[0] = %.16f\n", loser->x[0]);
          art_dprint ("winner->x[0] = %.16f\n", winner->x[0]);
#endif
	  loser->x[0] = winner->x[0];
	  loser->horiz_x = loser->x[0];
	  loser->horiz_delta_wind += loser->delta_wind;
	  winner->horiz_delta_wind -= loser->delta_wind;

	  art_svp_intersect_swap_active (ctx, left_seg, right_seg);
	  return ART_TRUE;
	}
    }
  else if (y == right_seg->y0)
    {
#ifdef VERBOSE
      art_dprint ("*** art_svp_intersect_test_cross: intersection (%.16f, %.16f) matches latter y0 of [%08x], %08x\n",
	      x, y, (unsigned long)left_seg, (unsigned long)right_seg);
      art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", left_seg, 
              left_seg->x[0], left_seg->y0,
              left_seg->x[1], left_seg->y1);
      art_dprint ("%08x = %.16f,%.16f -> %.16f %.16f\n", right_seg, 
              right_seg->x[0], right_seg->y0,
              right_seg->x[1], right_seg->y1);

#endif
      art_svp_intersect_push_pt (ctx, left_seg, x, y);
      if ((break_flags & ART_BREAK_LEFT) && left_seg->left != NULL)
	art_svp_intersect_add_point (ctx, x, y, left_seg->left,
				     break_flags);
    }
  else
    {
#ifdef VERBOSE
      art_dprint ("Inserting (%.16f, %.16f) into %08x, %08x\n",
	      x, y, (unsigned long)left_seg, (unsigned long)right_seg);
#endif
      /* Insert the intersection point into both segments. */
      art_svp_intersect_push_pt (ctx, left_seg, x, y);
      art_svp_intersect_push_pt (ctx, right_seg, x, y);
      if ((break_flags & ART_BREAK_LEFT) && left_seg->left != NULL)
	art_svp_intersect_add_point (ctx, x, y, left_seg->left, ART_BREAK_LEFT | ART_BREAK_RIGHT);
      if ((break_flags & ART_BREAK_RIGHT) && right_seg->right != NULL)
	art_svp_intersect_add_point (ctx, x, y, right_seg->right, ART_BREAK_LEFT | ART_BREAK_RIGHT);
    }
  return ART_FALSE;
}

/**
 * art_svp_intersect_active_delete: Delete segment from active list.
 * @ctx: Intersection context.
 * @seg: Segment to delete.
 *
 * Deletes @seg from the active list.
 **/
static /* todo inline */ void
art_svp_intersect_active_delete (ArtIntersectCtx *ctx, ArtActiveSeg *seg)
{
  ArtActiveSeg *left = seg->left, *right = seg->right;

  if (left != NULL)
    left->right = right;
  else
    ctx->active_head = right;
  if (right != NULL)
    right->left = left;
}

/**
 * art_svp_intersect_active_free: Free an active segment.
 * @seg: Segment to delete.
 *
 * Frees @seg.
 **/
static /* todo inline */ void
art_svp_intersect_active_free (ArtActiveSeg *seg)
{
  art_free (seg->stack);
#ifdef VERBOSE
  art_dprint ("Freeing %08x\n", (unsigned long) seg);
#else
  // !!!
  art_free (seg);
#endif
}

/**
 * art_svp_intersect_insert_cross: Test crossings of newly inserted line.
 *
 * Tests @seg against its left and right neighbors for intersections.
 * Precondition: the line in @seg is not purely horizontal.
 **/
static void
art_svp_intersect_insert_cross (ArtIntersectCtx *ctx,
				ArtActiveSeg *seg)
{
  ArtActiveSeg *left = seg, *right = seg;

  for (;;)
    {
      if (left != NULL)
	{
	  ArtActiveSeg *leftc;

	  for (leftc = left->left; leftc != NULL; leftc = leftc->left)
	    if (!(leftc->flags & ART_ACTIVE_FLAGS_DEL))
	      break;
	  if (leftc != NULL &&
	      art_svp_intersect_test_cross (ctx, leftc, left,
					    ART_BREAK_LEFT))
	    {
	      if (left == right || right == NULL)
		right = left->right;
	    }
	  else
	    {
	      left = NULL;
	    }
	}
      else if (right != NULL && right->right != NULL)
	{
	  ArtActiveSeg *rightc;

	  for (rightc = right->right; rightc != NULL; rightc = rightc->right)
	    if (!(rightc->flags & ART_ACTIVE_FLAGS_DEL))
	      break;
	  if (rightc != NULL &&
	      art_svp_intersect_test_cross (ctx, right, rightc,
					    ART_BREAK_RIGHT))
	    {
	      if (left == right || left == NULL)
		left = right->left;
	    }
	  else
	    {
	      right = NULL;
	    }
	}
      else
	break;
    }
}

/**
 * art_svp_intersect_horiz: Add horizontal line segment.
 * @ctx: Intersector context.
 * @seg: Segment on which to add horizontal line.
 * @x0: Old x position.
 * @x1: New x position.
 *
 * Adds a horizontal line from @x0 to @x1, and updates the current
 * location of @seg to @x1.
 **/
static void
art_svp_intersect_horiz (ArtIntersectCtx *ctx, ArtActiveSeg *seg,
			 double x0, double x1)
{
  ArtActiveSeg *hs;

  if (x0 == x1)
    return;

#ifdef VERBOSE
  art_dprint("horiz: seg=%08x x0=%f x1=%f segid=%08x flags=%s delta_wind=%d\n", seg, x0, x1, seg->seg_id,
          seg->flags & ART_ACTIVE_FLAGS_OUT?"out":"", seg->delta_wind);
#endif

  hs = art_new (ArtActiveSeg, 1);

  hs->flags = ART_ACTIVE_FLAGS_DEL | (seg->flags & ART_ACTIVE_FLAGS_OUT);
  if (seg->flags & ART_ACTIVE_FLAGS_OUT)
    {
      ArtSvpWriter *swr = ctx->out;

      swr->add_point (swr, seg->seg_id, x0, ctx->y);
    }
  hs->seg_id = seg->seg_id;
  hs->horiz_x = x0;
  hs->horiz_delta_wind = seg->delta_wind;
  hs->stack = NULL;

  /* Ideally, the (a, b, c) values will never be read. However, there
     are probably some tests remaining that don't check for _DEL
     before evaluating the line equation. For those, these
     initializations will at least prevent a UMR of the values, which
     can crash on some platforms. */
  hs->a = 0.0;
  hs->b = 0.0;
  hs->c = 0.0;

  seg->horiz_delta_wind -= seg->delta_wind;

  art_svp_intersect_add_horiz (ctx, hs);

  if (x0 > x1)
    {
      ArtActiveSeg *left;
      art_boolean first = ART_TRUE;

      for (left = seg->left; left != NULL; left = seg->left)
	{
	  int left_bneg = left->flags & ART_ACTIVE_FLAGS_BNEG;

	  if (left->x[left_bneg] <= x1)
	    break;
	  if (left->x[left_bneg ^ 1] <= x1 &&
	      x1 * left->a + ctx->y * left->b + left->c >= 0)
	    break;
	  if (left->y0 != ctx->y && left->y1 != ctx->y)
	    {
	      art_svp_intersect_break (ctx, left, x1, ctx->y, ART_BREAK_LEFT);
	    }
#ifdef VERBOSE
	  art_dprint ("x0=%.16f > x1=%.16f, swapping %08x, %08x\n",
		  x0, x1, (unsigned long)left, (unsigned long)seg);
#endif
	  art_svp_intersect_swap_active (ctx, left, seg);
	  if (first && left->right != NULL)
	    {
	      art_svp_intersect_test_cross (ctx, left, left->right,
					    ART_BREAK_RIGHT);
	      first = ART_FALSE;
	    }
	}
    }
  else
    {
      ArtActiveSeg *right;
      art_boolean first = ART_TRUE;

      for (right = seg->right; right != NULL; right = seg->right)
	{
	  int right_bneg = right->flags & ART_ACTIVE_FLAGS_BNEG;

	  if (right->x[right_bneg ^ 1] >= x1)
	    break;
	  if (right->x[right_bneg] >= x1 &&
	      x1 * right->a + ctx->y * right->b + right->c <= 0)
	    break;
	  if (right->y0 != ctx->y && right->y1 != ctx->y)
	    {
	      art_svp_intersect_break (ctx, right, x1, ctx->y,
				       ART_BREAK_LEFT);
	    }
#ifdef VERBOSE
	  art_dprint ("[right]x0=%.16f < x1=%.16f, swapping %08x, %08x\n",
		  x0, x1, (unsigned long)seg, (unsigned long)right);
#endif
	  art_svp_intersect_swap_active (ctx, seg, right);
	  if (first && right->left != NULL)
	    {
	      art_svp_intersect_test_cross (ctx, right->left, right,
					    ART_BREAK_RIGHT);
	      first = ART_FALSE;
	    }
	}
    }

  seg->x[0] = x1;
  seg->x[1] = x1;
  seg->horiz_x = x1;
  seg->flags &= ~ART_ACTIVE_FLAGS_OUT;
}

/**
 * art_svp_intersect_insert_line: Insert a line into the active list.
 * @ctx: Intersector context.
 * @seg: Segment containing line to insert.
 *
 * Inserts the line into the intersector context, taking care of any
 * intersections, and adding the appropriate horizontal points to the
 * active list.
 **/
static void
art_svp_intersect_insert_line (ArtIntersectCtx *ctx, ArtActiveSeg *seg)
{
  if (seg->y1 == seg->y0)
    {
#ifdef VERBOSE
      art_dprint ("art_svp_intersect_insert_line(%d): %08x is horizontal\n", seg->flags&ART_ACTIVE_FLAGS_IN_ACTIVE, (unsigned long)seg);
#endif
      art_svp_intersect_horiz (ctx, seg, seg->x[0], seg->x[1]);
    }
  else
    {
#ifdef VERBOSE
      art_dprint ("art_svp_intersect_insert_line(%d): %08x is non-horizontal\n", seg->flags&ART_ACTIVE_FLAGS_IN_ACTIVE, (unsigned long)seg);
#endif
      art_svp_intersect_insert_cross (ctx, seg);
      art_svp_intersect_add_horiz (ctx, seg);
    }

  seg->flags |= ART_ACTIVE_FLAGS_IN_ACTIVE; //q
}

static void
art_svp_intersect_process_intersection (ArtIntersectCtx *ctx,
					ArtActiveSeg *seg)
{
  int n_stack = --seg->n_stack;
  seg->x[1] = seg->stack[n_stack - 1].x;
  seg->y1 = seg->stack[n_stack - 1].y;
  seg->x[0] = seg->stack[n_stack].x;
  seg->y0 = seg->stack[n_stack].y;
  seg->horiz_x = seg->x[0];
#ifdef VERBOSE
  art_dprint("process intersection %08x (%.16f,%.16f-%.16f,%.16f)\n", seg, seg->x[0], seg->y0, seg->x[1], seg->y1);
#endif
  art_svp_intersect_insert_line (ctx, seg);
}

static void
art_svp_intersect_advance_cursor (ArtIntersectCtx *ctx, ArtActiveSeg *seg,
				  ArtPriPoint *pri_pt)
{
  const ArtSVPSeg *in_seg = seg->in_seg;
  int in_curs = seg->in_curs;
  ArtSvpWriter *swr = seg->flags & ART_ACTIVE_FLAGS_OUT ? ctx->out : NULL;

  if (swr != NULL)
    swr->add_point (swr, seg->seg_id, seg->x[1], seg->y1);
  if (in_curs + 1 >= in_seg->n_points)
    {
      ArtActiveSeg *left = seg->left, *right = seg->right;

#if 0
      if (swr != NULL)
	swr->close_segment (swr, seg->seg_id);
      seg->flags &= ~ART_ACTIVE_FLAGS_OUT;
#endif
      seg->flags |= ART_ACTIVE_FLAGS_DEL;
      art_svp_intersect_add_horiz (ctx, seg);
      art_svp_intersect_active_delete (ctx, seg);
      if (left != NULL && right != NULL)
	art_svp_intersect_test_cross (ctx, left, right,
				      ART_BREAK_LEFT | ART_BREAK_RIGHT);
      art_free (pri_pt);
    }
  else
    {
      seg->horiz_x = seg->x[1];

      art_svp_intersect_setup_seg (seg, pri_pt);
      art_pri_insert (ctx->pq, pri_pt);
      art_svp_intersect_insert_line (ctx, seg);
    }
}

static void
art_svp_intersect_add_seg (ArtIntersectCtx *ctx, const ArtSVPSeg *in_seg)
{
  ArtActiveSeg *seg = art_new (ArtActiveSeg, 1);
  ArtActiveSeg *test;
  double x0, y0;
  ArtActiveSeg *beg_range;
  ArtActiveSeg *last = NULL;
  ArtActiveSeg *left, *right;
  ArtPriPoint *pri_pt = art_new (ArtPriPoint, 1);

  seg->flags = 0;
  seg->in_seg = in_seg;
  seg->in_curs = 0;

  seg->n_stack_max = 4;
  seg->stack = art_new (ArtPoint, seg->n_stack_max);

  seg->horiz_delta_wind = 0;
  
  seg->wind_left = 0;

  pri_pt->user_data = seg;
  art_svp_intersect_setup_seg (seg, pri_pt);
  art_pri_insert (ctx->pq, pri_pt);

  /* Find insertion place for new segment */
  /* This is currently a left-to-right scan, but should be replaced
     with a binary search as soon as it's validated. */

  x0 = in_seg->points[0].x;
  y0 = in_seg->points[0].y;
  beg_range = NULL;
  for (test = ctx->active_head; test != NULL; test = test->right)
    {
      double d;
      int test_bneg = test->flags & ART_ACTIVE_FLAGS_BNEG;

      if (x0 < test->x[test_bneg])
	{
	  if (x0 < test->x[test_bneg ^ 1]) {
#ifdef VERBOSE
            art_dprint("inserting segment %08x before %08x (x0 < xmin)\n", seg, test);
#endif
	    break;
          }
	  d = x0 * test->a + y0 * test->b + test->c;
	  if (d < 0) {
#ifdef VERBOSE
            art_dprint("inserting segment %08x before %08x (d = %.16f)\n", seg, test, d);
#endif
	    break;
          }
#ifdef VERBOSE
          art_dprint("segment %08x is to the right of %08x d=%.16f\n", seg, test, d);
#endif
	} else {
#ifdef VERBOSE
	    d = x0 * test->a + y0 * test->b + test->c;
            art_dprint("segment %08x is to the right of %08x d=%.16f (not used)\n", seg, test, d);
#endif
        }
      last = test;
    }

  left = art_svp_intersect_add_point (ctx, x0, y0, last, ART_BREAK_LEFT | ART_BREAK_RIGHT);

#ifdef VERBOSE
  art_dprint("left=%08x x0=%.16f y=%.16f\n", left, x0, y0);
#endif

  seg->left = left;
  if (left == NULL)
    {
      right = ctx->active_head;
      ctx->active_head = seg;
    }
  else
    {
      right = left->right;
      left->right = seg;
    }
  seg->right = right;
  if (right != NULL)
    right->left = seg;

  seg->delta_wind = in_seg->dir ? 1 : -1;
  seg->horiz_x = x0;

  art_svp_intersect_insert_line (ctx, seg);
}

#ifdef SANITYCHECK
static void
art_svp_intersect_sanitycheck_winding (ArtIntersectCtx *ctx)
{
#if 0
  /* At this point, we seem to be getting false positives, so it's
     turned off for now. */

  ArtActiveSeg *seg;
  int winding_number = 0;

  for (seg = ctx->active_head; seg != NULL; seg = seg->right)
    {
      /* Check winding number consistency. */
      if (seg->flags & ART_ACTIVE_FLAGS_OUT)
	{
	  if (winding_number != seg->wind_left)
	    art_warn ("*** art_svp_intersect_sanitycheck_winding: seg %08x has wind_left of %d, expected %d\n",
		  (unsigned long) seg, seg->wind_left, winding_number);
	  winding_number = seg->wind_left + seg->delta_wind;
	}
    }
  if (winding_number != 0)
    art_warn ("*** art_svp_intersect_sanitycheck_winding: non-balanced winding number %d\n",
	      winding_number);
#endif
}
#endif

/**
 * art_svp_intersect_horiz_commit: Commit points in horiz list to output.
 * @ctx: Intersection context.
 *
 * The main function of the horizontal commit is to output new
 * points to the output writer.
 *
 * This "commit" pass is also where winding numbers are assigned,
 * because doing it here provides much greater tolerance for inputs
 * which are not in strict SVP order.
 *
 * Each cluster in the horiz_list contains both segments that are in
 * the active list (ART_ACTIVE_FLAGS_DEL is false) and that are not,
 * and are scheduled to be deleted (ART_ACTIVE_FLAGS_DEL is true). We
 * need to deal with both.
 **/
static void
art_svp_intersect_horiz_commit (ArtIntersectCtx *ctx)
{
  ArtActiveSeg *seg;
  int winding_number = 0; /* initialization just to avoid warning */
  int horiz_wind = 0;
  double last_x = 0; /* initialization just to avoid warning */

#ifdef VERBOSE
  art_dprint ("art_svp_intersect_horiz_commit: y=%.16f\n", ctx->y);
  for (seg = ctx->horiz_first; seg != NULL; seg = seg->horiz_right)
    art_dprint (" %08x: %.16f %+d segid=%d\n",
	    (unsigned long)seg, seg->horiz_x, seg->horiz_delta_wind, seg->seg_id);
#endif

  /* Output points to svp writer. */
  for (seg = ctx->horiz_first; seg != NULL;)
    {
      /* Find a cluster with common horiz_x, */
      ArtActiveSeg *curs;
      double x = seg->horiz_x;

      /* Generate any horizontal segments. */
      if (horiz_wind != 0)
	{
	  ArtSvpWriter *swr = ctx->out;
	  int seg_id;

#ifdef VERBOSE
          art_dprint ("generate horizontal segment: y=%.16f x=%.16f-%.16f wind=%d horiz_wind=%d\n", ctx->y, last_x, x, winding_number, horiz_wind);
#endif
	  seg_id = swr->add_segment (swr, winding_number, horiz_wind,
				     last_x, ctx->y);
	  swr->add_point (swr, seg_id, x, ctx->y);
	  swr->close_segment (swr, seg_id);
	}

      /* Find first active segment in cluster. */

      for (curs = seg; curs != NULL && curs->horiz_x == x;
	   curs = curs->horiz_right)
	if (!(curs->flags & ART_ACTIVE_FLAGS_DEL))
	  break;

      if (curs != NULL && curs->horiz_x == x)
	{
	  /* There exists at least one active segment in this cluster. */

	  /* Find beginning of cluster. */
	  for (; curs->left != NULL; curs = curs->left)
	    if (curs->left->horiz_x != x)
	      break;

	  if (curs->left != NULL)
	    winding_number = curs->left->wind_left + curs->left->delta_wind;
	  else
	    winding_number = 0;

	  do
	    {
#ifdef VERBOSE
	      art_dprint (" curs %08x: winding_number = %d += %d\n", (unsigned long)curs, winding_number, curs->delta_wind);
#endif
	      if (!(curs->flags & ART_ACTIVE_FLAGS_OUT) ||
		  curs->wind_left != winding_number)
		{
		  ArtSvpWriter *swr = ctx->out;

		  if (curs->flags & ART_ACTIVE_FLAGS_OUT)
		    {
		      swr->add_point (swr, curs->seg_id,
				      curs->horiz_x, ctx->y);
		      swr->close_segment (swr, curs->seg_id);
		    }

		  curs->seg_id = swr->add_segment (swr, winding_number,
						   curs->delta_wind,
						   x, ctx->y);
		  curs->flags |= ART_ACTIVE_FLAGS_OUT;
		}
	      curs->wind_left = winding_number;
	      winding_number += curs->delta_wind;
	      curs = curs->right;
	    }
	  while (curs != NULL && curs->horiz_x == x);
	}

      /* Skip past cluster. */
      do
	{
	  ArtActiveSeg *next = seg->horiz_right;

	  seg->flags &= ~ART_ACTIVE_FLAGS_IN_HORIZ;
	  horiz_wind += seg->horiz_delta_wind;
	  seg->horiz_delta_wind = 0;
	  if (seg->flags & ART_ACTIVE_FLAGS_DEL)
	    {
	      if (seg->flags & ART_ACTIVE_FLAGS_OUT)
		{
		  ArtSvpWriter *swr = ctx->out;
		  swr->close_segment (swr, seg->seg_id);
		}
	      art_svp_intersect_active_free (seg);
	    }
	  seg = next;
	}
      while (seg != NULL && seg->horiz_x == x);

      last_x = x;
    }
  ctx->horiz_first = NULL;
  ctx->horiz_last = NULL;
#ifdef SANITYCHECK
  art_svp_intersect_sanitycheck_winding (ctx);
#endif
}

#ifdef SANITYCHECK
static void
art_svp_intersect_sanitycheck (ArtIntersectCtx *ctx, double y)
{
  ArtActiveSeg *seg;
  ArtActiveSeg *last = NULL;
  double d;

#ifdef VERbOSE
  art_dprint ("Active list (y = %.16f (new: %.16f)):\n", ctx->y, y);
#endif

  for (seg = ctx->active_head; seg != NULL; seg = seg->right)
    {
#ifdef VERBOSE
      char c1=' ',c2=' ',e1=' ';
      if(seg->y0 > ctx->y) c1='!';
      if(seg->y0 > y) c2='!';
      if(seg->y0 > seg->y1) e1='E';
#endif

      if (seg->left != last)
	{
	  art_warn ("*** art_svp_intersect_sanitycheck: last=%08x, seg->left=%08x\n",
		    (unsigned long)last, (unsigned long)seg->left);
	}
      if (last != NULL)
	{
	  /* pairwise compare with previous seg */

	  /* First the top. */
	  if (last->y0 < seg->y0)
	    {
	    }
	  else
	    {
	    }

	  /* Then the bottom. */
	  if (last->y1 < seg->y1)
	    {
	      if (!((last->x[1] <
		     seg->x[(seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1]) ||
		    last->y1 == seg->y0))
		{
		  d = last->x[1] * seg->a + last->y1 * seg->b + seg->c;
		  if (d >= EPSILON_A) {
		    art_warn ("*** bottom (%.16f, %.16f) of %08x is not clear of %08x to right (d = %g)\n",
			      last->x[1], last->y1, (unsigned long) last,
			      (unsigned long) seg, d);
                    art_report_error();
                  } else {
#ifdef VERBOSE
                      art_dprint("is to the left (d=%.16f of previous x1,y1) of\n", d);
#endif
                  }
		} else {
#ifdef VERBOSE
                    art_dprint("is to the left (previous x1 < next x%d) of\n", (seg->flags & ART_ACTIVE_FLAGS_BNEG) ^ 1);
#endif
                }
	    }
	  else if (last->y1 > seg->y1)

	    {
	      if (!((seg->x[1] >
		     last->x[last->flags & ART_ACTIVE_FLAGS_BNEG]) ||
		    seg->y1 == last->y0))
	      {
		d = seg->x[1] * last->a + seg->y1 * last->b + last->c;
		if (d < -EPSILON_A) {
		  art_warn ("*** bottom (%.16f, %.16f) of %08x is not clear of %08x to left (d = %.16f)\n",
			      seg->x[1], seg->y1, (unsigned long) seg,
			      (unsigned long) last, d);
                  art_report_error();
                } else {
#ifdef VERBOSE
                  art_dprint("is to the left (d=%.16f of next x1,y1) of\n", d);
#endif
                } 
	      } else {
#ifdef VERBOSE
                  art_dprint("is to the left (next x1 > previous x%d) of\n", last->flags & ART_ACTIVE_FLAGS_BNEG);
#endif
              }
	    }
	  else
	    {
	      if (last->x[1] - seg->x[1] > EPSILON_A) {
		art_warn ("*** bottoms (%.16f, %.16f) of %08x and (%.16f, %.16f) of %08x out of order\n",
			  last->x[1], last->y1, (unsigned long)last,
			  seg->x[1], seg->y1, (unsigned long)seg);
                art_report_error();
              } else {
#ifdef VERBOSE
                  art_dprint("is to the left (y1s equal, previous x1 < next x1)\n");
#endif
              }
	    }
	}

#ifdef VERBOSE
      art_dprint ("%c%08x: %c%c %02d (%.16f, %.16f)-(%.16f, %.16f) ",
              e1,
	      (unsigned long)seg, c1, c2, seg->flags,
	      seg->x[0], seg->y0, seg->x[1], seg->y1);
#endif

      last = seg;
    }
#ifdef VERBOSE
  art_dprint("\n");
#endif
}
#endif

void
art_svp_intersector (const ArtSVP *in, ArtSvpWriter *out)
{
  ArtIntersectCtx *ctx;
  ArtPriQ *pq;
  ArtPriPoint *first_point;
#ifdef VERBOSE
  int count = 0;
#endif

  if (in->n_segs == 0)
    return;
  
  current_svp = in;

#ifdef VERBOSE
  int t;
  art_dprint("Segments: %d\n", in->n_segs);
  for(t=0;t<in->n_segs;t++) {
      const ArtSVPSeg* seg = &in->segs[t];
      art_dprint("Segment %08x(%d): %d points, %s, BBox: (%f,%f,%f,%f)\n", 
              seg, t, seg->n_points, seg->dir==0?"UP  ":"DOWN",
              seg->bbox.x0, seg->bbox.y0, seg->bbox.x1, seg->bbox.y1);
      int p;
      for(p=0;p<seg->n_points;p++) {
          ArtPoint* point = &seg->points[p];
          art_dprint("        (%.16f,%.16f)\n", point->x, point->y);
      }
  }
  art_dprint("\n");
#endif

  ctx = art_new (ArtIntersectCtx, 1);
  ctx->in = in;
  ctx->out = out;
  pq = art_pri_new ();
  ctx->pq = pq;

  ctx->active_head = NULL;

  ctx->horiz_first = NULL;
  ctx->horiz_last = NULL;

  ctx->in_curs = 0;
  first_point = art_new (ArtPriPoint, 1);
  first_point->x = in->segs[0].points[0].x;
  first_point->y = in->segs[0].points[0].y;
  first_point->user_data = NULL;
  ctx->y = first_point->y;
  art_pri_insert (pq, first_point);

  double lasty = -HUGE_VAL;
  while (!art_pri_empty (pq))
    {
      ArtPriPoint *pri_point = art_pri_choose (pq);
      ArtActiveSeg *seg = (ArtActiveSeg *)pri_point->user_data;

#ifdef VERBOSE
      art_dprint ("\nIntersector step %d (%d items in pq)\n", count++, pq->n_items);
#endif
#ifdef SANITYCHECK
      art_svp_intersect_sanitycheck(ctx, pri_point->y);
#endif
#ifdef VERBOSE
      art_dprint ("priq choose (%.16f, %.16f) %08x\n", pri_point->x, pri_point->y,
	      (unsigned long)pri_point->user_data);
      if(seg) {
          art_dprint ("            %08x = %.16f,%.16f -> %.16f %.16f\n", seg, seg->x[0], seg->y0, seg->x[1], seg->y1);
      }
      //int t;
      //for(t=0;t<pq->n_items;t++) {
      //    art_dprint("pq[%d] %.16f,%.16f %08x\n", 
      //            t,
      //            pq->items[t]->x,
      //            pq->items[t]->y,
      //            pq->items[t]->user_data);
      //}
#endif

      //art_dprint("y=%f %08x\n", pri_point->y, pri_point->user_data);
      if (ctx->y != pri_point->y)
	{
	  art_svp_intersect_horiz_commit (ctx);
	  ctx->y = pri_point->y;
	}

      if(ctx->y < lasty) {
          art_warn("y decreased\n");
          art_report_error();
      }

      if (seg == NULL)
	{
	  /* Insert new segment from input */
	  const ArtSVPSeg *in_seg = 0;
          
          while(1) {
              in_seg = &in->segs[ctx->in_curs++];
              if(in_seg->n_points > 1)
                  break;
              if(ctx->in_curs == in->n_segs) {
                  in_seg = 0;
                  break;
              }

#ifdef VERBOSE 
              art_dprint("ignoring input segment %08x- it only contains %d point(s)\n", 
                      in_seg, in_seg->n_points);
#endif
          }
        
          if(in_seg) {
              int t;
              int error = 0;
              for(t=0;t<in_seg->n_points-1;t++) {
                  if(!(in_seg->points[t].y <= in_seg->points[t+1].y)) {
                      error = 1;
                  }
              }
              if(error) {
                  art_warn("bad input: contains a segment with descending y\n");
                  for(t=0;t<in_seg->n_points;t++) {
                      art_dprint("%d) %.16f %.16f\n", t, in_seg->points[t].x, in_seg->points[t].y);
                  }
                  art_report_error();
              }

#ifdef VERBOSE
              art_dprint("insert new segment from input (%.16f,%.16f) dir=%d\n", in_seg->points[0].x, in_seg->points[0].y, in_seg->dir);
#endif
              art_svp_intersect_add_seg (ctx, in_seg);
              if (ctx->in_curs < in->n_segs)
                {
                  const ArtSVPSeg *next_seg = &in->segs[ctx->in_curs];
                  pri_point->x = next_seg->points[0].x;
                  pri_point->y = next_seg->points[0].y;
                  /* user_data is already NULL */
                  art_pri_insert (pq, pri_point);
                }
              else
                art_free(pri_point);pri_point =0;
          } else {
            art_free(pri_point);pri_point = 0;
          }
	}
      else
	{
          int n_stack = seg->n_stack;

          if (n_stack > 1)
            {
              art_svp_intersect_process_intersection (ctx, seg);
              art_free (pri_point);
            }
          else
            {
              art_svp_intersect_advance_cursor (ctx, seg, pri_point);
            }
	}
        lasty = ctx->y;
    }

  art_svp_intersect_horiz_commit (ctx);

  art_pri_free (pq);
  art_free (ctx);
  current_svp = 0;
}
