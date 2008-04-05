/*
This is a Optical-Character-Recognition program
Copyright (C) 2000-2006  Joerg Schulenburg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 see README for EMAIL address
*/

#include <string.h>
#include "unicode.h"
#include "output.h"
#include "gocr.h"  /* extern job_t JOB; */

/* function is only for debugging and for developing
   it prints out a part of pixmap b at point x0,y0 to stderr
   using dots .,; if no pixel, and @xoO for pixels
   modify n_run and print out what would happen on 2nd, 3th loop!
   new: output original and copied pixmap in the same figure
 */
void out_b(struct box *px, pix *b, int x0, int y0, int dx, int dy, int cs ){
  int x,y,x2,y2,yy0,tx,ty,n1,i;
  char c1, c2;
  yy0=y0;
  if(px){ /* overwrite rest of arguments */
    if (!b) {
      b=px->p;
      x0=px->x0; dx=px->x1-px->x0+1;
      y0=px->y0; dy=px->y1-px->y0+1; yy0=y0;
    }
    if(cs==0) cs=JOB->cfg.cs;
    fprintf(stderr,"\n# list box      x= %4d %4d d= %3d %3d r= %3d %3d"
                    " nrun=%d p=%p", /* ToDo: r,nrun is obsolete */
	  px->x0, px->y0, px->x1 - px->x0 + 1, px->y1 - px->y0 + 1,
	  px->x - px->x0, px->y - px->y0, JOB->tmp.n_run, (void*)px);
    fprintf(stderr,"\n#  dots=%d boxes=%d subboxes=%d c=%s mod=%s"
            " line=%d m= %d %d %d %d",
	  px->dots, px->num_boxes, px->num_subboxes, 
	  decode(px->c,ASCII), decode(px->modifier,ASCII), px->line,
	  px->m1 - px->y0, px->m2 - px->y0, px->m3 - px->y0, px->m4 - px->y0);
    if (px->num_frames) {
      int i,j,jo;
      fprintf(stderr,"\n#  frames= %d (sumvects=%d)",px->num_frames,
         ((px->num_frames)?px->num_frame_vectors[px->num_frames-1]:-1));
      for (jo=j=i=0; i<px->num_frames; i++, jo=j) {
        fprintf(stderr,"\n#  frame %d (%+4d,%3d,%2d) ",
                i, px->frame_vol[i], px->frame_per[i],
                   px->num_frame_vectors[i]-jo);
        /* print only the first vectors of each frame */
        for (;j<px->num_frame_vectors[i] && j<MaxFrameVectors; j++)
          fprintf(stderr," #%02d %2d %2d", j,
                            px->frame_vector[j][0] - px->x0,
                            px->frame_vector[j][1] - px->y0);
      }
    }
    if (px->num_ac){ /* output table of chars and its probabilities */
      fprintf(stderr,"\n# list box char: ");
      for(i=0;i<px->num_ac && i<NumAlt;i++)
      /* output the (xml-)string (picture position, barcodes, glyphs, ...) */
        if (px->tas[i])
         fprintf(stderr," %s(%d)",       px->tas[i]       ,px->wac[i]);
        else
         fprintf(stderr," %s(%d)",decode(px->tac[i],ASCII),px->wac[i]);
    }
    fprintf(stderr,"\n");
    if (px->dots && px->m2 && px->m1<y0) { yy0=px->m1; dy=px->y1-yy0+1; }
  }
  tx=dx/80+1;
  ty=dy/40+1; /* step, usually 1, but greater on large maps */
  fprintf(stderr,"# list pattern  x= %4d %4d d= %3d %3d t= %d %d\n",
                 x0,y0,dx,dy,tx,ty);
  if (dx>0)
  for(y=yy0;y<yy0+dy;y+=ty) { /* reduce the output to max 78x40 */
    /* first image is the copied and modified bitmap of the box */
    if (px)
    for(x=x0;x<x0+dx;x+=tx){  /* by merging sub-pixels */
      n1=0; c1='.';
      for(y2=y;y2<y+ty && y2<y0+dy;y2++) /* sub-pixels */
      for(x2=x;x2<x+tx && x2<x0+dx;x2++)
      {
        if((getpixel(px->p,x2-x0+px->x0,
                        y2-y0+px->y0)<cs)) c1='@';
      }
      if (px->num_frames) { /* mark vectors */
        int i;
        if (c1!='$' && c1!='S') /* dont mark twice */
        for (i=0;i<px->num_frame_vectors[px->num_frames-1];i++)
          if ((px->frame_vector[i][0]-px->x0)/tx==(x-x0)/tx
           && (px->frame_vector[i][1]-px->y0)/ty==(y-y0)/ty)
              { c1=((c1=='@')?'$':'S'); break; }
      }
      fprintf(stderr,"%c", c1 );
    }

    /* 2nd image is the boxframe in the original bitmap */
    if (dx<40) fprintf(stderr,"  ");
    if (dx<40) /* do it only, if we have enough place */
    for(x=x0;x<x0+dx;x+=tx){  /* by merging sub-pixels */
      c1='.';
      for(y2=y;y2<y+ty && y2<y0+dy;y2++) /* sub-pixels */
      for(x2=x;x2<x+tx && x2<x0+dx;x2++)
        { if((getpixel(b,x2,y2)<cs)) c1='@'; }
      fprintf(stderr,"%c", c1 );
    }

    c1=c2=' ';
    /* mark lines with < */
    if (px) if (y-y0+px->y0==px->m1 || y-y0+px->y0==px->m2
             || y-y0+px->y0==px->m3 || y-y0+px->y0==px->m4)  c1='<';
    if (y==y0 || y==yy0+dy-1)  c2='-';  /* boxmarks */
        
    fprintf(stderr,"%c%c\n",c1,c2);
  }
}

/* same as out_b, but for faster use, only a box as argument
 */
void out_x(struct box *px) {
  out_b(px,NULL,0, 0, 0, 0, JOB->cfg.cs);
}


/* print out two boxes side by side, for debugging comparision algos */
void out_x2(struct box *box1, struct box *box2){
  int x,y,i,tx,ty,dy;
  /*FIXME jb static*/static char *c1="OXXXXxx@.,,,,,,,";
  pix *b=&JOB->src.p;
  dy=(box1->y1-box1->y0+1);
  if(dy<box2->y1-box2->y0+1)dy=box2->y1-box2->y0+1;
  tx=(box1->x1-box1->x0)/40+1;
  ty=(box1->y1-box1->y0)/40+1; /* step, usually 1, but greater on large maps */
  if(box2)fprintf(stderr,"\n# list 2 patterns");
  for(i=0;i<dy;i+=ty) { /* reduce the output to max 78x40??? */
    fprintf(stderr,"\n"); y=box1->y0+i;
    for(x=box1->x0;x<=box1->x1;x+=tx) 
    fprintf(stderr,"%c", c1[ ((getpixel(b,x,y)<JOB->cfg.cs)?0:8)+marked(b,x,y) ] );
    if(!box2) continue;
    fprintf(stderr,"  "); y=box2->y0+i;
    for(x=box2->x0;x<=box2->x1;x+=tx)
    fprintf(stderr,"%c", c1[ ((getpixel(b,x,y)<JOB->cfg.cs)?0:8)+marked(b,x,y) ] );
  }
}


/* ---- list output ---- for debugging ---
 * list all boxes where the results can be found within the c-option  
 */
int output_list(job_t *job) {
  int i = 0, j;
  struct box *box2;
  pix  *pp = &job->src.p;
  char *lc = job->cfg.lc;

  fprintf(stderr,"\n# list shape for charlist %s",lc);
  for_each_data(&(JOB->res.boxlist)) {
    box2 = (struct box *) list_get_current(&(JOB->res.boxlist));
    for (j=0; j<box2->num_ac; j++) 
      if (!lc || (box2->tac[j] && strchr(lc, box2->tac[j]))
              || (box2->tas[j] && strstr(lc, box2->tas[j]))) break;
    if (j<box2->num_ac)
      fprintf(stderr,"\n#            box found in charlist");
    if (!lc || (strchr(lc, box2->c) && box2->c < 256 && box2->c)
            || (strchr(lc, '_') && box2->c==UNKNOWN) /* for compability */
            || j<box2->num_ac ){  /* also list alternative chars */
      if (!pp) pp=box2->p;
      fprintf(stderr,
	      "\n# list shape %3d x=%4d %4d d= %3d %3d vf=%d ac=%d %04x %s",
	      i, box2->x0, box2->y0,
	      box2->x1 - box2->x0 + 1,
	      box2->y1 - box2->y0 + 1,
	      box2->num_frames, box2->num_ac, 
	      (int)box2->c,   /* wchar_t -> char ???? */
	      decode(box2->c,ASCII) );
      if (JOB->cfg.verbose & 4) out_x(box2);
    }
    i++;
  } end_for_each(&(JOB->res.boxlist));
  fprintf(stderr,"\n");
  return 0;
}

