/*
 * see COPYRIGHT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#ifndef WIN32
#	include <netinet/in.h>
#	include <unistd.h>
#else
#	include "win_missing.h"
#endif

#include "ttf.h"
#include "pt1.h"
#include "global.h"

/* big and small values for comparisons */
#define FBIGVAL	(1e20)
#define FEPS	(100000./FBIGVAL)

int      stdhw, stdvw;	/* dominant stems widths */
int      stemsnaph[12], stemsnapv[12];	/* most typical stem width */

int      bluevalues[14];
int      nblues;
int      otherblues[10];
int      notherb;
int      bbox[4];	/* the FontBBox array */
double   italic_angle;

GLYPH   *glyph_list;
int    encoding[ENCTABSZ];	/* inverse of glyph[].char_no */
int    kerning_pairs = 0;

/* prototypes */
static int isign( int x);
static int fsign( double x);
static void fixcvdir( GENTRY * ge, int dir);
static void fixcvends( GENTRY * ge);
static int fgetcvdir( GENTRY * ge);
static int igetcvdir( GENTRY * ge);
static int fiszigzag( GENTRY *ge);
static int iiszigzag( GENTRY *ge);
static GENTRY * freethisge( GENTRY *ge);
static void addgeafter( GENTRY *oge, GENTRY *nge );
static GENTRY * newgentry( int flags);
static void debugstems( char *name, STEM * hstems, int nhs, STEM * vstems, int nvs);
static int addbluestems( STEM *s, int n);
static void sortstems( STEM * s, int n);
static int stemoverlap( STEM * s1, STEM * s2);
static int steminblue( STEM *s);
static void markbluestems( STEM *s, int nold);
static int joinmainstems( STEM * s, int nold, int useblues);
static void joinsubstems( STEM * s, short *pairs, int nold, int useblues);
static void fixendpath( GENTRY *ge);
static void fdelsmall( GLYPH *g, double minlen);
static double fcvarea( GENTRY *ge);
static int fckjoinedcv( GLYPH *g, double t, GENTRY *nge, 
	GENTRY *old1, GENTRY *old2, double k);
static double fcvval( GENTRY *ge, int axis, double t);
static double fclosegap( GENTRY *from, GENTRY *to, int axis,
	double gap, double *ret);

static int
isign(
     int x
)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

static int
fsign(
     double x
)
{
	if (x > 0.0)
		return 1;
	else if (x < 0.0)
		return -1;
	else
		return 0;
}

static GENTRY *
newgentry(
	int flags
)
{
	GENTRY         *ge;

	ge = calloc(1, sizeof(GENTRY));

	if (ge == 0) {
		fprintf(stderr, "***** Memory allocation error *****\n");
		exit(255);
	}
	ge->stemid = -1;
	ge->flags = flags;
	/* the rest is set to 0 by calloc() */
	return ge;
}

/*
 * Routines to print out Postscript functions with optimization
 */

void
rmoveto(
	int dx,
	int dy
)
{
	if (optimize && dx == 0)
		fprintf(pfa_file, "%d vmoveto\n", dy);
	else if (optimize && dy == 0)
		fprintf(pfa_file, "%d hmoveto\n", dx);
	else
		fprintf(pfa_file, "%d %d rmoveto\n", dx, dy);
}

void
rlineto(
	int dx,
	int dy
)
{
	if (optimize && dx == 0 && dy == 0)	/* for special pathologic
						 * case */
		return;
	else if (optimize && dx == 0)
		fprintf(pfa_file, "%d vlineto\n", dy);
	else if (optimize && dy == 0)
		fprintf(pfa_file, "%d hlineto\n", dx);
	else
		fprintf(pfa_file, "%d %d rlineto\n", dx, dy);
}

void
rrcurveto(
	  int dx1,
	  int dy1,
	  int dx2,
	  int dy2,
	  int dx3,
	  int dy3
)
{
	/* first two ifs are for crazy cases that occur surprisingly often */
	if (optimize && dx1 == 0 && dx2 == 0 && dx3 == 0)
		rlineto(0, dy1 + dy2 + dy3);
	else if (optimize && dy1 == 0 && dy2 == 0 && dy3 == 0)
		rlineto(dx1 + dx2 + dx3, 0);
	else if (optimize && dy1 == 0 && dx3 == 0)
		fprintf(pfa_file, "%d %d %d %d hvcurveto\n",
			dx1, dx2, dy2, dy3);
	else if (optimize && dx1 == 0 && dy3 == 0)
		fprintf(pfa_file, "%d %d %d %d vhcurveto\n",
			dy1, dx2, dy2, dx3);
	else
		fprintf(pfa_file, "%d %d %d %d %d %d rrcurveto\n",
			dx1, dy1, dx2, dy2, dx3, dy3);
}

void
closepath(void)
{
	fprintf(pfa_file, "closepath\n");
}

/*
 * Many of the path processing routines exist (or will exist) in
 * both floating-point and integer version. Fimally most of the
 * processing will go in floating point and the integer processing
 * will become legacy.
 * The names of floating routines start with f, names of integer 
 * routines start with i, and those old routines existing in one 
 * version only have no such prefix at all.
 */

/*
** Routine that checks integrity of the path, for debugging
*/

void
assertpath(
	   GENTRY * from,
	   char *file,
	   int line,
	   char *name
)
{
	GENTRY         *first, *pe, *ge;
	int	isfloat;

	if(from==0)
		return;
	isfloat = (from->flags & GEF_FLOAT);
	pe = from->prev;
	for (ge = from; ge != 0; pe = ge, ge = ge->next) {
		if( (ge->flags & GEF_FLOAT) ^ isfloat ) {
			fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
			fprintf(stderr, "float flag changes from %s to %s at 0x%p (type %c, prev type %c)\n",
				(isfloat ? "TRUE" : "FALSE"), (isfloat ? "FALSE" : "TRUE"), ge, ge->type, pe->type);
			abort();
		}
		if (pe != ge->prev) {
			fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
			fprintf(stderr, "unidirectional chain 0x%x -next-> 0x%x -prev-> 0x%x \n",
				pe, ge, ge->prev);
			abort();
		}

		switch(ge->type) {
		case GE_MOVE:
			break;
		case GE_PATH:
			if (ge->prev == 0) {
				fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
				fprintf(stderr, "empty path at 0x%x \n", ge);
				abort();
			}
			break;
		case GE_LINE:
		case GE_CURVE:
			if(ge->frwd->bkwd != ge) {
				fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
				fprintf(stderr, "unidirectional chain 0x%x -frwd-> 0x%x -bkwd-> 0x%x \n",
					ge, ge->frwd, ge->frwd->bkwd);
				abort();
			}
			if(ge->prev->type == GE_MOVE) {
				first = ge;
				if(ge->bkwd->next->type != GE_PATH) {
					fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
					fprintf(stderr, "broken first backlink 0x%x -bkwd-> 0x%x -next-> 0x%x \n",
						ge, ge->bkwd, ge->bkwd->next);
					abort();
				}
			}
			if(ge->next->type == GE_PATH) {
				if(ge->frwd != first) {
					fprintf(stderr, "**! assertpath: called from %s line %d (%s) ****\n", file, line, name);
					fprintf(stderr, "broken loop 0x%x -...-> 0x%x -frwd-> 0x%x \n",
						first, ge, ge->frwd);
					abort();
				}
			}
			break;
		}

	}
}

void
assertisfloat(
	GLYPH *g,
	char *msg
)
{
	if( !(g->flags & GF_FLOAT) ) {
		fprintf(stderr, "**! Glyph %s is not float: %s\n", g->name, msg);
		abort();
	}
	if(g->lastentry) {
		if( !(g->lastentry->flags & GEF_FLOAT) ) {
			fprintf(stderr, "**! Glyphs %s last entry is int: %s\n", g->name, msg);
			abort();
		}
	}
}

void
assertisint(
	GLYPH *g,
	char *msg
)
{
	if( (g->flags & GF_FLOAT) ) {
		fprintf(stderr, "**! Glyph %s is not int: %s\n", g->name, msg);
		abort();
	}
	if(g->lastentry) {
		if( (g->lastentry->flags & GEF_FLOAT) ) {
			fprintf(stderr, "**! Glyphs %s last entry is float: %s\n", g->name, msg);
			abort();
		}
	}
}


/*
 * Routines to save the generated data about glyph
 */

void
fg_rmoveto(
	  GLYPH * g,
	  double x,
	  double y)
{
	GENTRY         *oge;

	if (ISDBG(BUILDG))
		fprintf(stderr, "%s: f rmoveto(%g, %g)\n", g->name, x, y);

	assertisfloat(g, "adding float MOVE");

	if ((oge = g->lastentry) != 0) {
		if (oge->type == GE_MOVE) {	/* just eat up the first move */
			oge->fx3 = x;
			oge->fy3 = y;
		} else if (oge->type == GE_LINE || oge->type == GE_CURVE) {
			fprintf(stderr, "Glyph %s: MOVE in middle of path\n", g->name);
		} else {
			GENTRY         *nge;

			nge = newgentry(GEF_FLOAT);
			nge->type = GE_MOVE;
			nge->fx3 = x;
			nge->fy3 = y;

			oge->next = nge;
			nge->prev = oge;
			g->lastentry = nge;
		}
	} else {
		GENTRY         *nge;

		nge = newgentry(GEF_FLOAT);
		nge->type = GE_MOVE;
		nge->fx3 = x;
		nge->fy3 = y;
		nge->bkwd = (GENTRY*)&g->entries;
		g->entries = g->lastentry = nge;
	}

	if (0 && ISDBG(BUILDG))
		dumppaths(g, NULL, NULL);
}

void
fg_rlineto(
	  GLYPH * g,
	  double x,
	  double y)
{
	GENTRY         *oge, *nge;

	if (ISDBG(BUILDG))
		fprintf(stderr, "%s: f rlineto(%g, %g)\n", g->name, x, y);

	assertisfloat(g, "adding float LINE");

	nge = newgentry(GEF_FLOAT);
	nge->type = GE_LINE;
	nge->fx3 = x;
	nge->fy3 = y;

	if ((oge = g->lastentry) != 0) {
		if (x == oge->fx3 && y == oge->fy3) {	/* empty line */
			/* ignore it or we will get in troubles later */
			free(nge);
			return;
		}
		if (g->path == 0) {
			g->path = nge;
			nge->bkwd = nge->frwd = nge;
		} else {
			oge->frwd = nge;
			nge->bkwd = oge;
			g->path->bkwd = nge;
			nge->frwd = g->path;
		}

		oge->next = nge;
		nge->prev = oge;
		g->lastentry = nge;
	} else {
		WARNING_1 fprintf(stderr, "Glyph %s: LINE outside of path\n", g->name);
		free(nge);
	}

	if (0 && ISDBG(BUILDG))
		dumppaths(g, NULL, NULL);
}

void
fg_rrcurveto(
	    GLYPH * g,
	    double x1,
	    double y1,
	    double x2,
	    double y2,
	    double x3,
	    double y3)
{
	GENTRY         *oge, *nge;

	oge = g->lastentry;

	if (ISDBG(BUILDG))
		fprintf(stderr, "%s: f rrcurveto(%g, %g, %g, %g, %g, %g)\n"
			,g->name, x1, y1, x2, y2, x3, y3);

	assertisfloat(g, "adding float CURVE");

	if (oge && oge->fx3 == x1 && x1 == x2 && x2 == x3)	/* check if it's
								 * actually a line */
		fg_rlineto(g, x1, y3);
	else if (oge && oge->fy3 == y1 && y1 == y2 && y2 == y3)
		fg_rlineto(g, x3, y1);
	else {
		nge = newgentry(GEF_FLOAT);
		nge->type = GE_CURVE;
		nge->fx1 = x1;
		nge->fy1 = y1;
		nge->fx2 = x2;
		nge->fy2 = y2;
		nge->fx3 = x3;
		nge->fy3 = y3;

		if (oge != 0) {
			if (x3 == oge->fx3 && y3 == oge->fy3) {
				free(nge);	/* consider this curve empty */
				/* ignore it or we will get in troubles later */
				return;
			}
			if (g->path == 0) {
				g->path = nge;
				nge->bkwd = nge->frwd = nge;
			} else {
				oge->frwd = nge;
				nge->bkwd = oge;
				g->path->bkwd = nge;
				nge->frwd = g->path;
			}

			oge->next = nge;
			nge->prev = oge;
			g->lastentry = nge;
		} else {
			WARNING_1 fprintf(stderr, "Glyph %s: CURVE outside of path\n", g->name);
			free(nge);
		}
	}

	if (0 && ISDBG(BUILDG))
		dumppaths(g, NULL, NULL);
}

void
g_closepath(
	    GLYPH * g
)
{
	GENTRY         *oge, *nge;

	if (ISDBG(BUILDG))
		fprintf(stderr, "%s: closepath\n", g->name);

	oge = g->lastentry;

	if (g->path == 0) {
		WARNING_1 fprintf(stderr, "Warning: **** closepath on empty path in glyph \"%s\" ****\n",
			g->name);
		if (oge == 0) {
			WARNING_1 fprintf(stderr, "No previois entry\n");
		} else {
			WARNING_1 fprintf(stderr, "Previous entry type: %c\n", oge->type);
			if (oge->type == GE_MOVE) {
				g->lastentry = oge->prev;
				if (oge->prev == 0)
					g->entries = 0;
			}
		}
		return;
	}

	nge = newgentry(oge->flags & GEF_FLOAT); /* keep the same type */
	nge->type = GE_PATH;

	g->path = 0;

	oge->next = nge;
	nge->prev = oge;
	g->lastentry = nge;

	if (0 && ISDBG(BUILDG))
		dumppaths(g, NULL, NULL);
}

/*
 * * SB * Routines to smooth and fix the glyphs
 */

/*
** we don't want to see the curves with coinciding middle and
** outer points
*/

static void
fixcvends(
	  GENTRY * ge
)
{
	int             dx, dy;
	int             x0, y0, x1, y1, x2, y2, x3, y3;

	if (ge->type != GE_CURVE)
		return;

	if(ge->flags & GEF_FLOAT) {
		fprintf(stderr, "**! fixcvends(0x%x) on floating entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	x0 = ge->prev->ix3;
	y0 = ge->prev->iy3;
	x1 = ge->ix1;
	y1 = ge->iy1;
	x2 = ge->ix2;
	y2 = ge->iy2;
	x3 = ge->ix3;
	y3 = ge->iy3;


	/* look at the start of the curve */
	if (x1 == x0 && y1 == y0) {
		dx = x2 - x1;
		dy = y2 - y1;

		if (dx == 0 && dy == 0
		    || x2 == x3 && y2 == y3) {
			/* Oops, we actually have a straight line */
			/*
			 * if it's small, we hope that it will get optimized
			 * later
			 */
			if (abs(x3 - x0) <= 2 || abs(y3 - y0) <= 2) {
				ge->ix1 = x3;
				ge->iy1 = y3;
				ge->ix2 = x0;
				ge->iy2 = y0;
			} else {/* just make it a line */
				ge->type = GE_LINE;
			}
		} else {
			if (abs(dx) < 4 && abs(dy) < 4) {	/* consider it very
								 * small */
				ge->ix1 = x2;
				ge->iy1 = y2;
			} else if (abs(dx) < 8 && abs(dy) < 8) {	/* consider it small */
				ge->ix1 += dx / 2;
				ge->iy1 += dy / 2;
			} else {
				ge->ix1 += dx / 4;
				ge->iy1 += dy / 4;
			}
			/* make sure that it's still on the same side */
			if (abs(x3 - x0) * abs(dy) < abs(y3 - y0) * abs(dx)) {
				if (abs(x3 - x0) * abs(ge->iy1 - y0) > abs(y3 - y0) * abs(ge->ix1 - x0))
					ge->ix1 += isign(dx);
			} else {
				if (abs(x3 - x0) * abs(ge->iy1 - y0) < abs(y3 - y0) * abs(ge->ix1 - x0))
					ge->iy1 += isign(dy);
			}

			ge->ix2 += (x3 - x2) / 8;
			ge->iy2 += (y3 - y2) / 8;
			/* make sure that it's still on the same side */
			if (abs(x3 - x0) * abs(y3 - y2) < abs(y3 - y0) * abs(x3 - x2)) {
				if (abs(x3 - x0) * abs(y3 - ge->iy2) > abs(y3 - y0) * abs(x3 - ge->ix2))
					ge->iy1 -= isign(y3 - y2);
			} else {
				if (abs(x3 - x0) * abs(y3 - ge->iy2) < abs(y3 - y0) * abs(x3 - ge->ix2))
					ge->ix1 -= isign(x3 - x2);
			}

		}
	} else if (x2 == x3 && y2 == y3) {
		dx = x1 - x2;
		dy = y1 - y2;

		if (dx == 0 && dy == 0) {
			/* Oops, we actually have a straight line */
			/*
			 * if it's small, we hope that it will get optimized
			 * later
			 */
			if (abs(x3 - x0) <= 2 || abs(y3 - y0) <= 2) {
				ge->ix1 = x3;
				ge->iy1 = y3;
				ge->ix2 = x0;
				ge->iy2 = y0;
			} else {/* just make it a line */
				ge->type = GE_LINE;
			}
		} else {
			if (abs(dx) < 4 && abs(dy) < 4) {	/* consider it very
								 * small */
				ge->ix2 = x1;
				ge->iy2 = y1;
			} else if (abs(dx) < 8 && abs(dy) < 8) {	/* consider it small */
				ge->ix2 += dx / 2;
				ge->iy2 += dy / 2;
			} else {
				ge->ix2 += dx / 4;
				ge->iy2 += dy / 4;
			}
			/* make sure that it's still on the same side */
			if (abs(x3 - x0) * abs(dy) < abs(y3 - y0) * abs(dx)) {
				if (abs(x3 - x0) * abs(ge->iy2 - y3) > abs(y3 - y0) * abs(ge->ix2 - x3))
					ge->ix2 += isign(dx);
			} else {
				if (abs(x3 - x0) * abs(ge->iy2 - y3) < abs(y3 - y0) * abs(ge->ix2 - x3))
					ge->iy2 += isign(dy);
			}

			ge->ix1 += (x0 - x1) / 8;
			ge->iy1 += (y0 - y1) / 8;
			/* make sure that it's still on the same side */
			if (abs(x3 - x0) * abs(y0 - y1) < abs(y3 - y0) * abs(x0 - x1)) {
				if (abs(x3 - x0) * abs(y0 - ge->iy1) > abs(y3 - y0) * abs(x0 - ge->ix1))
					ge->iy1 -= isign(y0 - y1);
			} else {
				if (abs(x3 - x0) * abs(y0 - ge->iy1) < abs(y3 - y0) * abs(x0 - ge->ix1))
					ge->ix1 -= isign(x0 - x1);
			}

		}
	}
}

/* if we have any curves that are in fact flat but
** are not horizontal nor vertical, substitute
** them also with lines
*/

void
flattencurves(
	      GLYPH * g
)
{
	GENTRY         *ge;
	int             x0, y0, x1, y1, x2, y2, x3, y3;

	assertisint(g, "flattencurves INT");

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type != GE_CURVE)
			continue;

		x0 = ge->prev->ix3;
		y0 = ge->prev->iy3;
		x1 = ge->ix1;
		y1 = ge->iy1;
		x2 = ge->ix2;
		y2 = ge->iy2;
		x3 = ge->ix3;
		y3 = ge->iy3;

		if ((x1 - x0) * (y2 - y1) == (x2 - x1) * (y1 - y0)
		    && (x1 - x0) * (y3 - y2) == (x3 - x2) * (y1 - y0)) {
			ge->type = GE_LINE;
		}
	}
}

/*
** After transformations we want to make sure that the resulting
** curve is going in the same quadrant as the original one,
** because rounding errors introduced during transformations
** may make the result completeley wrong.
**
** `dir' argument describes the direction of the original curve,
** it is the superposition of two values for the front and
** rear ends of curve:
**
** >EQUAL - goes over the line connecting the ends
** =EQUAL - coincides with the line connecting the ends
** <EQUAL - goes under the line connecting the ends
**
** See CVDIR_* for exact definitions.
*/

static void
fixcvdir(
	 GENTRY * ge,
	 int dir
)
{
	int             a, b, c, d;
	double          kk, kk1, kk2;
	int             changed;
	int             fdir, rdir;

	if(ge->flags & GEF_FLOAT) {
		fprintf(stderr, "**! fixcvdir(0x%x) on floating entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	fdir = (dir & CVDIR_FRONT) - CVDIR_FEQUAL;
	if ((dir & CVDIR_REAR) == CVDIR_RSAME)
		rdir = fdir; /* we need only isign, exact value doesn't matter */
	else
		rdir = (dir & CVDIR_REAR) - CVDIR_REQUAL;

	fixcvends(ge);

	c = isign(ge->ix3 - ge->prev->ix3);	/* note the direction of
						 * curve */
	d = isign(ge->iy3 - ge->prev->iy3);

	a = ge->iy3 - ge->prev->iy3;
	b = ge->ix3 - ge->prev->ix3;
	kk = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
	a = ge->iy1 - ge->prev->iy3;
	b = ge->ix1 - ge->prev->ix3;
	kk1 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
	a = ge->iy3 - ge->iy2;
	b = ge->ix3 - ge->ix2;
	kk2 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));

	changed = 1;
	while (changed) {
		if (ISDBG(FIXCVDIR)) {
			/* for debugging */
			fprintf(stderr, "fixcvdir %d %d (%d %d %d %d %d %d) %f %f %f\n",
				fdir, rdir,
				ge->ix1 - ge->prev->ix3,
				ge->iy1 - ge->prev->iy3,
				ge->ix2 - ge->ix1,
				ge->iy2 - ge->iy1,
				ge->ix3 - ge->ix2,
				ge->iy3 - ge->iy2,
				kk1, kk, kk2);
		}
		changed = 0;

		if (fdir > 0) {
			if (kk1 > kk) {	/* the front end has problems */
				if (c * (ge->ix1 - ge->prev->ix3) > 0) {
					ge->ix1 -= c;
					changed = 1;
				} if (d * (ge->iy2 - ge->iy1) > 0) {
					ge->iy1 += d;
					changed = 1;
				}
				/* recalculate the coefficients */
				a = ge->iy3 - ge->prev->iy3;
				b = ge->ix3 - ge->prev->ix3;
				kk = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
				a = ge->iy1 - ge->prev->iy3;
				b = ge->ix1 - ge->prev->ix3;
				kk1 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
			}
		} else if (fdir < 0) {
			if (kk1 < kk) {	/* the front end has problems */
				if (c * (ge->ix2 - ge->ix1) > 0) {
					ge->ix1 += c;
					changed = 1;
				} if (d * (ge->iy1 - ge->prev->iy3) > 0) {
					ge->iy1 -= d;
					changed = 1;
				}
				/* recalculate the coefficients */
				a = ge->iy1 - ge->prev->iy3;
				b = ge->ix1 - ge->prev->ix3;
				kk1 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
				a = ge->iy3 - ge->prev->iy3;
				b = ge->ix3 - ge->prev->ix3;
				kk = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
			}
		}
		if (rdir > 0) {
			if (kk2 < kk) {	/* the rear end has problems */
				if (c * (ge->ix2 - ge->ix1) > 0) {
					ge->ix2 -= c;
					changed = 1;
				} if (d * (ge->iy3 - ge->iy2) > 0) {
					ge->iy2 += d;
					changed = 1;
				}
				/* recalculate the coefficients */
				a = ge->iy3 - ge->prev->iy3;
				b = ge->ix3 - ge->prev->ix3;
				kk = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
				a = ge->iy3 - ge->iy2;
				b = ge->ix3 - ge->ix2;
				kk2 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
			}
		} else if (rdir < 0) {
			if (kk2 > kk) {	/* the rear end has problems */
				if (c * (ge->ix3 - ge->ix2) > 0) {
					ge->ix2 += c;
					changed = 1;
				} if (d * (ge->iy2 - ge->iy1) > 0) {
					ge->iy2 -= d;
					changed = 1;
				}
				/* recalculate the coefficients */
				a = ge->iy3 - ge->prev->iy3;
				b = ge->ix3 - ge->prev->ix3;
				kk = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
				a = ge->iy3 - ge->iy2;
				b = ge->ix3 - ge->ix2;
				kk2 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
			}
		}
	}
	fixcvends(ge);
}

/* Get the directions of ends of curve for further usage */

/* expects that the previous element is also float */

static int
fgetcvdir(
	 GENTRY * ge
)
{
	double          a, b;
	double          k, k1, k2;
	int             dir = 0;

	if( !(ge->flags & GEF_FLOAT) ) {
		fprintf(stderr, "**! fgetcvdir(0x%x) on int entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	a = ge->fy3 - ge->prev->fy3;
	b = ge->fx3 - ge->prev->fx3;
	k = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ( b / a));
	a = ge->fy1 - ge->prev->fy3;
	b = ge->fx1 - ge->prev->fx3;
	k1 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ( b / a));
	a = ge->fy3 - ge->fy2;
	b = ge->fx3 - ge->fx2;
	k2 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ( b / a));

	if (k1 < k)
		dir |= CVDIR_FUP;
	else if (k1 > k)
		dir |= CVDIR_FDOWN;
	else
		dir |= CVDIR_FEQUAL;

	if (k2 > k)
		dir |= CVDIR_RUP;
	else if (k2 < k)
		dir |= CVDIR_RDOWN;
	else
		dir |= CVDIR_REQUAL;

	return dir;
}


/* expects that the previous element is also int */

static int
igetcvdir(
	 GENTRY * ge
)
{
	int             a, b;
	double          k, k1, k2;
	int             dir = 0;

	if(ge->flags & GEF_FLOAT) {
		fprintf(stderr, "**! igetcvdir(0x%x) on floating entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	a = ge->iy3 - ge->prev->iy3;
	b = ge->ix3 - ge->prev->ix3;
	k = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
	a = ge->iy1 - ge->prev->iy3;
	b = ge->ix1 - ge->prev->ix3;
	k1 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));
	a = ge->iy3 - ge->iy2;
	b = ge->ix3 - ge->ix2;
	k2 = fabs(a == 0 ? (b == 0 ? 1. : 100000.) : ((double) b / (double) a));

	if (k1 < k)
		dir |= CVDIR_FUP;
	else if (k1 > k)
		dir |= CVDIR_FDOWN;
	else
		dir |= CVDIR_FEQUAL;

	if (k2 > k)
		dir |= CVDIR_RUP;
	else if (k2 < k)
		dir |= CVDIR_RDOWN;
	else
		dir |= CVDIR_REQUAL;

	return dir;
}

#if 0
/* a function just to test the work of fixcvdir() */
static void
testfixcvdir(
	     GLYPH * g
)
{
	GENTRY         *ge;
	int             dir;

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type == GE_CURVE) {
			dir = igetcvdir(ge);
			fixcvdir(ge, dir);
		}
	}
}
#endif

static int
iround(
	double val
)
{
	return (int) (val > 0 ? val + 0.5 : val - 0.5);
}
	
/* for debugging - dump the glyph
 * mark with a star the entries from start to end inclusive
 * (start == NULL means don't mark any, end == NULL means to the last)
 */

void
dumppaths(
	GLYPH *g,
	GENTRY *start,
	GENTRY *end
)
{
	GENTRY *ge;
	int i;
	char mark=' ';

	fprintf(stderr, "Glyph %s:\n", g->name);

	/* now do the conversion */
	for(ge = g->entries; ge != 0; ge = ge->next) {
		if(ge == start)
			mark = '*';
		fprintf(stderr, " %c %8x", mark, ge);
		switch(ge->type) {
		case GE_MOVE:
		case GE_LINE:
			if(ge->flags & GEF_FLOAT)
				fprintf(stderr," %c float (%g, %g)\n", ge->type, ge->fx3, ge->fy3);
			else
				fprintf(stderr," %c int (%d, %d)\n", ge->type, ge->ix3, ge->iy3);
			break;
		case GE_CURVE:
			if(ge->flags & GEF_FLOAT) {
				fprintf(stderr," C float ");
				for(i=0; i<3; i++)
					fprintf(stderr,"(%g, %g) ", ge->fxn[i], ge->fyn[i]);
				fprintf(stderr,"\n");
			} else {
				fprintf(stderr," C int ");
				for(i=0; i<3; i++)
					fprintf(stderr,"(%d, %d) ", ge->ixn[i], ge->iyn[i]);
				fprintf(stderr,"\n");
			}
			break;
		default:
			fprintf(stderr, " %c\n", ge->type);
			break;
		}
		if(ge == end)
			mark = ' ';
	}
}

/*
 * Routine that converts all entries in the path from float to int
 */

void
pathtoint(
	GLYPH *g
)
{
	GENTRY *ge;
	int x[3], y[3];
	int i;


	if(ISDBG(TOINT))
		fprintf(stderr, "TOINT: glyph %s\n", g->name);
	assertisfloat(g, "converting path to int\n");

	fdelsmall(g, 1.0); /* get rid of sub-pixel contours */
	assertpath(g->entries, __FILE__, __LINE__, g->name);

	/* 1st pass, collect the directions of the curves: have
	 * to do that in advance, while everyting is float
	 */
	for(ge = g->entries; ge != 0; ge = ge->next) {
		if( !(ge->flags & GEF_FLOAT) ) {
			fprintf(stderr, "**! glyphs %s has int entry, found in conversion to int\n",
				g->name);
			exit(1);
		}
		if(ge->type == GE_CURVE) {
			ge->dir = fgetcvdir(ge);
		}
	}

	/* now do the conversion */
	for(ge = g->entries; ge != 0; ge = ge->next) {
		switch(ge->type) {
		case GE_MOVE:
		case GE_LINE:
			if(ISDBG(TOINT))
				fprintf(stderr," %c float x=%g y=%g\n", ge->type, ge->fx3, ge->fy3);
			x[0] = iround(ge->fx3);
			y[0] = iround(ge->fy3);
			for(i=0; i<3; i++) { /* put some valid values everywhere, for convenience */
				ge->ixn[i] = x[0];
				ge->iyn[i] = y[0];
			}
			if(ISDBG(TOINT))
				fprintf(stderr,"   int   x=%d y=%d\n", ge->ix3, ge->iy3);
			break;
		case GE_CURVE:
			if(ISDBG(TOINT))
				fprintf(stderr," %c float ", ge->type);

			for(i=0; i<3; i++) {
				if(ISDBG(TOINT))
					fprintf(stderr,"(%g, %g) ", ge->fxn[i], ge->fyn[i]);
				x[i] = iround(ge->fxn[i]);
				y[i] = iround(ge->fyn[i]);
			}

			if(ISDBG(TOINT))
				fprintf(stderr,"\n   int   ");

			for(i=0; i<3; i++) {
				ge->ixn[i] = x[i];
				ge->iyn[i] = y[i];
				if(ISDBG(TOINT))
					fprintf(stderr,"(%d, %d) ", ge->ixn[i], ge->iyn[i]);
			}
			ge->flags &= ~GEF_FLOAT; /* for fixcvdir */
			fixcvdir(ge, ge->dir);

			if(ISDBG(TOINT)) {
				fprintf(stderr,"\n   fixed ");
				for(i=0; i<3; i++)
					fprintf(stderr,"(%d, %d) ", ge->ixn[i], ge->iyn[i]);
				fprintf(stderr,"\n");
			}

			break;
		}
		ge->flags &= ~GEF_FLOAT;
	}
	g->flags &= ~GF_FLOAT;
}


/* check whether we can fix up the curve to change its size by (dx,dy) */
/* 0 means NO, 1 means YES */

/* for float: if scaling would be under 10% */

int
fcheckcv(
	GENTRY * ge,
	double dx,
	double dy
)
{
	if( !(ge->flags & GEF_FLOAT) ) {
		fprintf(stderr, "**! fcheckcv(0x%x) on int entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	if (ge->type != GE_CURVE)
		return 0;

	if( fabs(ge->fx3 - ge->prev->fx3) < fabs(dx) * 10 )
		return 0;

	if( fabs(ge->fy3 - ge->prev->fy3) < fabs(dy) * 10 )
		return 0;

	return 1;
}

/* for int: if won't create new zigzags at the ends */

int
icheckcv(
	GENTRY * ge,
	int dx,
	int dy
)
{
	int             xdep, ydep;

	if(ge->flags & GEF_FLOAT) {
		fprintf(stderr, "**! icheckcv(0x%x) on floating entry, ABORT\n", ge);
		abort(); /* dump core */
	}

	if (ge->type != GE_CURVE)
		return 0;

	xdep = ge->ix3 - ge->prev->ix3;
	ydep = ge->iy3 - ge->prev->iy3;

	if (ge->type == GE_CURVE
	    && (xdep * (xdep + dx)) > 0
	    && (ydep * (ydep + dy)) > 0) {
		return 1;
	} else
		return 0;
}

/* float connect the ends of open contours */

void
fclosepaths(
	   GLYPH * g
)
{
	GENTRY         *ge, *fge, *xge, *nge;
	int             i;

	assertisfloat(g, "fclosepaths float\n");

	for (xge = g->entries; xge != 0; xge = xge->next) {
		if( xge->type != GE_PATH )
			continue;

		ge = xge->prev;
		if(ge == 0 || ge->type != GE_LINE && ge->type!= GE_CURVE) {
			fprintf(stderr, "**! Glyph %s got empty path\n",
				g->name);
			exit(1);
		}

		fge = ge->frwd;
		if (fge->prev == 0 || fge->prev->type != GE_MOVE) {
			fprintf(stderr, "**! Glyph %s got strange beginning of path\n",
				g->name);
			exit(1);
		}
		fge = fge->prev;
		if (fge->fx3 != ge->fx3 || fge->fy3 != ge->fy3) {
			/* we have to fix this open path */

			WARNING_4 fprintf(stderr, "Glyph %s got path open by dx=%g dy=%g\n",
			g->name, fge->fx3 - ge->fx3, fge->fy3 - ge->fy3);


			/* add a new line */
			nge = newgentry(GEF_FLOAT);
			(*nge) = (*ge);
			nge->fx3 = fge->fx3;
			nge->fy3 = fge->fy3;
			nge->type = GE_LINE;

			addgeafter(ge, nge);

			if (fabs(ge->fx3 - fge->fx3) <= 2 && fabs(ge->fy3 - fge->fy3) <= 2) {
				/*
				 * small change, try to get rid of the new entry
				 */

				double df[2];

				for(i=0; i<2; i++) {
					df[i] = ge->fpoints[i][2] - fge->fpoints[i][2];
					df[i] = fclosegap(nge, nge, i, df[i], NULL);
				}

				if(df[0] == 0. && df[1] == 0.) {
					/* closed gap successfully, remove the added entry */
					freethisge(nge);
				}
			}
		}
	}
}

void
smoothjoints(
	     GLYPH * g
)
{
	GENTRY         *ge, *ne;
	int             dx1, dy1, dx2, dy2, k;
	int             dir;

	return; /* this stuff seems to create problems */

	assertisint(g, "smoothjoints int");

	if (g->entries == 0)	/* nothing to do */
		return;

	for (ge = g->entries->next; ge != 0; ge = ge->next) {
		ne = ge->frwd;

		/*
		 * although there should be no one-line path * and any path
		 * must end with CLOSEPATH, * nobody can say for sure
		 */

		if (ge == ne || ne == 0)
			continue;

		/* now handle various joints */

		if (ge->type == GE_LINE && ne->type == GE_LINE) {
			dx1 = ge->ix3 - ge->prev->ix3;
			dy1 = ge->iy3 - ge->prev->iy3;
			dx2 = ne->ix3 - ge->ix3;
			dy2 = ne->iy3 - ge->iy3;

			/* check whether they have the same direction */
			/* and the same slope */
			/* then we can join them into one line */

			if (dx1 * dx2 >= 0 && dy1 * dy2 >= 0 && dx1 * dy2 == dy1 * dx2) {
				/* extend the previous line */
				ge->ix3 = ne->ix3;
				ge->iy3 = ne->iy3;

				/* and get rid of the next line */
				freethisge(ne);
			}
		} else if (ge->type == GE_LINE && ne->type == GE_CURVE) {
			fixcvends(ne);

			dx1 = ge->ix3 - ge->prev->ix3;
			dy1 = ge->iy3 - ge->prev->iy3;
			dx2 = ne->ix1 - ge->ix3;
			dy2 = ne->iy1 - ge->iy3;

			/* if the line is nearly horizontal and we can fix it */
			if (dx1 != 0 && 5 * abs(dy1) / abs(dx1) == 0
			    && icheckcv(ne, 0, -dy1)
			    && abs(dy1) <= 4) {
				dir = igetcvdir(ne);
				ge->iy3 -= dy1;
				ne->iy1 -= dy1;
				fixcvdir(ne, dir);
				if (ge->next != ne)
					ne->prev->iy3 -= dy1;
				dy1 = 0;
			} else if (dy1 != 0 && 5 * abs(dx1) / abs(dy1) == 0
				   && icheckcv(ne, -dx1, 0)
				   && abs(dx1) <= 4) {
				/* the same but vertical */
				dir = igetcvdir(ne);
				ge->ix3 -= dx1;
				ne->ix1 -= dx1;
				fixcvdir(ne, dir);
				if (ge->next != ne)
					ne->prev->ix3 -= dx1;
				dx1 = 0;
			}
			/*
			 * if line is horizontal and curve begins nearly
			 * horizontally
			 */
			if (dy1 == 0 && dx2 != 0 && 5 * abs(dy2) / abs(dx2) == 0) {
				dir = igetcvdir(ne);
				ne->iy1 -= dy2;
				fixcvdir(ne, dir);
				dy2 = 0;
			} else if (dx1 == 0 && dy2 != 0 && 5 * abs(dx2) / abs(dy2) == 0) {
				/* the same but vertical */
				dir = igetcvdir(ne);
				ne->ix1 -= dx2;
				fixcvdir(ne, dir);
				dx2 = 0;
			}
		} else if (ge->type == GE_CURVE && ne->type == GE_LINE) {
			fixcvends(ge);

			dx1 = ge->ix3 - ge->ix2;
			dy1 = ge->iy3 - ge->iy2;
			dx2 = ne->ix3 - ge->ix3;
			dy2 = ne->iy3 - ge->iy3;

			/* if the line is nearly horizontal and we can fix it */
			if (dx2 != 0 && 5 * abs(dy2) / abs(dx2) == 0
			    && icheckcv(ge, 0, dy2)
			    && abs(dy2) <= 4) {
				dir = igetcvdir(ge);
				ge->iy3 += dy2;
				ge->iy2 += dy2;
				fixcvdir(ge, dir);
				if (ge->next != ne)
					ne->prev->iy3 += dy2;
				dy2 = 0;
			} else if (dy2 != 0 && 5 * abs(dx2) / abs(dy2) == 0
				   && icheckcv(ge, dx2, 0)
				   && abs(dx2) <= 4) {
				/* the same but vertical */
				dir = igetcvdir(ge);
				ge->ix3 += dx2;
				ge->ix2 += dx2;
				fixcvdir(ge, dir);
				if (ge->next != ne)
					ne->prev->ix3 += dx2;
				dx2 = 0;
			}
			/*
			 * if line is horizontal and curve ends nearly
			 * horizontally
			 */
			if (dy2 == 0 && dx1 != 0 && 5 * abs(dy1) / abs(dx1) == 0) {
				dir = igetcvdir(ge);
				ge->iy2 += dy1;
				fixcvdir(ge, dir);
				dy1 = 0;
			} else if (dx2 == 0 && dy1 != 0 && 5 * abs(dx1) / abs(dy1) == 0) {
				/* the same but vertical */
				dir = igetcvdir(ge);
				ge->ix2 += dx1;
				fixcvdir(ge, dir);
				dx1 = 0;
			}
		} else if (ge->type == GE_CURVE && ne->type == GE_CURVE) {
			fixcvends(ge);
			fixcvends(ne);

			dx1 = ge->ix3 - ge->ix2;
			dy1 = ge->iy3 - ge->iy2;
			dx2 = ne->ix1 - ge->ix3;
			dy2 = ne->iy1 - ge->iy3;

			/*
			 * check if we have a rather smooth joint at extremal
			 * point
			 */
			/* left or right extremal point */
			if (abs(dx1) <= 4 && abs(dx2) <= 4
			    && dy1 != 0 && 5 * abs(dx1) / abs(dy1) == 0
			    && dy2 != 0 && 5 * abs(dx2) / abs(dy2) == 0
			    && (ge->iy3 < ge->prev->iy3 && ne->iy3 < ge->iy3
				|| ge->iy3 > ge->prev->iy3 && ne->iy3 > ge->iy3)
			  && (ge->ix3 - ge->prev->ix3) * (ne->ix3 - ge->ix3) < 0
				) {
				dir = igetcvdir(ge);
				ge->ix2 += dx1;
				dx1 = 0;
				fixcvdir(ge, dir);
				dir = igetcvdir(ne);
				ne->ix1 -= dx2;
				dx2 = 0;
				fixcvdir(ne, dir);
			}
			/* top or down extremal point */
			else if (abs(dy1) <= 4 && abs(dy2) <= 4
				 && dx1 != 0 && 5 * abs(dy1) / abs(dx1) == 0
				 && dx2 != 0 && 5 * abs(dy2) / abs(dx2) == 0
				 && (ge->ix3 < ge->prev->ix3 && ne->ix3 < ge->ix3
				|| ge->ix3 > ge->prev->ix3 && ne->ix3 > ge->ix3)
				 && (ge->iy3 - ge->prev->iy3) * (ne->iy3 - ge->iy3) < 0
				) {
				dir = igetcvdir(ge);
				ge->iy2 += dy1;
				dy1 = 0;
				fixcvdir(ge, dir);
				dir = igetcvdir(ne);
				ne->iy1 -= dy2;
				dy2 = 0;
				fixcvdir(ne, dir);
			}
			/* or may be we just have a smooth junction */
			else if (dx1 * dx2 >= 0 && dy1 * dy2 >= 0
				 && 10 * abs(k = abs(dx1 * dy2) - abs(dy1 * dx2)) < (abs(dx1 * dy2) + abs(dy1 * dx2))) {
				int             tries[6][4];
				int             results[6];
				int             i, b;

				/* build array of changes we are going to try */
				/* uninitalized entries are 0 */
				if (k > 0) {
					static int      t1[6][4] = {
						{0, 0, 0, 0},
						{-1, 0, 1, 0},
						{-1, 0, 0, 1},
						{0, -1, 1, 0},
						{0, -1, 0, 1},
					{-1, -1, 1, 1}};
					memcpy(tries, t1, sizeof tries);
				} else {
					static int      t1[6][4] = {
						{0, 0, 0, 0},
						{1, 0, -1, 0},
						{1, 0, 0, -1},
						{0, 1, -1, 0},
						{0, 1, 0, -1},
					{1, 1, -1, -1}};
					memcpy(tries, t1, sizeof tries);
				}

				/* now try the changes */
				results[0] = abs(k);
				for (i = 1; i < 6; i++) {
					results[i] = abs((abs(dx1) + tries[i][0]) * (abs(dy2) + tries[i][1]) -
							 (abs(dy1) + tries[i][2]) * (abs(dx2) + tries[i][3]));
				}

				/* and find the best try */
				k = abs(k);
				b = 0;
				for (i = 1; i < 6; i++)
					if (results[i] < k) {
						k = results[i];
						b = i;
					}
				/* and finally apply it */
				if (dx1 < 0)
					tries[b][0] = -tries[b][0];
				if (dy2 < 0)
					tries[b][1] = -tries[b][1];
				if (dy1 < 0)
					tries[b][2] = -tries[b][2];
				if (dx2 < 0)
					tries[b][3] = -tries[b][3];

				dir = igetcvdir(ge);
				ge->ix2 -= tries[b][0];
				ge->iy2 -= tries[b][2];
				fixcvdir(ge, dir);
				dir = igetcvdir(ne);
				ne->ix1 += tries[b][3];
				ne->iy1 += tries[b][1];
				fixcvdir(ne, dir);
			}
		}
	}
}

/* debugging: print out stems of a glyph */
static void
debugstems(
	   char *name,
	   STEM * hstems,
	   int nhs,
	   STEM * vstems,
	   int nvs
)
{
	int             i;

	fprintf(pfa_file, "%% %s\n", name);
	fprintf(pfa_file, "%% %d horizontal stems:\n", nhs);
	for (i = 0; i < nhs; i++)
		fprintf(pfa_file, "%% %3d    %d (%d...%d) %c %c%c%c%c\n", i, hstems[i].value,
			hstems[i].from, hstems[i].to,
			((hstems[i].flags & ST_UP) ? 'U' : 'D'),
			((hstems[i].flags & ST_END) ? 'E' : '-'),
			((hstems[i].flags & ST_FLAT) ? 'F' : '-'),
			((hstems[i].flags & ST_ZONE) ? 'Z' : ' '),
			((hstems[i].flags & ST_TOPZONE) ? 'T' : ' '));
	fprintf(pfa_file, "%% %d vertical stems:\n", nvs);
	for (i = 0; i < nvs; i++)
		fprintf(pfa_file, "%% %3d    %d (%d...%d) %c %c%c\n", i, vstems[i].value,
			vstems[i].from, vstems[i].to,
			((vstems[i].flags & ST_UP) ? 'U' : 'D'),
			((vstems[i].flags & ST_END) ? 'E' : '-'),
			((vstems[i].flags & ST_FLAT) ? 'F' : '-'));
}

/* add pseudo-stems for the limits of the Blue zones to the stem array */
static int
addbluestems(
	STEM *s,
	int n
)
{
	int i;

	for(i=0; i<nblues && i<2; i+=2) { /* baseline */
		s[n].value=bluevalues[i];
		s[n].flags=ST_UP|ST_ZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i;
		n++;
		s[n].value=bluevalues[i+1];
		s[n].flags=ST_ZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i+1;
		n++;
	}
	for(i=2; i<nblues; i+=2) { /* top zones */
		s[n].value=bluevalues[i];
		s[n].flags=ST_UP|ST_ZONE|ST_TOPZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i;
		n++;
		s[n].value=bluevalues[i+1];
		s[n].flags=ST_ZONE|ST_TOPZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i+1;
		n++;
	}
	for(i=0; i<notherb; i+=2) { /* bottom zones */
		s[n].value=otherblues[i];
		s[n].flags=ST_UP|ST_ZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i+nblues;
		n++;
		s[n].value=otherblues[i+1];
		s[n].flags=ST_ZONE;
		/* don't overlap with anything */
		s[n].origin=s[n].from=s[n].to= -10000+i+1+nblues;
		n++;
	}
	return n;
}

/* sort stems in array */
static void
sortstems(
	  STEM * s,
	  int n
)
{
	int             i, j;
	STEM            x;


	/* a simple sorting */
	/* hm, the ordering criteria are not quite simple :-) 
	 * if the values are tied
	 * ST_UP always goes under not ST_UP
	 * ST_ZONE goes on the most outer side
	 * ST_END goes towards inner side after ST_ZONE
	 * ST_FLAT goes on the inner side
	 */

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			if(s[i].value < s[j].value)
				continue;
			if(s[i].value == s[j].value) {
				if( (s[i].flags & ST_UP) < (s[j].flags & ST_UP) )
					continue;
				if( (s[i].flags & ST_UP) == (s[j].flags & ST_UP) ) {
					if( s[i].flags & ST_UP ) {
						if(
						(s[i].flags & (ST_ZONE|ST_FLAT|ST_END) ^ ST_FLAT)
							>
						(s[j].flags & (ST_ZONE|ST_FLAT|ST_END) ^ ST_FLAT)
						)
							continue;
					} else {
						if(
						(s[i].flags & (ST_ZONE|ST_FLAT|ST_END) ^ ST_FLAT)
							<
						(s[j].flags & (ST_ZONE|ST_FLAT|ST_END) ^ ST_FLAT)
						)
							continue;
					}
				}
			}
			x = s[j];
			s[j] = s[i];
			s[i] = x;
		}
}

/* check whether two stem borders overlap */

static int
stemoverlap(
	    STEM * s1,
	    STEM * s2
)
{
	int             result;

	if (s1->from <= s2->from && s1->to >= s2->from
	    || s2->from <= s1->from && s2->to >= s1->from)
		result = 1;
	else
		result = 0;

	if (ISDBG(STEMOVERLAP))
		fprintf(pfa_file, "%% overlap %d(%d..%d)x%d(%d..%d)=%d\n",
			s1->value, s1->from, s1->to, s2->value, s2->from, s2->to, result);
	return result;
}

/* 
 * check if the stem [border] is in an appropriate blue zone
 * (currently not used)
 */

static int
steminblue(
	STEM *s
)
{
	int i, val;

	val=s->value;
	if(s->flags & ST_UP) {
		/* painted size up, look at lower zones */
		if(nblues>=2 && val>=bluevalues[0] && val<=bluevalues[1] )
			return 1;
		for(i=0; i<notherb; i++) {
			if( val>=otherblues[i] && val<=otherblues[i+1] )
				return 1;
		}
	} else {
		/* painted side down, look at upper zones */
		for(i=2; i<nblues; i++) {
			if( val>=bluevalues[i] && val<=bluevalues[i+1] )
				return 1;
		}
	}

	return 0;
}

/* mark the outermost stem [borders] in the blue zones */

static void
markbluestems(
	STEM *s,
	int nold
)
{
	int i, j, a, b, c;
	/*
	 * traverse the list of Blue Values, mark the lowest upper
	 * stem in each bottom zone and the topmost lower stem in
	 * each top zone with ST_BLUE
	 */

	/* top zones */
	for(i=2; i<nblues; i+=2) {
		a=bluevalues[i]; b=bluevalues[i+1];
		if(ISDBG(BLUESTEMS))
			fprintf(pfa_file, "%% looking at blue zone %d...%d\n", a, b);
		for(j=nold-1; j>=0; j--) {
			if( s[j].flags & (ST_ZONE|ST_UP|ST_END) )
				continue;
			c=s[j].value;
			if(c<a) /* too low */
				break;
			if(c<=b) { /* found the topmost stem border */
				/* mark all the stems with the same value */
				if(ISDBG(BLUESTEMS))
					fprintf(pfa_file, "%% found D BLUE at %d\n", s[j].value);
				/* include ST_END values */
				while( s[j+1].value==c && (s[j+1].flags & ST_ZONE)==0 )
					j++;
				s[j].flags |= ST_BLUE;
				for(j--; j>=0 && s[j].value==c 
						&& (s[j].flags & (ST_UP|ST_ZONE))==0 ; j--)
					s[j].flags |= ST_BLUE;
				break;
			}
		}
	}
	/* baseline */
	if(nblues>=2) {
		a=bluevalues[0]; b=bluevalues[1];
		for(j=0; j<nold; j++) {
			if( (s[j].flags & (ST_ZONE|ST_UP|ST_END))!=ST_UP )
				continue;
			c=s[j].value;
			if(c>b) /* too high */
				break;
			if(c>=a) { /* found the lowest stem border */
				/* mark all the stems with the same value */
				if(ISDBG(BLUESTEMS))
					fprintf(pfa_file, "%% found U BLUE at %d\n", s[j].value);
				/* include ST_END values */
				while( s[j-1].value==c && (s[j-1].flags & ST_ZONE)==0 )
					j--;
				s[j].flags |= ST_BLUE;
				for(j++; j<nold && s[j].value==c
						&& (s[j].flags & (ST_UP|ST_ZONE))==ST_UP ; j++)
					s[j].flags |= ST_BLUE;
				break;
			}
		}
	}
	/* bottom zones: the logic is the same as for baseline */
	for(i=0; i<notherb; i+=2) {
		a=otherblues[i]; b=otherblues[i+1];
		for(j=0; j<nold; j++) {
			if( (s[j].flags & (ST_UP|ST_ZONE|ST_END))!=ST_UP )
				continue;
			c=s[j].value;
			if(c>b) /* too high */
				break;
			if(c>=a) { /* found the lowest stem border */
				/* mark all the stems with the same value */
				if(ISDBG(BLUESTEMS))
					fprintf(pfa_file, "%% found U BLUE at %d\n", s[j].value);
				/* include ST_END values */
				while( s[j-1].value==c && (s[j-1].flags & ST_ZONE)==0 )
					j--;
				s[j].flags |= ST_BLUE;
				for(j++; j<nold && s[j].value==c
						&& (s[j].flags & (ST_UP|ST_ZONE))==ST_UP ; j++)
					s[j].flags |= ST_BLUE;
				break;
			}
		}
	}
}

/* Eliminate invalid stems, join equivalent lines and remove nested stems
 * to build the main (non-substituted) set of stems.
 * XXX add consideration of the italic angle
 */
static int
joinmainstems(
	  STEM * s,
	  int nold,
	  int useblues /* do we use the blue values ? */
)
{
#define MAX_STACK	1000
	STEM            stack[MAX_STACK];
	int             nstack = 0;
	int             sbottom = 0;
	int             nnew;
	int             i, j, k;
	int             a, b, c, w1, w2, w3;
	int             fw, fd;
	/*
	 * priority of the last found stem: 
	 * 0 - nothing found yet 
	 * 1 - has ST_END in it (one or more) 
	 * 2 - has no ST_END and no ST_FLAT, can override only one stem 
	 *     with priority 1 
	 * 3 - has no ST_END and at least one ST_FLAT, can override one 
	 *     stem with priority 2 or any number of stems with priority 1
	 * 4 (handled separately) - has ST_BLUE, can override anything
	 */
	int             readystem = 0;
	int             pri;
	int             nlps = 0;	/* number of non-committed
					 * lowest-priority stems */


	for (i = 0, nnew = 0; i < nold; i++) {
		if (s[i].flags & (ST_UP|ST_ZONE)) {
			if(s[i].flags & ST_BLUE) {
				/* we just HAVE to use this value */
				if (readystem)
					nnew += 2;
				readystem=0;

				/* remember the list of Blue zone stems with the same value */
				for(a=i, i++; i<nold && s[a].value==s[i].value
					&& (s[i].flags & ST_BLUE); i++)
					{}
				b=i; /* our range is a <= i < b */
				c= -1; /* index of our best guess up to now */
				pri=0;
				/* try to find a match, don't cross blue zones */
				for(; i<nold && (s[i].flags & ST_BLUE)==0; i++) {
					if(s[i].flags & ST_UP) {
						if(s[i].flags & ST_TOPZONE)
							break;
						else
							continue;
					}
					for(j=a; j<b; j++) {
						if(!stemoverlap(&s[j], &s[i]) )
							continue;
						/* consider priorities */
						if( ( (s[j].flags|s[i].flags) & (ST_FLAT|ST_END) )==ST_FLAT ) {
							c=i;
							goto bluematch;
						}
						if( ((s[j].flags|s[i].flags) & ST_END)==0 )  {
							if(pri < 2) {
								c=i; pri=2;
							}
						} else {
							if(pri == 0) {
								c=i; pri=1;
							}
						}
					}
				}
			bluematch:
				/* clean up the stack */
				nstack=sbottom=0;
				readystem=0;
				/* add this stem */
				s[nnew++]=s[a];
				if(c<0) { /* make one-dot-wide stem */
					if(nnew>=b) { /* have no free space */
						for(j=nold; j>=b; j--) /* make free space */
							s[j]=s[j-1];
						b++;
						nold++;
					}
					s[nnew]=s[a];
					s[nnew].flags &= ~(ST_UP|ST_BLUE);
					nnew++;
					i=b-1;
				} else {
					s[nnew++]=s[c];
					i=c; /* skip up to this point */
				}
				if (ISDBG(MAINSTEMS))
					fprintf(pfa_file, "%% +stem %d...%d U BLUE\n",
						s[nnew-2].value, s[nnew-1].value);
			} else {
				if (nstack >= MAX_STACK) {
					WARNING_1 fprintf(stderr, "Warning: **** converter's stem stack overflow ****\n");
					nstack = 0;
				}
				stack[nstack++] = s[i];
			}
		} else if(s[i].flags & ST_BLUE) {
			/* again, we just HAVE to use this value */
			if (readystem)
				nnew += 2;
			readystem=0;

			/* remember the list of Blue zone stems with the same value */
			for(a=i, i++; i<nold && s[a].value==s[i].value
				&& (s[i].flags & ST_BLUE); i++)
				{}
			b=i; /* our range is a <= i < b */
			c= -1; /* index of our best guess up to now */
			pri=0;
			/* try to find a match */
			for (i = nstack - 1; i >= 0; i--) {
				if( (stack[i].flags & ST_UP)==0 ) {
					if( (stack[i].flags & (ST_ZONE|ST_TOPZONE))==ST_ZONE )
						break;
					else
						continue;
				}
				for(j=a; j<b; j++) {
					if(!stemoverlap(&s[j], &stack[i]) )
						continue;
					/* consider priorities */
					if( ( (s[j].flags|stack[i].flags) & (ST_FLAT|ST_END) )==ST_FLAT ) {
						c=i;
						goto bluedownmatch;
					}
					if( ((s[j].flags|stack[i].flags) & ST_END)==0 )  {
						if(pri < 2) {
							c=i; pri=2;
						}
					} else {
						if(pri == 0) {
							c=i; pri=1;
						}
					}
				}
			}
		bluedownmatch:
			/* if found no match make a one-dot-wide stem */
			if(c<0) {
				c=0;
				stack[0]=s[b-1];
				stack[0].flags |= ST_UP;
				stack[0].flags &= ~ST_BLUE;
			}
			/* remove all the stems conflicting with this one */
			readystem=0;
			for(j=nnew-2; j>=0; j-=2) {
				if (ISDBG(MAINSTEMS))
					fprintf(pfa_file, "%% ?stem %d...%d -- %d\n",
						s[j].value, s[j+1].value, stack[c].value);
				if(s[j+1].value < stack[c].value) /* no conflict */
					break;
				if(s[j].flags & ST_BLUE) {
					/* oops, we don't want to spoil other blue zones */
					stack[c].value=s[j+1].value+1;
					break;
				}
				if( (s[j].flags|s[j+1].flags) & ST_END ) {
					if (ISDBG(MAINSTEMS))
						fprintf(pfa_file, "%% -stem %d...%d p=1\n",
							s[j].value, s[j+1].value);
					continue; /* pri==1, silently discard it */
				}
				/* we want to discard no nore than 2 stems of pri>=2 */
				if( ++readystem > 2 ) {
					/* change our stem to not conflict */
					stack[c].value=s[j+1].value+1;
					break;
				} else {
					if (ISDBG(MAINSTEMS))
						fprintf(pfa_file, "%% -stem %d...%d p>=2\n",
							s[j].value, s[j+1].value);
					continue;
				}
			}
			nnew=j+2;
			/* add this stem */
			if(nnew>=b-1) { /* have no free space */
				for(j=nold; j>=b-1; j--) /* make free space */
					s[j]=s[j-1];
				b++;
				nold++;
			}
			s[nnew++]=stack[c];
			s[nnew++]=s[b-1];
			/* clean up the stack */
			nstack=sbottom=0;
			readystem=0;
			/* set the next position to search */
			i=b-1;
			if (ISDBG(MAINSTEMS))
				fprintf(pfa_file, "%% +stem %d...%d D BLUE\n",
					s[nnew-2].value, s[nnew-1].value);
		} else if (nstack > 0) {

			/*
			 * check whether our stem overlaps with anything in
			 * stack
			 */
			for (j = nstack - 1; j >= sbottom; j--) {
				if (s[i].value <= stack[j].value)
					break;
				if (stack[j].flags & ST_ZONE)
					continue;

				if ((s[i].flags & ST_END)
				    || (stack[j].flags & ST_END))
					pri = 1;
				else if ((s[i].flags & ST_FLAT)
					 || (stack[j].flags & ST_FLAT))
					pri = 3;
				else
					pri = 2;

				if (pri < readystem && s[nnew + 1].value >= stack[j].value
				    || !stemoverlap(&stack[j], &s[i]))
					continue;

				if (readystem > 1 && s[nnew + 1].value < stack[j].value) {
					nnew += 2;
					readystem = 0;
					nlps = 0;
				}
				/*
				 * width of the previous stem (if it's
				 * present)
				 */
				w1 = s[nnew + 1].value - s[nnew].value;

				/* width of this stem */
				w2 = s[i].value - stack[j].value;

				if (readystem == 0) {
					/* nothing yet, just add a new stem */
					s[nnew] = stack[j];
					s[nnew + 1] = s[i];
					readystem = pri;
					if (pri == 1)
						nlps = 1;
					else if (pri == 2)
						sbottom = j;
					else {
						sbottom = j + 1;
						while (sbottom < nstack
						       && stack[sbottom].value <= stack[j].value)
							sbottom++;
					}
					if (ISDBG(MAINSTEMS))
						fprintf(pfa_file, "%% +stem %d...%d p=%d n=%d\n",
							stack[j].value, s[i].value, pri, nlps);
				} else if (pri == 1) {
					if (stack[j].value > s[nnew + 1].value) {
						/*
						 * doesn't overlap with the
						 * previous one
						 */
						nnew += 2;
						nlps++;
						s[nnew] = stack[j];
						s[nnew + 1] = s[i];
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% +stem %d...%d p=%d n=%d\n",
								stack[j].value, s[i].value, pri, nlps);
					} else if (w2 < w1) {
						/* is narrower */
						s[nnew] = stack[j];
						s[nnew + 1] = s[i];
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% /stem %d...%d p=%d n=%d %d->%d\n",
								stack[j].value, s[i].value, pri, nlps, w1, w2);
					}
				} else if (pri == 2) {
					if (readystem == 2) {
						/* choose the narrower stem */
						if (w1 > w2) {
							s[nnew] = stack[j];
							s[nnew + 1] = s[i];
							sbottom = j;
							if (ISDBG(MAINSTEMS))
								fprintf(pfa_file, "%% /stem %d...%d p=%d n=%d\n",
									stack[j].value, s[i].value, pri, nlps);
						}
						/* else readystem==1 */
					} else if (stack[j].value > s[nnew + 1].value) {
						/*
						 * value doesn't overlap with
						 * the previous one
						 */
						nnew += 2;
						nlps = 0;
						s[nnew] = stack[j];
						s[nnew + 1] = s[i];
						sbottom = j;
						readystem = pri;
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% +stem %d...%d p=%d n=%d\n",
								stack[j].value, s[i].value, pri, nlps);
					} else if (nlps == 1
						   || stack[j].value > s[nnew - 1].value) {
						/*
						 * we can replace the top
						 * stem
						 */
						nlps = 0;
						s[nnew] = stack[j];
						s[nnew + 1] = s[i];
						readystem = pri;
						sbottom = j;
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% /stem %d...%d p=%d n=%d\n",
								stack[j].value, s[i].value, pri, nlps);
					}
				} else if (readystem == 3) {	/* that means also
								 * pri==3 */
					/* choose the narrower stem */
					if (w1 > w2) {
						s[nnew] = stack[j];
						s[nnew + 1] = s[i];
						sbottom = j + 1;
						while (sbottom < nstack
						       && stack[sbottom].value <= stack[j].value)
							sbottom++;
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% /stem %d...%d p=%d n=%d\n",
								stack[j].value, s[i].value, pri, nlps);
					}
				} else if (pri == 3) {
					/*
					 * we can replace as many stems as
					 * neccessary
					 */
					nnew += 2;
					while (nnew > 0 && s[nnew - 1].value >= stack[j].value) {
						nnew -= 2;
						if (ISDBG(MAINSTEMS))
							fprintf(pfa_file, "%% -stem %d..%d\n",
								s[nnew].value, s[nnew + 1].value);
					}
					nlps = 0;
					s[nnew] = stack[j];
					s[nnew + 1] = s[i];
					readystem = pri;
					sbottom = j + 1;
					while (sbottom < nstack
					       && stack[sbottom].value <= stack[j].value)
						sbottom++;
					if (ISDBG(MAINSTEMS))
						fprintf(pfa_file, "%% +stem %d...%d p=%d n=%d\n",
							stack[j].value, s[i].value, pri, nlps);
				}
			}
		}
	}
	if (readystem)
		nnew += 2;

	/* change the 1-pixel-wide stems to 20-pixel-wide stems if possible 
	 * the constant 20 is recommended in the Type1 manual 
	 */
	if(useblues) {
		for(i=0; i<nnew; i+=2) {
			if(s[i].value != s[i+1].value)
				continue;
			if( ((s[i].flags ^ s[i+1].flags) & ST_BLUE)==0 )
				continue;
			if( s[i].flags & ST_BLUE ) {
				if(nnew>i+2 && s[i+2].value<s[i].value+22)
					s[i+1].value=s[i+2].value-2; /* compensate for fuzziness */
				else
					s[i+1].value+=20;
			} else {
				if(i>0 && s[i-1].value>s[i].value-22)
					s[i].value=s[i-1].value+2; /* compensate for fuzziness */
				else
					s[i].value-=20;
			}
		}
	}
	/* make sure that no stem it stretched between
	 * a top zone and a bottom zone
	 */
	if(useblues) {
		for(i=0; i<nnew; i+=2) {
			a=10000; /* lowest border of top zone crosing the stem */
			b= -10000; /* highest border of bottom zone crossing the stem */

			for(j=2; j<nblues; j++) {
				c=bluevalues[j];
				if( c>=s[i].value && c<=s[i+1].value && c<a )
					a=c;
			}
			if(nblues>=2) {
				c=bluevalues[1];
				if( c>=s[i].value && c<=s[i+1].value && c>b )
					b=c;
			}
			for(j=1; j<notherb; j++) {
				c=otherblues[j];
				if( c>=s[i].value && c<=s[i+1].value && c>b )
					b=c;
			}
			if( a!=10000 && b!= -10000 ) { /* it is stretched */
				/* split the stem into 2 ghost stems */
				for(j=nnew+1; j>i+1; j--) /* make free space */
					s[j]=s[j-2];
				nnew+=2;

				if(s[i].value+22 >= a)
					s[i+1].value=a-2; /* leave space for fuzziness */
				else
					s[i+1].value=s[i].value+20;

				if(s[i+3].value-22 <= b)
					s[i+2].value=b+2; /* leave space for fuzziness */
				else
					s[i+2].value=s[i+3].value-20;

				i+=2;
			}
		}
	}
	/* look for triple stems */
	for (i = 0; i < nnew; i += 2) {
		if (nnew - i >= 6) {
			a = s[i].value + s[i + 1].value;
			b = s[i + 2].value + s[i + 3].value;
			c = s[i + 4].value + s[i + 5].value;

			w1 = s[i + 1].value - s[i].value;
			w2 = s[i + 3].value - s[i + 2].value;
			w3 = s[i + 5].value - s[i + 4].value;

			fw = w3 - w1;	/* fuzz in width */
			fd = ((c - b) - (b - a));	/* fuzz in distance
							 * (doubled) */

			/* we are able to handle some fuzz */
			/*
			 * it doesn't hurt if the declared stem is a bit
			 * narrower than actual unless it's an edge in
			 * a blue zone
			 */
			if (abs(abs(fd) - abs(fw)) * 5 < w2
			    && abs(fw) * 20 < (w1 + w3)) {	/* width dirrerence <10% */

				if(useblues) { /* check that we don't disturb any blue stems */
					j=c; k=a;
					if (fw > 0) {
						if (fd > 0) {
							if( s[i+5].flags & ST_BLUE )
								continue;
							j -= fw;
						} else {
							if( s[i+4].flags & ST_BLUE )
								continue;
							j += fw;
						}
					} else if(fw < 0) {
						if (fd > 0) {
							if( s[i+1].flags & ST_BLUE )
								continue;
							k -= fw;
						} else {
							if( s[i].flags & ST_BLUE )
								continue;
							k += fw;
						}
					}
					pri = ((j - b) - (b - k));

					if (pri > 0) {
						if( s[i+2].flags & ST_BLUE )
							continue;
					} else if(pri < 0) {
						if( s[i+3].flags & ST_BLUE )
							continue;
					}
				}

				/*
				 * first fix up the width of 1st and 3rd
				 * stems
				 */
				if (fw > 0) {
					if (fd > 0) {
						s[i + 5].value -= fw;
						c -= fw;
					} else {
						s[i + 4].value += fw;
						c += fw;
					}
				} else {
					if (fd > 0) {
						s[i + 1].value -= fw;
						a -= fw;
					} else {
						s[i].value += fw;
						a += fw;
					}
				}
				fd = ((c - b) - (b - a));

				if (fd > 0) {
					s[i + 2].value += abs(fd) / 2;
				} else {
					s[i + 3].value -= abs(fd) / 2;
				}

				s[i].flags |= ST_3;
				i += 4;
			}
		}
	}

	return (nnew & ~1);	/* number of lines must be always even */
}

/*
 * these macros and function allow to set the base stem,
 * check that it's not empty and subtract another stem
 * from the base stem (possibly dividing it into multiple parts)
 */

/* pairs for pieces of the base stem */
static short xbstem[MAX_STEMS*2]; 
/* index of the last point */
static int xblast= -1; 

#define setbasestem(from, to) \
	(xbstem[0]=from, xbstem[1]=to, xblast=1)
#define isbaseempty()	(xblast<=0)

/* returns 1 if was overlapping, 0 otherwise */
static int
subfrombase(
	int from,
	int to
) 
{
	int a, b;
	int i, j;

	if(isbaseempty())
		return 0;

	/* handle the simple case simply */
	if(from > xbstem[xblast] || to < xbstem[0])
		return 0;

	/* the binary search may be more efficient */
	/* but for now the linear search is OK */
	for(b=1; from > xbstem[b]; b+=2) {} /* result: from <= xbstem[b] */
	for(a=xblast-1; to < xbstem[a]; a-=2) {} /* result: to >= xbstem[a] */

	/* now the interesting examples are:
	 * (it was hard for me to understand, so I looked at the examples)
	 * 1
	 *     a|-----|          |-----|b   |-----|     |-----|
	 *              f|-----|t
	 * 2
	 *     a|-----|b         |-----|    |-----|     |-----|
	 *      f|--|t
	 * 3
	 *     a|-----|b         |-----|    |-----|     |-----|
	 *           f|-----|t
	 * 4
	 *      |-----|b        a|-----|    |-----|     |-----|
	 *          f|------------|t
	 * 5
	 *      |-----|          |-----|b   |-----|    a|-----|
	 *                   f|-----------------------------|t
	 * 6
	 *      |-----|b         |-----|    |-----|    a|-----|
	 *   f|--------------------------------------------------|t
	 * 7
	 *      |-----|b         |-----|   a|-----|     |-----|
	 *          f|--------------------------|t
	 */

	if(a < b-1) /* hits a gap  - example 1 */
		return 0;

	/* now the subtraction itself */

	if(a==b-1 && from > xbstem[a] && to < xbstem[b]) {
		/* overlaps with only one subrange and splits it - example 2 */
		j=xblast; i=(xblast+=2);
		while(j>=b)
			xbstem[i--]=xbstem[j--];
		xbstem[b]=from-1;
		xbstem[b+1]=to+1;
		return 1;
	/* becomes
	 * 2a
	 *     a|b   ||          |-----|    |-----|     |-----|
	 *      f|--|t
	 */
	}

	if(xbstem[b-1] < from) {
		/* cuts the back of this subrange - examples 3, 4, 7 */
		xbstem[b] = from-1;
		b+=2;
	/* becomes
	 * 3a
	 *     a|----|           |-----|b   |-----|     |-----|
	 *           f|-----|t
	 * 4a
	 *      |---|           a|-----|b   |-----|     |-----|
	 *          f|------------|t
	 * 7a
	 *      |---|            |-----|b  a|-----|     |-----|
	 *          f|--------------------------|t
	 */
	}

	if(xbstem[a+1] > to) {
		/* cuts the front of this subrange - examples 4a, 5, 7a */
		xbstem[a] = to+1;
		a-=2;
	/* becomes
	 * 4b
	 *     a|---|              |---|b   |-----|     |-----|
	 *          f|------------|t
	 * 5b
	 *      |-----|          |-----|b  a|-----|          ||
	 *                   f|-----------------------------|t
	 * 7b
	 *      |---|           a|-----|b        ||     |-----|
	 *          f|--------------------------|t
	 */
	}

	if(a < b-1) /* now after modification it hits a gap - examples 3a, 4b */
		return 1; /* because we have removed something */

	/* now remove the subranges completely covered by the new stem */
	/* examples 5b, 6, 7b */
	i=b-1; j=a+2;
	/* positioned as:
	 * 5b                    i                           j
	 *      |-----|          |-----|b  a|-----|          ||
	 *                   f|-----------------------------|t
	 * 6    i                                             xblast  j
	 *      |-----|b         |-----|    |-----|    a|-----|
	 *   f|--------------------------------------------------|t
	 * 7b                    i               j
	 *      |---|           a|-----|b        ||     |-----|
	 *          f|--------------------------|t
	 */
	while(j <= xblast)
		xbstem[i++]=xbstem[j++];
	xblast=i-1;
	return 1;
}

/* for debugging */
static void
printbasestem(void)
{
	int i;

	printf("( ");
	for(i=0; i<xblast; i+=2)
		printf("%d-%d ", xbstem[i], xbstem[i+1]);
	printf(") %d\n", xblast);
}

/*
 * Join the stem borders to build the sets of substituted stems
 * XXX add consideration of the italic angle
 */
static void
joinsubstems(
	  STEM * s,
	  short *pairs,
	  int nold,
	  int useblues /* do we use the blue values ? */
)
{
	int i, j, x;
	static unsigned char mx[MAX_STEMS][MAX_STEMS];

	/* we do the substituted groups of stems first
	 * and it looks like it's going to be REALLY SLOW 
	 * AND PAINFUL but let's bother about it later
	 */

	/* for the substituted stems we don't bother about [hv]stem3 -
	 * anyway the X11R6 rasterizer does not bother about hstem3
	 * at all and is able to handle only one global vstem3
	 * per glyph 
	 */

	/* clean the used part of matrix */
	for(i=0; i<nold; i++)
		for(j=0; j<nold; j++)
			mx[i][j]=0;

	/* build the matrix of stem pairs */
	for(i=0; i<nold; i++) {
		if( s[i].flags & ST_ZONE )
			continue;
		if(s[i].flags & ST_BLUE)
			mx[i][i]=1; /* allow to pair with itself if no better pair */
		if(s[i].flags & ST_UP) { /* the down-stems are already matched */
			setbasestem(s[i].from, s[i].to);
			for(j=i+1; j<nold; j++) {
				if(s[i].value==s[j].value
				|| s[j].flags & ST_ZONE ) {
					continue;
				}
				x=subfrombase(s[j].from, s[j].to);

				if(s[j].flags & ST_UP) /* match only up+down pairs */
					continue;

				mx[i][j]=mx[j][i]=x;

				if(isbaseempty()) /* nothing else to do */
					break;
			}
		}
	}

	if(ISDBG(SUBSTEMS)) {
		fprintf(pfa_file, "%%     ");
		for(j=0; j<nold; j++)
			putc( j%10==0 ? '0'+(j/10)%10 : ' ', pfa_file);
		fprintf(pfa_file, "\n%%     ");
		for(j=0; j<nold; j++)
			putc('0'+j%10, pfa_file);
		putc('\n', pfa_file);
		for(i=0; i<nold; i++) {
			fprintf(pfa_file, "%% %3d ",i);
			for(j=0; j<nold; j++)
				putc( mx[i][j] ? 'X' : '.', pfa_file);
			putc('\n', pfa_file);
		}
	}

	/* now use the matrix to find the best pair for each stem */
	for(i=0; i<nold; i++) {
		int pri, lastpri, v, f;

		x= -1; /* best pair: none */
		lastpri=0;

		v=s[i].value;
		f=s[i].flags;

		if(f & ST_ZONE) {
			pairs[i]= -1;
			continue;
		}

		if(f & ST_UP) {
			for(j=i+1; j<nold; j++) {
				if(mx[i][j]==0)
					continue;

				if( (f | s[j].flags) & ST_END )
					pri=1;
				else if( (f | s[j].flags) & ST_FLAT )
					pri=3;
				else
					pri=2;

				if(lastpri==0
				|| pri > lastpri  
				&& ( lastpri==1 || s[j].value-v<20 || (s[x].value-v)*2 >= s[j].value-v ) ) {
					lastpri=pri;
					x=j;
				}
			}
		} else {
			for(j=i-1; j>=0; j--) {
				if(mx[i][j]==0)
					continue;

				if( (f | s[j].flags) & ST_END )
					pri=1;
				else if( (f | s[j].flags) & ST_FLAT )
					pri=3;
				else
					pri=2;

				if(lastpri==0
				|| pri > lastpri  
				&& ( lastpri==1 || v-s[j].value<20 || (v-s[x].value)*2 >= v-s[j].value ) ) {
					lastpri=pri;
					x=j;
				}
			}
		}
		if(x== -1 && mx[i][i])
			pairs[i]=i; /* a special case */
		else
			pairs[i]=x;
	}

	if(ISDBG(SUBSTEMS)) {
		for(i=0; i<nold; i++) {
			j=pairs[i];
			if(j>0)
				fprintf(pfa_file, "%% %d...%d  (%d x %d)\n", s[i].value, s[j].value, i, j);
		}
	}
}

/*
 * Make all the stems originating at the same value get the
 * same width. Without this the rasterizer may move the dots
 * randomly up or down by one pixel, and that looks bad.
 * The prioritisation is the same as in findstemat().
 */
static void
uniformstems(
	  STEM * s,
	  short *pairs,
	  int ns
)
{
	int i, j, from, to, val, dir;
	int pri, prevpri[2], wd, prevwd[2], prevbest[2];

	for(from=0; from<ns; from=to) {
		prevpri[0] = prevpri[1] = 0;
		prevwd[0] = prevwd[1] = 0;
		prevbest[0] = prevbest[1] = -1;
		val = s[from].value;

		for(to = from; to<ns && s[to].value == val; to++) {
			dir = ((s[to].flags & ST_UP)!=0);

			i=pairs[to]; /* the other side of this stem */
			if(i<0 || i==to)
				continue; /* oops, no other side */
			wd=abs(s[i].value-val);
			if(wd == 0)
				continue;
			pri=1;
			if( (s[to].flags | s[i].flags) & ST_END )
				pri=0;
			if( prevbest[dir] == -1 || pri > prevpri[dir] || wd<prevwd[dir] ) {
				prevbest[dir]=i;
				prevpri[dir]=pri;
				prevwd[dir]=wd;
			}
		}

		for(i=from; i<to; i++) {
			dir = ((s[i].flags & ST_UP)!=0);
			if(prevbest[dir] >= 0) {
				if(ISDBG(SUBSTEMS)) {
					fprintf(stderr, "at %d (%s %d) pair %d->%d(%d)\n", i, 
						(dir ? "UP":"DOWN"), s[i].value, pairs[i], prevbest[dir],
						s[prevbest[dir]].value);
				}
				pairs[i] = prevbest[dir];
			}
		}
	}
}

/* 
 * Find the best stem in the array at the specified (value, origin),
 * related to the entry ge.
 * Returns its index in the array sp, -1 means "none".
 * prevbest is the result for the other end of the line, we must 
 * find something better than it or leave it as it is.
 */
static int
findstemat(
	int value,
	int origin,
	GENTRY *ge,
	STEM *sp,
	short *pairs,
	int ns,
	int prevbest /* -1 means "none" */
)
{
	int i, min, max;
	int v, si;
	int pri, prevpri; /* priority, 0 = has ST_END, 1 = no ST_END */
	int wd, prevwd; /* stem width */

	si= -1; /* nothing yet */

	/* stems are ordered by value, binary search */
	min=0; max=ns; /* min <= i < max */
	while( min < max ) {
		i=(min+max)/2;
		v=sp[i].value;
		if(v<value)
			min=i+1;
		else if(v>value)
			max=i;
		else {
			si=i; /* temporary value */
			break;
		}
	}

	if( si < 0 ) /* found nothing this time */
		return prevbest;

	/* find the priority of the prevbest */
	/* we expect that prevbest has a pair */
	if(prevbest>=0) {
		i=pairs[prevbest];
		prevpri=1;
		if( (sp[prevbest].flags | sp[i].flags) & ST_END )
			prevpri=0; 
		prevwd=abs(sp[i].value-value);
	}

	/* stems are not ordered by origin, so now do the linear search */

	while( si>0 && sp[si-1].value==value ) /* find the first one */
		si--;

	for(; si<ns && sp[si].value==value; si++) {
		if(sp[si].origin != origin) 
			continue;
		if(sp[si].ge != ge) {
			if(ISDBG(SUBSTEMS)) {
				fprintf(stderr, 
					"dbg: possible self-intersection at v=%d o=%d exp_ge=0x%x ge=0x%x\n",
					value, origin, ge, sp[si].ge);
			}
			continue;
		}
		i=pairs[si]; /* the other side of this stem */
		if(i<0)
			continue; /* oops, no other side */
		pri=1;
		if( (sp[si].flags | sp[i].flags) & ST_END )
			pri=0;
		wd=abs(sp[i].value-value);
		if( prevbest == -1 || pri >prevpri 
		|| pri==prevpri && prevwd==0 || wd!=0 && wd<prevwd ) {
			prevbest=si;
			prevpri=pri;
			prevwd=wd;
			continue;
		}
	}

	return prevbest;
}

/* add the substems for one glyph entry 
 * (called from groupsubstems())
 * returns 0 if all OK, 1 if too many groups
 */

static int gssentry_lastgrp=0; /* reset to 0 for each new glyph */

static int
gssentry( /* crazy number of parameters */
	GENTRY *ge,
	STEM *hs, /* horizontal stems, sorted by value */
	short *hpairs,
	int nhs,
	STEM *vs, /* vertical stems, sorted by value */
	short *vpairs,
	int nvs,
	STEMBOUNDS *s,
	short *egp,
	int *nextvsi, 
	int *nexthsi /* -2 means "check by yourself" */
) {
	enum {
		SI_VP,	/* vertical primary */
		SI_HP,	/* horizontal primary */
		SI_SIZE /* size of the array */
	};
	int si[SI_SIZE]; /* indexes of relevant stems */

	/* the bounds of the existing relevant stems */
	STEMBOUNDS r[ sizeof(si) / sizeof(si[0]) * 2 ];
	char rexpand; /* by how much we need to expand the group */
	int nr; /* and the number of them */

	/* yet more temporary storage */
	short lb, hb, isvert;
	int conflict, grp;
	int i, j, x, y;


	/* for each line or curve we try to find a horizontal and
	 * a vertical stem corresponding to its first point
	 * (corresponding to the last point of the previous
	 * glyph entry), because the directions of the lines
	 * will be eventually reversed and it will then become the last
	 * point. And the T1 rasterizer applies the hints to 
	 * the last point.
	 *
	 */

	/* start with the common part, the first point */
	x=ge->prev->ix3;
	y=ge->prev->iy3;

	if(*nextvsi == -2)
		si[SI_VP]=findstemat(x, y, ge, vs, vpairs, nvs, -1);
	else {
		si[SI_VP]= *nextvsi; *nextvsi= -2;
	}
	if(*nexthsi == -2)
		si[SI_HP]=findstemat(y, x, ge, hs, hpairs, nhs, -1);
	else {
		si[SI_HP]= *nexthsi; *nexthsi= -2;
	}

	/*
	 * For the horizontal lines we make sure that both
	 * ends of the line have the same horizontal stem,
	 * and the same thing for vertical lines and stems.
	 * In both cases we enforce the stem for the next entry.
	 * Otherwise unpleasant effects may arise.
	 */

	if(ge->type==GE_LINE) {
		if(ge->ix3==x) { /* vertical line */
			*nextvsi=si[SI_VP]=findstemat(x, ge->iy3, ge->frwd, vs, vpairs, nvs, si[SI_VP]);
		} else if(ge->iy3==y) { /* horizontal line */
			*nexthsi=si[SI_HP]=findstemat(y, ge->ix3, ge->frwd, hs, hpairs, nhs, si[SI_HP]);
		}
	}

	if(si[SI_VP]+si[SI_HP] == -2) /* no stems, leave it alone */
		return 0;

	/* build the array of relevant bounds */
	nr=0;
	for(i=0; i< sizeof(si) / sizeof(si[0]); i++) {
		STEM *sp;
		short *pairs;
		int step;
		int f;
		int nzones, firstzone, binzone, einzone;
		int btype, etype;

		if(si[i] < 0)
			continue;

		if(i<SI_HP) {
			r[nr].isvert=1; sp=vs; pairs=vpairs;
		} else {
			r[nr].isvert=0; sp=hs; pairs=hpairs;
		}

		r[nr].low=sp[ si[i] ].value;
		r[nr].high=sp[ pairs[ si[i] ] ].value;

		if(r[nr].low > r[nr].high) {
			j=r[nr].low; r[nr].low=r[nr].high; r[nr].high=j;
			step= -1;
		} else {
			step=1;
		}

		/* handle the interaction with Blue Zones */

		if(i>=SI_HP) { /* only for horizontal stems */
			if(si[i]==pairs[si[i]]) {
				/* special case, the outermost stem in the
				 * Blue Zone without a pair, simulate it to 20-pixel
				 */
				if(sp[ si[i] ].flags & ST_UP) {
					r[nr].high+=20;
					for(j=si[i]+1; j<nhs; j++)
						if( (sp[j].flags & (ST_ZONE|ST_TOPZONE))
						== (ST_ZONE|ST_TOPZONE) ) {
							if(r[nr].high > sp[j].value-2)
								r[nr].high=sp[j].value-2;
							break;
						}
				} else {
					r[nr].low-=20;
					for(j=si[i]-1; j>=0; j--)
						if( (sp[j].flags & (ST_ZONE|ST_TOPZONE))
						== (ST_ZONE) ) {
							if(r[nr].low < sp[j].value+2)
								r[nr].low=sp[j].value+2;
							break;
						}
				}
			}

			/* check that the stem borders don't end up in
			 * different Blue Zones */
			f=sp[ si[i] ].flags;
			nzones=0; einzone=binzone=0;
			for(j=si[i]; j!=pairs[ si[i] ]; j+=step) {
				if( (sp[j].flags & ST_ZONE)==0 )
					continue;
				/* if see a zone border going in the same direction */
				if( ((f ^ sp[j].flags) & ST_UP)==0 ) {
					if( ++nzones == 1 ) {
						firstzone=sp[j].value; /* remember the first one */
						etype=sp[j].flags & ST_TOPZONE;
					}
					einzone=1;

				} else { /* the opposite direction */
					if(nzones==0) { /* beginning is in a blue zone */
						binzone=1;
						btype=sp[j].flags & ST_TOPZONE;
					}
					einzone=0;
				}
			}

			/* beginning and end are in Blue Zones of different types */
			if( binzone && einzone && (btype ^ etype)!=0 ) {
				if( sp[si[i]].flags & ST_UP ) {
					if(firstzone > r[nr].low+22)
						r[nr].high=r[nr].low+20;
					else
						r[nr].high=firstzone-2;
				} else {
					if(firstzone < r[nr].high-22)
						r[nr].low=r[nr].high-20;
					else
						r[nr].low=firstzone+2;
				}
			}
		}

		if(ISDBG(SUBSTEMS))
			fprintf(pfa_file, "%%  at(%d,%d)[%d,%d] %d..%d %c (%d x %d)\n", x, y, i, nr,
				r[nr].low, r[nr].high, r[nr].isvert ? 'v' : 'h',
				si[i], pairs[si[i]]);

		nr++;
	}

	/* now try to find a group */
	conflict=0; /* no conflicts found yet */
	for(j=0; j<nr; j++)
		r[j].already=0;

	/* check if it fits into the last group */
	grp = gssentry_lastgrp;
	i = (grp==0)? 0 : egp[grp-1];
	for(; i<egp[grp]; i++) {
		lb=s[i].low; hb=s[i].high; isvert=s[i].isvert;
		for(j=0; j<nr; j++)
			if( r[j].isvert==isvert  /* intersects */
			&& r[j].low <= hb && r[j].high >= lb ) {
				if( r[j].low == lb && r[j].high == hb ) /* coincides */
					r[j].already=1;
				else
					conflict=1;
			}

		if(conflict) 
			break;
	}

	if(conflict) { /* nope, check all the groups */
		for(j=0; j<nr; j++)
			r[j].already=0;

		for(i=0, grp=0; i<egp[NSTEMGRP-1]; i++) {
			if(i == egp[grp]) { /* checked all stems in a group */
				if(conflict) {
					grp++; conflict=0; /* check the next group */
					for(j=0; j<nr; j++)
						r[j].already=0;
				} else
					break; /* insert into this group */
			}

			lb=s[i].low; hb=s[i].high; isvert=s[i].isvert;
			for(j=0; j<nr; j++)
				if( r[j].isvert==isvert  /* intersects */
				&& r[j].low <= hb && r[j].high >= lb ) {
					if( r[j].low == lb && r[j].high == hb ) /* coincides */
						r[j].already=1;
					else
						conflict=1;
				}

			if(conflict) 
				i=egp[grp]-1; /* fast forward to the next group */
		}
	}

	/* do we have any empty group ? */
	if(conflict && grp < NSTEMGRP-1) {
		grp++; conflict=0;
		for(j=0; j<nr; j++)
			r[j].already=0;
	}

	if(conflict) { /* oops, can't find any group to fit */
		return 1;
	}

	/* OK, add stems to this group */

	rexpand = nr;
	for(j=0; j<nr; j++)
		rexpand -= r[j].already;

	if(rexpand > 0) {
		for(i=egp[NSTEMGRP-1]-1; i>=egp[grp]; i--)
			s[i+rexpand]=s[i];
		for(i=0; i<nr; i++)
			if(!r[i].already)
				s[egp[grp]++]=r[i];
		for(i=grp+1; i<NSTEMGRP; i++)
			egp[i]+=rexpand;
	}

	ge->stemid = gssentry_lastgrp = grp;
	return 0;
}

/*
 * Create the groups of substituted stems from the list.
 * Each group will be represented by a subroutine in the Subs
 * array.
 */

static void
groupsubstems(
	GLYPH *g,
	STEM *hs, /* horizontal stems, sorted by value */
	short *hpairs,
	int nhs,
	STEM *vs, /* vertical stems, sorted by value */
	short *vpairs,
	int nvs
)
{
	GENTRY *ge;
	int i, j;

	/* temporary storage */
	STEMBOUNDS s[MAX_STEMS*2];
	/* indexes in there, pointing past the end each stem group */
	short egp[NSTEMGRP]; 

	int nextvsi, nexthsi; /* -2 means "check by yourself" */

	for(i=0; i<NSTEMGRP; i++)
		egp[i]=0;

	nextvsi=nexthsi= -2; /* processed no horiz/vert line */

	gssentry_lastgrp = 0; /* reset the last group for new glyph */

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if(ge->type!=GE_LINE && ge->type!=GE_CURVE) {
			nextvsi=nexthsi= -2; /* next path is independent */
			continue;
		}

		if( gssentry(ge, hs, hpairs, nhs, vs, vpairs, nvs, s, egp, &nextvsi, &nexthsi) ) {
			WARNING_2 fprintf(stderr, "*** glyph %s requires over %d hint subroutines, ignored them\n",
				g->name, NSTEMGRP);
			/* it's better to have no substituted hints at all than have only part */
			for (ge = g->entries; ge != 0; ge = ge->next)
				ge->stemid= -1;
			g->nsg=0; /* just to be safe, already is 0 by initialization */
			return;
		}

		/*
		 * handle the last vert/horiz line of the path specially,
		 * correct the hint for the first entry of the path
		 */
		if(ge->frwd != ge->next && (nextvsi != -2 || nexthsi != -2) ) {
			if( gssentry(ge->frwd, hs, hpairs, nhs, vs, vpairs, nvs, s, egp, &nextvsi, &nexthsi) ) {
				WARNING_2 fprintf(stderr, "*** glyph %s requires over %d hint subroutines, ignored them\n",
					g->name, NSTEMGRP);
				/* it's better to have no substituted hints at all than have only part */
				for (ge = g->entries; ge != 0; ge = ge->next)
					ge->stemid= -1;
				g->nsg=0; /* just to be safe, already is 0 by initialization */
				return;
			}
		}

	}

	/* find the index of the first empty group - same as the number of groups */
	if(egp[0]>0) {
		for(i=1; i<NSTEMGRP && egp[i]!=egp[i-1]; i++)
			{}
		g->nsg=i;
	} else
		g->nsg=0;

	if(ISDBG(SUBSTEMS)) {
		fprintf(pfa_file, "%% %d substem groups (%d %d %d)\n", g->nsg,
			g->nsg>1 ? egp[g->nsg-2] : -1,
			g->nsg>0 ? egp[g->nsg-1] : -1,
			g->nsg<NSTEMGRP ? egp[g->nsg] : -1 );
		j=0;
		for(i=0; i<g->nsg; i++) {
			fprintf(pfa_file, "%% grp %3d:      ", i);
			for(; j<egp[i]; j++) {
				fprintf(pfa_file, " %4d...%-4d %c  ", s[j].low, s[j].high,
					s[j].isvert ? 'v' : 'h');
			}
			fprintf(pfa_file, "\n");
		}
	}

	if(g->nsg==1) { /* it would be the same as the main stems */
		/* so erase it */
		for (ge = g->entries; ge != 0; ge = ge->next)
			ge->stemid= -1;
		g->nsg=0;
	}

	if(g->nsg>0) {
		if( (g->nsbs=malloc(g->nsg * sizeof (egp[0]))) == 0 ) {
			fprintf(stderr, "**** not enough memory for substituted hints ****\n");
			exit(255);
		}
		memmove(g->nsbs, egp, g->nsg * sizeof(short));
		if( (g->sbstems=malloc(egp[g->nsg-1] * sizeof (s[0]))) == 0 ) {
			fprintf(stderr, "**** not enough memory for substituted hints ****\n");
			exit(255);
		}
		memmove(g->sbstems, s, egp[g->nsg-1] * sizeof(s[0]));
	}
}

void
buildstems(
	   GLYPH * g
)
{
	STEM            hs[MAX_STEMS], vs[MAX_STEMS];	/* temporary working
							 * storage */
	short	hs_pairs[MAX_STEMS], vs_pairs[MAX_STEMS]; /* best pairs for these stems */
	STEM           *sp;
	GENTRY         *ge, *nge, *pge;
	int             nx, ny;
	int ovalue;
	int totals, grp, lastgrp;

	assertisint(g, "buildstems int");

	g->nhs = g->nvs = 0;
	memset(hs, 0, sizeof hs);
	memset(vs, 0, sizeof vs);

	/* first search the whole character for possible stem points */

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type == GE_CURVE) {

			/*
			 * SURPRISE! 
			 * We consider the stems bound by the
			 * H/V ends of the curves as flat ones.
			 *
			 * But we don't include the point on the
			 * other end into the range.
			 */

			/* first check the beginning of curve */
			/* if it is horizontal, add a hstem */
			if (ge->iy1 == ge->prev->iy3) {
				hs[g->nhs].value = ge->iy1;

				if (ge->ix1 < ge->prev->ix3)
					hs[g->nhs].flags = ST_FLAT | ST_UP;
				else
					hs[g->nhs].flags = ST_FLAT;

				hs[g->nhs].origin = ge->prev->ix3;
				hs[g->nhs].ge = ge;

				if (ge->ix1 < ge->prev->ix3) {
					hs[g->nhs].from = ge->ix1+1;
					hs[g->nhs].to = ge->prev->ix3;
					if(hs[g->nhs].from > hs[g->nhs].to)
						hs[g->nhs].from--;
				} else {
					hs[g->nhs].from = ge->prev->ix3;
					hs[g->nhs].to = ge->ix1-1;
					if(hs[g->nhs].from > hs[g->nhs].to)
						hs[g->nhs].to++;
				}
				if (ge->ix1 != ge->prev->ix3)
					g->nhs++;
			}
			/* if it is vertical, add a vstem */
			else if (ge->ix1 == ge->prev->ix3) {
				vs[g->nvs].value = ge->ix1;

				if (ge->iy1 > ge->prev->iy3)
					vs[g->nvs].flags = ST_FLAT | ST_UP;
				else
					vs[g->nvs].flags = ST_FLAT;

				vs[g->nvs].origin = ge->prev->iy3;
				vs[g->nvs].ge = ge;

				if (ge->iy1 < ge->prev->iy3) {
					vs[g->nvs].from = ge->iy1+1;
					vs[g->nvs].to = ge->prev->iy3;
					if(vs[g->nvs].from > vs[g->nvs].to)
						vs[g->nvs].from--;
				} else {
					vs[g->nvs].from = ge->prev->iy3;
					vs[g->nvs].to = ge->iy1-1;
					if(vs[g->nvs].from > vs[g->nvs].to)
						vs[g->nvs].to++;
				}

				if (ge->iy1 != ge->prev->iy3)
					g->nvs++;
			}
			/* then check the end of curve */
			/* if it is horizontal, add a hstem */
			if (ge->iy3 == ge->iy2) {
				hs[g->nhs].value = ge->iy3;

				if (ge->ix3 < ge->ix2)
					hs[g->nhs].flags = ST_FLAT | ST_UP;
				else
					hs[g->nhs].flags = ST_FLAT;

				hs[g->nhs].origin = ge->ix3;
				hs[g->nhs].ge = ge->frwd;

				if (ge->ix3 < ge->ix2) {
					hs[g->nhs].from = ge->ix3;
					hs[g->nhs].to = ge->ix2-1;
					if( hs[g->nhs].from > hs[g->nhs].to )
						hs[g->nhs].to++;
				} else {
					hs[g->nhs].from = ge->ix2+1;
					hs[g->nhs].to = ge->ix3;
					if( hs[g->nhs].from > hs[g->nhs].to )
						hs[g->nhs].from--;
				}

				if (ge->ix3 != ge->ix2)
					g->nhs++;
			}
			/* if it is vertical, add a vstem */
			else if (ge->ix3 == ge->ix2) {
				vs[g->nvs].value = ge->ix3;

				if (ge->iy3 > ge->iy2)
					vs[g->nvs].flags = ST_FLAT | ST_UP;
				else
					vs[g->nvs].flags = ST_FLAT;

				vs[g->nvs].origin = ge->iy3;
				vs[g->nvs].ge = ge->frwd;

				if (ge->iy3 < ge->iy2) {
					vs[g->nvs].from = ge->iy3;
					vs[g->nvs].to = ge->iy2-1;
					if( vs[g->nvs].from > vs[g->nvs].to )
						vs[g->nvs].to++;
				} else {
					vs[g->nvs].from = ge->iy2+1;
					vs[g->nvs].to = ge->iy3;
					if( vs[g->nvs].from > vs[g->nvs].to )
						vs[g->nvs].from--;
				}

				if (ge->iy3 != ge->iy2)
					g->nvs++;
			} else {

				/*
				 * check the end of curve for a not smooth
				 * local extremum
				 */
				nge = ge->frwd;

				if (nge == 0)
					continue;
				else if (nge->type == GE_LINE) {
					nx = nge->ix3;
					ny = nge->iy3;
				} else if (nge->type == GE_CURVE) {
					nx = nge->ix1;
					ny = nge->iy1;
				} else
					continue;

				/* check for vertical extremums */
				if (ge->iy3 > ge->iy2 && ge->iy3 > ny
				|| ge->iy3 < ge->iy2 && ge->iy3 < ny) {
					hs[g->nhs].value = ge->iy3;
					hs[g->nhs].from
						= hs[g->nhs].to
						= hs[g->nhs].origin = ge->ix3;
					hs[g->nhs].ge = ge->frwd;

					if (ge->ix3 < ge->ix2
					    || nx < ge->ix3)
						hs[g->nhs].flags = ST_UP;
					else
						hs[g->nhs].flags = 0;

					if (ge->ix3 != ge->ix2 || nx != ge->ix3)
						g->nhs++;
				}
				/*
				 * the same point may be both horizontal and
				 * vertical extremum
				 */
				/* check for horizontal extremums */
				if (ge->ix3 > ge->ix2 && ge->ix3 > nx
				|| ge->ix3 < ge->ix2 && ge->ix3 < nx) {
					vs[g->nvs].value = ge->ix3;
					vs[g->nvs].from
						= vs[g->nvs].to
						= vs[g->nvs].origin = ge->iy3;
					vs[g->nvs].ge = ge->frwd;

					if (ge->iy3 > ge->iy2
					    || ny > ge->iy3)
						vs[g->nvs].flags = ST_UP;
					else
						vs[g->nvs].flags = 0;

					if (ge->iy3 != ge->iy2 || ny != ge->iy3)
						g->nvs++;
				}
			}

		} else if (ge->type == GE_LINE) {
			nge = ge->frwd;

			/* if it is horizontal, add a hstem */
			/* and the ends as vstems if they brace the line */
			if (ge->iy3 == ge->prev->iy3
			&& ge->ix3 != ge->prev->ix3) {
				hs[g->nhs].value = ge->iy3;
				if (ge->ix3 < ge->prev->ix3) {
					hs[g->nhs].flags = ST_FLAT | ST_UP;
					hs[g->nhs].from = ge->ix3;
					hs[g->nhs].to = ge->prev->ix3;
				} else {
					hs[g->nhs].flags = ST_FLAT;
					hs[g->nhs].from = ge->prev->ix3;
					hs[g->nhs].to = ge->ix3;
				}
				hs[g->nhs].origin = ge->ix3;
				hs[g->nhs].ge = ge->frwd;

				pge = ge->bkwd;

				/* add beginning as vstem */
				vs[g->nvs].value = pge->ix3;
				vs[g->nvs].origin
					= vs[g->nvs].from
					= vs[g->nvs].to = pge->iy3;
				vs[g->nvs].ge = ge;

				if(pge->type==GE_CURVE)
					ovalue=pge->iy2;
				else
					ovalue=pge->prev->iy3;

				if (pge->iy3 > ovalue)
					vs[g->nvs].flags = ST_UP | ST_END;
				else if (pge->iy3 < ovalue)
					vs[g->nvs].flags = ST_END;
				else
					vs[g->nvs].flags = 0;

				if( vs[g->nvs].flags != 0 )
					g->nvs++;

				/* add end as vstem */
				vs[g->nvs].value = ge->ix3;
				vs[g->nvs].origin
					= vs[g->nvs].from
					= vs[g->nvs].to = ge->iy3;
				vs[g->nvs].ge = ge->frwd;

				if(nge->type==GE_CURVE)
					ovalue=nge->iy1;
				else
					ovalue=nge->iy3;

				if (ovalue > ge->iy3)
					vs[g->nvs].flags = ST_UP | ST_END;
				else if (ovalue < ge->iy3)
					vs[g->nvs].flags = ST_END;
				else
					vs[g->nvs].flags = 0;

				if( vs[g->nvs].flags != 0 )
					g->nvs++;

				g->nhs++;
			}
			/* if it is vertical, add a vstem */
			/* and the ends as hstems if they brace the line  */
			else if (ge->ix3 == ge->prev->ix3 
			&& ge->iy3 != ge->prev->iy3) {
				vs[g->nvs].value = ge->ix3;
				if (ge->iy3 > ge->prev->iy3) {
					vs[g->nvs].flags = ST_FLAT | ST_UP;
					vs[g->nvs].from = ge->prev->iy3;
					vs[g->nvs].to = ge->iy3;
				} else {
					vs[g->nvs].flags = ST_FLAT;
					vs[g->nvs].from = ge->iy3;
					vs[g->nvs].to = ge->prev->iy3;
				}
				vs[g->nvs].origin = ge->iy3;
				vs[g->nvs].ge = ge->frwd;

				pge = ge->bkwd;

				/* add beginning as hstem */
				hs[g->nhs].value = pge->iy3;
				hs[g->nhs].origin
					= hs[g->nhs].from
					= hs[g->nhs].to = pge->ix3;
				hs[g->nhs].ge = ge;

				if(pge->type==GE_CURVE)
					ovalue=pge->ix2;
				else
					ovalue=pge->prev->ix3;

				if (pge->ix3 < ovalue)
					hs[g->nhs].flags = ST_UP | ST_END;
				else if (pge->ix3 > ovalue)
					hs[g->nhs].flags = ST_END;
				else
					hs[g->nhs].flags = 0;

				if( hs[g->nhs].flags != 0 )
					g->nhs++;

				/* add end as hstem */
				hs[g->nhs].value = ge->iy3;
				hs[g->nhs].origin
					= hs[g->nhs].from
					= hs[g->nhs].to = ge->ix3;
				hs[g->nhs].ge = ge->frwd;

				if(nge->type==GE_CURVE)
					ovalue=nge->ix1;
				else
					ovalue=nge->ix3;

				if (ovalue < ge->ix3)
					hs[g->nhs].flags = ST_UP | ST_END;
				else if (ovalue > ge->ix3)
					hs[g->nhs].flags = ST_END;
				else
					hs[g->nhs].flags = 0;

				if( hs[g->nhs].flags != 0 )
					g->nhs++;

				g->nvs++;
			}
			/*
			 * check the end of line for a not smooth local
			 * extremum
			 */
			nge = ge->frwd;

			if (nge == 0)
				continue;
			else if (nge->type == GE_LINE) {
				nx = nge->ix3;
				ny = nge->iy3;
			} else if (nge->type == GE_CURVE) {
				nx = nge->ix1;
				ny = nge->iy1;
			} else
				continue;

			/* check for vertical extremums */
			if (ge->iy3 > ge->prev->iy3 && ge->iy3 > ny
			|| ge->iy3 < ge->prev->iy3 && ge->iy3 < ny) {
				hs[g->nhs].value = ge->iy3;
				hs[g->nhs].from
					= hs[g->nhs].to
					= hs[g->nhs].origin = ge->ix3;
				hs[g->nhs].ge = ge->frwd;

				if (ge->ix3 < ge->prev->ix3
				    || nx < ge->ix3)
					hs[g->nhs].flags = ST_UP;
				else
					hs[g->nhs].flags = 0;

				if (ge->ix3 != ge->prev->ix3 || nx != ge->ix3)
					g->nhs++;
			}
			/*
			 * the same point may be both horizontal and vertical
			 * extremum
			 */
			/* check for horizontal extremums */
			if (ge->ix3 > ge->prev->ix3 && ge->ix3 > nx
			|| ge->ix3 < ge->prev->ix3 && ge->ix3 < nx) {
				vs[g->nvs].value = ge->ix3;
				vs[g->nvs].from
					= vs[g->nvs].to
					= vs[g->nvs].origin = ge->iy3;
				vs[g->nvs].ge = ge->frwd;

				if (ge->iy3 > ge->prev->iy3
				    || ny > ge->iy3)
					vs[g->nvs].flags = ST_UP;
				else
					vs[g->nvs].flags = 0;

				if (ge->iy3 != ge->prev->iy3 || ny != ge->iy3)
					g->nvs++;
			}
		}
	}

	g->nhs=addbluestems(hs, g->nhs);
	sortstems(hs, g->nhs);
	sortstems(vs, g->nvs);

	if (ISDBG(STEMS))
		debugstems(g->name, hs, g->nhs, vs, g->nvs);

	/* find the stems interacting with the Blue Zones */
	markbluestems(hs, g->nhs);

	if(subhints) {
		if (ISDBG(SUBSTEMS))
			fprintf(pfa_file, "%% %s: joining subst horizontal stems\n", g->name);
		joinsubstems(hs, hs_pairs, g->nhs, 1);
		uniformstems(hs, hs_pairs, g->nhs);

		if (ISDBG(SUBSTEMS))
			fprintf(pfa_file, "%% %s: joining subst vertical stems\n", g->name);
		joinsubstems(vs, vs_pairs, g->nvs, 0);

		groupsubstems(g, hs, hs_pairs, g->nhs, vs, vs_pairs, g->nvs);
	}

	if (ISDBG(MAINSTEMS))
		fprintf(pfa_file, "%% %s: joining main horizontal stems\n", g->name);
	g->nhs = joinmainstems(hs, g->nhs, 1);
	if (ISDBG(MAINSTEMS))
		fprintf(pfa_file, "%% %s: joining main vertical stems\n", g->name);
	g->nvs = joinmainstems(vs, g->nvs, 0);

	if (ISDBG(MAINSTEMS))
		debugstems(g->name, hs, g->nhs, vs, g->nvs);

	if(g->nhs > 0) {
		if ((sp = malloc(sizeof(STEM) * g->nhs)) == 0) {
			fprintf(stderr, "**** not enough memory for hints ****\n");
			exit(255);
		}
		g->hstems = sp;
		memcpy(sp, hs, sizeof(STEM) * g->nhs);
	} else
		g->hstems = 0;

	if(g->nvs > 0) {
		if ((sp = malloc(sizeof(STEM) * g->nvs)) == 0) {
			fprintf(stderr, "**** not enough memory for hints ****\n");
			exit(255);
		}
		g->vstems = sp;
		memcpy(sp, vs, sizeof(STEM) * g->nvs);
	} else
		g->vstems = 0;

	/* now check that the stems won't overflow the interpreter's stem stack:
	 * some interpreters (like X11) push the stems on each change into
	 * stack and pop them only after the whole glyphs is completed.
	 */

	totals = (g->nhs+g->nvs) / 2; /* we count whole stems, not halves */
	lastgrp = -1;

	for (ge = g->entries; ge != 0; ge = ge->next) {
		grp=ge->stemid;
		if(grp >= 0 && grp != lastgrp)  {
			if(grp==0)
				totals += g->nsbs[0];
			else
				totals += g->nsbs[grp] - g->nsbs[grp-1];

			lastgrp = grp;
		}
	}

	/* be on the safe side, check for >= , not > */
	if(totals >= max_stemdepth) {  /* oops, too deep */
		WARNING_2 {
			fprintf(stderr, "Warning: glyph %s needs hint stack depth %d\n", g->name, totals);
			fprintf(stderr, "  (limit %d): removed the substituted hints from it\n", max_stemdepth);
		}
		if(g->nsg > 0) {
			for (ge = g->entries; ge != 0; ge = ge->next)
				ge->stemid = -1;
			free(g->sbstems); g->sbstems = 0;
			free(g->nsbs); g->nsbs = 0;
			g->nsg = 0;
		}
	}

	/* now check if there are too many main stems */
	totals = (g->nhs+g->nvs) / 2; /* we count whole stems, not halves */
	if(totals >= max_stemdepth) { 
		/* even worse, too much of non-substituted stems */
		WARNING_2 {
			fprintf(stderr, "Warning: glyph %s has %d main hints\n", g->name, totals);
			fprintf(stderr, "  (limit %d): removed the hints from it\n", max_stemdepth);
		}
		if(g->vstems) {
			free(g->vstems); g->vstems = 0; g->nvs = 0;
		}
		if(g->hstems) {
			free(g->hstems); g->hstems = 0; g->nhs = 0;
		}
	}
}

/* convert weird curves that are close to lines into lines.
*/

void
fstraighten(
	   GLYPH * g
)
{
	GENTRY         *ge, *pge, *nge, *ige;
	double          df;
	int             dir;
	double          iln, oln;
	int             svdir, i, o;

	for (ige = g->entries; ige != 0; ige = ige->next) {
		if (ige->type != GE_CURVE)
			continue;

		ge = ige;
		pge = ge->bkwd;
		nge = ge->frwd;

		df = 0.;

		/* look for vertical then horizontal */
		for(i=0; i<2; i++) {
			o = !i; /* other axis */

			iln = fabs(ge->fpoints[i][2] - pge->fpoints[i][2]);
			oln = fabs(ge->fpoints[o][2] - pge->fpoints[o][2]);
			/*
			 * if current curve is almost a vertical line, and it
			 * doesn't begin or end horizontally (and the prev/next
			 * line doesn't join smoothly ?)
			 */
			if( oln < 1.
			|| ge->fpoints[o][2] == ge->fpoints[o][1] 
			|| ge->fpoints[o][0] == pge->fpoints[o][2]
			|| iln > 2.
			|| iln > 1.  && iln/oln > 0.1 )
				continue;


			if(ISDBG(STRAIGHTEN)) 
				fprintf(stderr,"** straighten almost %s\n", (i? "horizontal":"vertical"));

			df = ge->fpoints[i][2] - pge->fpoints[i][2];
			dir = fsign(ge->fpoints[o][2] - pge->fpoints[o][2]);
			ge->type = GE_LINE;

			/*
			 * suck in all the sequence of such almost lines
			 * going in the same direction but not deviating
			 * too far from vertical
			 */
			iln = fabs(nge->fpoints[i][2] - ge->fpoints[i][2]);
			oln = nge->fpoints[o][2] - ge->fpoints[o][2];

			while (fabs(df) <= 5 && nge->type == GE_CURVE
			&& dir == fsign(oln) /* that also gives oln != 0 */
			&& iln <= 2.
			&& ( iln <= 1.  || iln/fabs(oln) <= 0.1 ) ) {
				ge->fx3 = nge->fx3;
				ge->fy3 = nge->fy3;

				if(ISDBG(STRAIGHTEN))
					fprintf(stderr,"** straighten collapsing %s\n", (i? "horizontal":"vertical"));
				freethisge(nge);
				fixendpath(ge);
				pge = ge->bkwd;
				nge = ge->frwd;

				df = ge->fpoints[i][2] - pge->fpoints[i][2];

				iln = fabs(nge->fpoints[i][2] - ge->fpoints[i][2]);
				oln = nge->fpoints[o][2] - ge->fpoints[o][2];
			}

			/* now check what do we have as previous/next line */

			if(ge != pge) { 
				if( pge->type == GE_LINE && pge->fpoints[i][2] == pge->prev->fpoints[i][2]
				&& fabs(pge->fpoints[o][2] != pge->prev->fpoints[o][2]) ) {
					if(ISDBG(STRAIGHTEN)) fprintf(stderr,"** straighten join with previous 0x%x 0x%x\n", pge, ge);
					/* join the previous line with current */
					pge->fx3 = ge->fx3;
					pge->fy3 = ge->fy3;

					ige = freethisge(ge)->prev; /* keep the iterator valid */
					ge = pge;
					fixendpath(ge);
					pge = ge->bkwd;
				}
			}

			if(ge != nge) { 
				if (nge->type == GE_LINE && nge->fpoints[i][2] == ge->fpoints[i][2]
				&& fabs(nge->fpoints[o][2] != ge->fpoints[o][2]) ) {
					if(ISDBG(STRAIGHTEN)) fprintf(stderr,"** straighten join with next 0x%x 0x%x\n", ge, nge);
					/* join the next line with current */
					ge->fx3 = nge->fx3;
					ge->fy3 = nge->fy3;

					freethisge(nge);
					fixendpath(ge);
					pge = ge->bkwd;
					nge = ge->frwd;

				}
			}

			if(ge != pge) { 
				/* try to align the lines if neccessary */
				if(df != 0.)
					fclosegap(ge, ge, i, df, NULL);
			} else {
				/* contour consists of only one line, get rid of it */
				ige = freethisge(ge)->prev; /* keep the iterator valid */
			}

			break; /* don't bother looking at the other axis */
		}
	}
}

/* solve a square equation,
 * returns the number of solutions found, the solutions
 * are stored in res which should point to array of two doubles.
 * min and max limit the area for solutions
 */

static int
fsqequation(
	double a,
	double b,
	double c,
	double *res,
	double min,
	double max
)
{
	double D;
	int n;

	if(ISDBG(SQEQ)) fprintf(stderr, "sqeq(%g,%g,%g) [%g;%g]\n", a, b, c, min, max);

	if(fabs(a) < 0.000001) { /* if a linear equation */
		n=0;
		if(fabs(b) < 0.000001) /* not an equation at all */
			return 0;
		res[0] = -c/b;
		if(ISDBG(SQEQ)) fprintf(stderr, "sqeq: linear t=%g\n", res[0]);
		if(res[0] >= min && res[0] <= max)
			n++;
		return n;
	}

	D = b*b - 4.0*a*c;
	if(ISDBG(SQEQ)) fprintf(stderr, "sqeq: D=%g\n", D);
	if(D<0)
		return 0;

	D = sqrt(D);

	n=0;
	res[0] = (-b+D) / (2*a);
	if(ISDBG(SQEQ)) fprintf(stderr, "sqeq: t1=%g\n", res[0]);
	if(res[0] >= min && res[0] <= max)
		n++;

	res[n] = (-b-D) / (2*a);
	if(ISDBG(SQEQ)) fprintf(stderr, "sqeq: t2=%g\n", res[n]);
	if(res[n] >= min && res[n] <= max)
		n++;

	/* return 2nd solution only if it's different enough */
	if(n==2 && fabs(res[0]-res[1])<0.000001)
		n=1;

	return n;
}

/* check that the curves don't cross quadrant boundary */
/* (float) */

/*
  Here we make sure that the curve does not continue past
  horizontal or vertical extremums. The horizontal points are
  explained, vertical points are by analogy.

  The horizontal points are where the derivative
  dy/dx is equal to 0. But the Bezier curves are defined by
  parametric formulas
   x=fx(t)
   y=fy(t)
  so finding this derivative is complicated.
  Also even if we find some point (x,y) splitting at this point
  is far not obvious. Fortunately we can use dy/dt = 0 instead,
  this gets to a rather simple square equation and splitting
  at a known value of t is simple.

  The formulas are:

  y = A*(1-t)^3 + 3*B*(1-t)^2*t + 3*C*(1-t)*t^2 + D*t^3
  y = (-A+3*B-3*C+D)*t^3 + (3*A-6*B+3*C)*t^2 + (-3*A+3*B)*t + A
  dy/dt = 3*(-A+3*B-3*C+D)*t^2 + 2*(3*A-6*B+3*C)*t + (-3*A+3*B)
 */

void
ffixquadrants(
	GLYPH *g
)
{
	GENTRY         *ge, *nge;
	int	i, j, np, oldnp;
	double	sp[5]; /* split points, last one empty */
	char dir[5]; /* for debugging, direction by which split happened */
	double a, b, *pts; /* points of a curve */

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type != GE_CURVE)
			continue;
		
	doagain:
		np = 0; /* no split points yet */
		if(ISDBG(QUAD)) {
			fprintf(stderr, "%s: trying 0x%x (%g %g) (%g %g) (%g %g) (%g %g)\n  ", g->name,
				ge,  ge->prev->fx3, ge->prev->fy3, ge->fx1, ge->fy1, ge->fx2, ge->fy2,
				ge->fx3, ge->fy3);
		}
		for(i=0; i<2; i++) { /* first for x then for y */
			/* find the cooridnates of control points */
			a = ge->prev->fpoints[i][2];
			pts = &ge->fpoints[i][0];

			oldnp = np;
			np += fsqequation(
				3.0*(-a + 3.0*pts[0] - 3.0*pts[1] + pts[2]),
				6.0*(a - 2.0*pts[0] + pts[1]),
				3.0*(-a + pts[0]),
				&sp[np],
				0.0, 1.0); /* XXX range is [0;1] */

			if(np == oldnp)
				continue;

			if(ISDBG(QUAD))
				fprintf(stderr, "%s: 0x%x: %d pts(%c): ", 
					g->name, ge, np-oldnp, i? 'y':'x');

			/* remove points that are too close to the ends 
			 * because hor/vert ends are permitted, also
			 * if the split point is VERY close to the ends
			 * but not exactly then just flatten it and check again.
			 */
			for(j = oldnp; j<np; j++) {
				dir[j] = i;
				if(ISDBG(QUAD))
					fprintf(stderr, "%g ", sp[j]);
				if(sp[j] < 0.03) { /* front end of curve */
					if(ge->fpoints[i][0] != ge->prev->fpoints[i][2]) {
						ge->fpoints[i][0] = ge->prev->fpoints[i][2];
						if(ISDBG(QUAD)) fprintf(stderr, "flattened at front\n");
						goto doagain;
					}
					if( ge->fpoints[i][1] != ge->fpoints[i][0]
					&& fsign(ge->fpoints[i][2] - ge->fpoints[i][1])
							!= fsign(ge->fpoints[i][1] - ge->fpoints[i][0]) ) {
						ge->fpoints[i][1] = ge->fpoints[i][0];
						if(ISDBG(QUAD)) fprintf(stderr, "flattened zigzag at front\n");
						goto doagain;
					}
					sp[j] = sp[j+1]; np--; j--;
					if(ISDBG(QUAD)) fprintf(stderr, "(front flat)  ");
				} else if(sp[j] > 0.97) { /* rear end of curve */
					if(ge->fpoints[i][1] != ge->fpoints[i][2]) {
						ge->fpoints[i][1] = ge->fpoints[i][2];
						if(ISDBG(QUAD)) fprintf(stderr, "flattened at rear\n");
						goto doagain;
					}
					if( ge->fpoints[i][0] != ge->fpoints[i][1]
					&& fsign(ge->prev->fpoints[i][2] - ge->fpoints[i][0])
							!= fsign(ge->fpoints[i][0] - ge->fpoints[i][1]) ) {
						ge->fpoints[i][0] = ge->fpoints[i][1];
						if(ISDBG(QUAD)) fprintf(stderr, "flattened zigzag at rear\n");
						goto doagain;
					}
					sp[j] = sp[j+1]; np--; j--;
					if(ISDBG(QUAD)) fprintf(stderr, "(rear flat)  ");
				} 
			}
			if(ISDBG(QUAD)) fprintf(stderr, "\n");
		}

		if(np==0) /* no split points, leave it alone */
			continue;

		if(ISDBG(QUAD)) {
			fprintf(stderr, "%s: splitting 0x%x (%g %g) (%g %g) (%g %g) (%g %g) at %d points\n  ", g->name,
				ge,  ge->prev->fx3, ge->prev->fy3, ge->fx1, ge->fy1, ge->fx2, ge->fy2,
				ge->fx3, ge->fy3, np);
			for(i=0; i<np; i++)
				fprintf(stderr, "%g(%c) ", sp[i], dir[i] ? 'y':'x');
			fprintf(stderr, "\n");
		}

		/* sort the points ascending */
		for(i=0; i<np; i++)
			for(j=i+1; j<np; j++)
				if(sp[i] > sp[j]) {
					a = sp[i]; sp[i] = sp[j]; sp[j] = a;
				}

		/* now finally do the split on each point */
		for(j=0; j<np; j++) {
			double k1, k2, c;

			k1 = sp[j];
			k2 = 1 - k1;

			if(ISDBG(QUAD)) fprintf(stderr, "   0x%x %g/%g\n", ge, k1, k2);

			nge = newgentry(GEF_FLOAT);
			(*nge) = (*ge);

#define SPLIT(pt1, pt2)	( (pt1) + k1*((pt2)-(pt1)) ) /* order is important! */
			for(i=0; i<2; i++) { /* for x and y */
				a = ge->fpoints[i][0]; /* get the middle points */
				b = ge->fpoints[i][1];

				/* calculate new internal points */
				c = SPLIT(a, b);

				ge->fpoints[i][0] = SPLIT(ge->prev->fpoints[i][2], a);
				ge->fpoints[i][1] = SPLIT(ge->fpoints[i][0], c);

				nge->fpoints[i][1] = SPLIT(b, nge->fpoints[i][2]);
				nge->fpoints[i][0] = SPLIT(c, nge->fpoints[i][1]);

				ge->fpoints[i][2] = SPLIT(ge->fpoints[i][1],
					+ nge->fpoints[i][0]);
			}
#undef SPLIT

			addgeafter(ge, nge);

			/* go to the next part, adjust remaining points */
			ge = nge;
			for(i=j+1; i<np; i++)
				sp[i] = (sp[i]-k1) / k2;
		}
	}

}

/* check if a curve is a zigzag */

static int
iiszigzag(
	GENTRY *ge
) 
{
	double          k, k1, k2;
	double          a, b;

	if (ge->type != GE_CURVE)
		return 0;

	a = ge->iy2 - ge->iy1;
	b = ge->ix2 - ge->ix1;
	k = fabs(a == 0 ? (b == 0 ? 1. : FBIGVAL) : (double) b / (double) a);
	a = ge->iy1 - ge->prev->iy3;
	b = ge->ix1 - ge->prev->ix3;
	k1 = fabs(a == 0 ? (b == 0 ? 1. : FBIGVAL) : (double) b / (double) a);
	a = ge->iy3 - ge->iy2;
	b = ge->ix3 - ge->ix2;
	k2 = fabs(a == 0 ? (b == 0 ? 1. : FBIGVAL) : (double) b / (double) a);

	/* if the curve is not a zigzag */
	if (k1 >= k && k2 <= k || k1 <= k && k2 >= k)
		return 0;
	else
		return 1;
}

/* check if a curve is a zigzag - floating */

static int
fiszigzag(
	GENTRY *ge
) 
{
	double          k, k1, k2;
	double          a, b;

	if (ge->type != GE_CURVE)
		return 0;

	a = fabs(ge->fy2 - ge->fy1);
	b = fabs(ge->fx2 - ge->fx1);
	k = a < FEPS ? (b <FEPS ? 1. : FBIGVAL) : b / a;
	a = fabs(ge->fy1 - ge->prev->fy3);
	b = fabs(ge->fx1 - ge->prev->fx3);
	k1 = a < FEPS ? (b < FEPS ? 1. : FBIGVAL) : b / a;
	a = fabs(ge->fy3 - ge->fy2);
	b = fabs(ge->fx3 - ge->fx2);
	k2 = a < FEPS ? (b <FEPS ? 1. : FBIGVAL) : b / a;

	/* if the curve is not a zigzag */
	if (k1 >= k && k2 <= k || k1 <= k && k2 >= k)
		return 0;
	else
		return 1;
}

/* split the zigzag-like curves into two parts */

void
fsplitzigzags(
	     GLYPH * g
)
{
	GENTRY         *ge, *nge;
	double          a, b, c, d;

	assertisfloat(g, "splitting zigzags");
	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type != GE_CURVE)
			continue;

		/* if the curve is not a zigzag */
		if ( !fiszigzag(ge) ) {
			continue;
		}

		/* split the curve by t=0.5 */
		nge = newgentry(GEF_FLOAT);
		(*nge) = (*ge);
		nge->type = GE_CURVE;

		a = ge->prev->fx3;
		b = ge->fx1;
		c = ge->fx2;
		d = ge->fx3;
		nge->fx3 = d;
		nge->fx2 = (c + d) / 2.;
		nge->fx1 = (b + 2. * c + d) / 4.;
		ge->fx3 = (a + b * 3. + c * 3. + d) / 8.;
		ge->fx2 = (a + 2. * b + c) / 4.;
		ge->fx1 = (a + b) / 2.;

		a = ge->prev->fy3;
		b = ge->fy1;
		c = ge->fy2;
		d = ge->fy3;
		nge->fy3 = d;
		nge->fy2 = (c + d) / 2.;
		nge->fy1 = (b + 2. * c + d) / 4.;
		ge->fy3 = (a + b * 3. + c * 3. + d) / 8.;
		ge->fy2 = (a + 2. * b + c) / 4.;
		ge->fy1 = (a + b) / 2.;

		addgeafter(ge, nge);
	}
}

/* free this GENTRY, returns what was ge->next
 * (ge must be of type GE_LINE or GE_CURVE)
 * works on both float and int entries
 */

static GENTRY *
freethisge(
	GENTRY *ge
)
{
	GENTRY *xge;

	if (ge->bkwd != ge->prev) {
		/* at beginning of the contour */

		xge = ge->bkwd;
		if(xge == ge) { /* was the only line in contour */
			/* remove the contour completely */
			/* prev is GE_MOVE, next is GE_PATH, remove them all */

			/* may be the first contour, then ->bkwd points to ge->entries */
			if(ge->prev->prev == 0)
				*(GENTRY **)(ge->prev->bkwd) = ge->next->next;
			else
				ge->prev->prev->next = ge->next->next;

			if(ge->next->next) {
				ge->next->next->prev = ge->prev->prev;
				ge->next->next->bkwd = ge->prev->bkwd;
			}

			xge = ge->next->next;
			free(ge->prev); free(ge->next); free(ge);
			return xge;
		}

		/* move the start point of the contour */
		if(ge->flags & GEF_FLOAT) {
			ge->prev->fx3 = xge->fx3;
			ge->prev->fy3 = xge->fy3;
		} else {
			ge->prev->ix3 = xge->ix3;
			ge->prev->iy3 = xge->iy3;
		}
	} else if(ge->frwd != ge->next) {
		/* at end of the contour */

		xge = ge->frwd->prev;
		/* move the start point of the contour */
		if(ge->flags & GEF_FLOAT) {
			xge->fx3 = ge->bkwd->fx3;
			xge->fy3 = ge->bkwd->fy3;
		} else {
			xge->ix3 = ge->bkwd->ix3;
			xge->iy3 = ge->bkwd->iy3;
		}
	}

	ge->prev->next = ge->next;
	ge->next->prev = ge->prev;
	ge->bkwd->frwd = ge->frwd;
	ge->frwd->bkwd = ge->bkwd;

	xge = ge->next;
	free(ge);
	return xge;
}

/* inserts a new gentry (LINE or CURVE) after another (MOVE
 * or LINE or CURVE)
 * corrects the first GE_MOVE if neccessary
 */

static void
addgeafter(
	GENTRY *oge, /* after this */
	GENTRY *nge /* insert this */
)
{
	if(oge->type == GE_MOVE) {
		/* insert before next */
		if(oge->next->type == GE_PATH) {
			/* first and only GENTRY in path */
			nge->frwd = nge->bkwd = nge;
		} else {
			nge->frwd = oge->next;
			nge->bkwd = oge->next->bkwd;
			oge->next->bkwd->frwd = nge;
			oge->next->bkwd = nge;
		}
	} else {
		nge->frwd = oge->frwd;
		nge->bkwd = oge;
		oge->frwd->bkwd = nge;
		oge->frwd = nge;
	}

	nge->next = oge->next;
	nge->prev = oge;
	oge->next->prev = nge;
	oge->next = nge;

	if(nge->frwd->prev->type == GE_MOVE) {
		/* fix up the GE_MOVE entry */
		if(nge->flags & GEF_FLOAT) {
			nge->frwd->prev->fx3 = nge->fx3;
			nge->frwd->prev->fy3 = nge->fy3;
		} else {
			nge->frwd->prev->ix3 = nge->ix3;
			nge->frwd->prev->iy3 = nge->iy3;
		}
	}
}

/*
 * Check if this GENTRY happens to be at the end of path
 * and fix the first MOVETO accordingly
 * handles both int and float
 */

static void
fixendpath(
	GENTRY *ge
)
{
	GENTRY *mge;

	mge = ge->frwd->prev;
	if(mge->type == GE_MOVE) {
		if(ge->flags & GEF_FLOAT) {
			mge->fx3 = ge->fx3;
			mge->fy3 = ge->fy3;
		} else {
			mge->ix3 = ge->ix3;
			mge->iy3 = ge->iy3;
		}
	}
}

/*
 * This function adjusts the rest of path (the part from...to is NOT changed)
 * to cover the specified gap by the specified axis (0 - X, 1 - Y).
 * Gap is counted in direction (end_of_to - beginning_of_from).
 * Returns by how much the gap was not closed (0.0 if it was fully closed).
 * Ret contains by how much the first and last points of [from...to]
 * were moved to bring them in consistence to the rest of the path.
 * If ret==NULL then this info is not returned.
 */

static double
fclosegap(
	GENTRY *from,
	GENTRY *to,
	int axis,
	double gap,
	double *ret
)
{
#define TIMESLARGER 10.	/* how many times larger must be a curve to not change too much */
	double rm[2];
	double oldpos[2];
	double times, limit, df, dx;
	int j, k;
	GENTRY *xge, *pge, *nge, *bge[2];

	/* remember the old points to calculate ret */
	oldpos[0] = from->prev->fpoints[axis][2];
	oldpos[1] = to->fpoints[axis][2];

	rm[0] = rm[1] = gap / 2. ;

	bge[0] = from; /* this is convenient for iterations */
	bge[1] = to;

	/* first try to modify large curves but if have none then settle for small */
	for(times = (TIMESLARGER-1); times > 0.1; times /= 2. ) {

		if(rm[0]+rm[1] == 0.)
			break;

		/* iterate in both directions, backwards then forwards */
		for(j = 0; j<2; j++) {

			if(rm[j] == 0.) /* if this direction is exhausted */
				continue;

			limit = fabs(rm[j]) * (1.+times);

			for(xge = bge[j]->cntr[j]; xge != bge[!j]; xge = xge->cntr[j]) {
				dx = xge->fpoints[axis][2] - xge->prev->fpoints[axis][2];
				df = fabs(dx) - limit;
				if( df <= FEPS ) /* curve is too small to change */
					continue;

				if( df >= fabs(rm[j]) )
					df = rm[j];
				else 
					df *= fsign(rm[j]); /* we may cover this part of rm */

				rm[j] -= df;
				limit = fabs(rm[j]) * (1.+times);

				if(xge->type == GE_CURVE) { /* correct internal points */
					double scale = ((dx+df) / dx) - 1.;
					double base;

					if(j)
						base = xge->fpoints[axis][2];
					else
						base = xge->prev->fpoints[axis][2];

					for(k = 0; k<2; k++)
						xge->fpoints[axis][k] += scale * 
							(xge->fpoints[axis][k] - base);
				}

				/* move all the intermediate lines */
				if(j) {
					df = -df; /* absolute direction */
					pge = bge[1]->bkwd;
					nge = xge->bkwd;
				} else {
					xge->fpoints[axis][2] += df;
					pge = bge[0];
					nge = xge->frwd;
				}
				while(nge != pge) {
					if(nge->type == GE_CURVE) {
						nge->fpoints[axis][0] +=df;
						nge->fpoints[axis][1] +=df;
					}
					nge->fpoints[axis][2] += df;
					if(nge->next != nge->frwd) { /* last entry of contour */
						nge->frwd->prev->fpoints[axis][2] += df;
					}
					nge = nge->cntr[!j];
				}

				if(rm[j] == 0.)
					break;
			}
		}
	}

	/* find the difference */
	oldpos[0] -= from->prev->fpoints[axis][2];
	oldpos[1] -= to->fpoints[axis][2];

	if(ret) {
		ret[0] = oldpos[0] - from->prev->fpoints[axis][2];
		ret[1] = oldpos[1] - to->fpoints[axis][2];
	}

#if 0
	if( rm[0]+rm[1] != gap - oldpos[1] + oldpos[0]) {
		fprintf(stderr, "** gap=%g rm[0]=%g rm[1]=%g o[0]=%g o[1]=%g rg=%g og=%g\n",
			gap, rm[0], rm[1], oldpos[0], oldpos[1], rm[0]+rm[1], 
			gap - oldpos[1] + oldpos[0]);
	}
#endif

	return rm[0]+rm[1];
#undef TIMESLARGER
}

/* remove the lines or curves smaller or equal to the size limit */

static void
fdelsmall(
	GLYPH *g,
	double minlen
)
{
	GENTRY  *ge, *nge, *pge, *xge, *next;
	int i, k;
	double dx, dy, d2, d2m;
	double minlen2;
#define TIMESLARGER 10.	/* how much larger must be a curve to not change too much */

	minlen2 = minlen*minlen;

	for (ge = g->entries; ge != 0; ge = next) {
		next = ge->next;

		if (ge->type != GE_CURVE && ge->type != GE_LINE)
			continue;

		d2m = 0;
		for(i= (ge->type==GE_CURVE? 0: 2); i<3; i++) {
			dx = ge->fxn[i] - ge->prev->fx3;
			dy = ge->fyn[i] - ge->prev->fy3;
			d2 = dx*dx + dy*dy;
			if(d2m < d2)
				d2m = d2;
		}

		if( d2m > minlen2 ) { /* line is not too small */
			/* XXX add more normalization here */
			continue;
		}

		/* if the line is too small */

		/* check forwards if we have a whole sequence of them */
		nge = ge;
		for(xge = ge->frwd; xge != ge; xge = xge->frwd) {
			d2m = 0;
			for(i= (xge->type==GE_CURVE? 0: 2); i<3; i++) {
				dx = xge->fxn[i] - xge->prev->fx3;
				dy = xge->fyn[i] - xge->prev->fy3;
				d2 = dx*dx + dy*dy;
				if(d2m < d2)
					d2m = d2;
			}
			if( d2m > minlen2 ) /* line is not too small */
				break;
			nge = xge;
			if(next == nge) /* move the next step past this sequence */
				next = next->next;
		}

		/* check backwards if we have a whole sequence of them */
		pge = ge;
		for(xge = ge->bkwd; xge != ge; xge = xge->bkwd) {
			d2m = 0;
			for(i= (xge->type==GE_CURVE? 0: 2); i<3; i++) {
				dx = xge->fxn[i] - xge->prev->fx3;
				dy = xge->fyn[i] - xge->prev->fy3;
				d2 = dx*dx + dy*dy;
				if(d2m < d2)
					d2m = d2;
			}
			if( d2m > minlen2 ) /* line is not too small */
				break;
			pge = xge;
		}

		/* now we have a sequence of small fragments in pge...nge (inclusive) */

		if(ISDBG(FCONCISE))  {
			fprintf(stderr, "glyph %s has very small fragments(%x..%x..%x)\n", 
			g->name, pge, ge, nge);
			dumppaths(g, pge, nge);
		}

		/* reduce whole sequence to one part and remember the middle point */
		if(pge != nge) {
			while(1) {
				xge = pge->frwd;
				if(xge == nge) {
					pge->fx1 = pge->fx2 = pge->fx3;
					pge->fx3 = nge->fx3;
					pge->fy1 = pge->fy2 = pge->fy3;
					pge->fy3 = nge->fy3;
					pge->type = GE_CURVE;
					freethisge(nge);
					break;
				}
				if(xge == nge->bkwd) {
					pge->fx1 = pge->fx2 = (pge->fx3+xge->fx3)/2.;
					pge->fx3 = nge->fx3;
					pge->fy1 = pge->fy2 = (pge->fy3+xge->fy3)/2.;
					pge->fy3 = nge->fy3;
					pge->type = GE_CURVE;
					freethisge(nge);
					freethisge(xge);
					break;
				}
				freethisge(pge); pge = xge;
				xge = nge->bkwd; freethisge(nge); nge = xge;
			}
		}
		ge = pge;

		/* check if the whole sequence is small */
		dx = ge->fx3 - ge->prev->fx3;
		dy = ge->fy3 - ge->prev->fy3;
		d2 = dx*dx + dy*dy;

		if( d2 > minlen2 ) { /* no, it is not */
			double b, d;

			WARNING_3 fprintf(stderr, "glyph %s had a sequence of fragments < %g points each, reduced to one curve\n",
				g->name, minlen);

			/* check that we did not create a monstrosity spanning quadrants */
			if(fsign(ge->fx1 - ge->prev->fx1) * fsign(ge->fx3 - ge->fx1) < 0
			|| fsign(ge->fy1 - ge->prev->fy1) * fsign(ge->fy3 - ge->fy1) < 0 ) { 
				/* yes, we did; are both parts of this thing big enough ? */
				dx = ge->fx1 - ge->prev->fx3;
				dy = ge->fy1 - ge->prev->fy3;
				d2 = dx*dx + dy*dy;

				dx = ge->fx3 - ge->fx1;
				dy = ge->fy3 - ge->fy1;
				d2m = dx*dx + dy*dy;

				if(d2 > minlen2 && d2m > minlen2) { /* make two straights */
					nge = newgentry(GEF_FLOAT);
					*nge = *ge;
					
					for(i=0; i<2; i++) {
						ge->fpoints[i][2] = ge->fpoints[i][0];
						b = nge->fpoints[i][0];
						d = nge->fpoints[i][2] - b;
						nge->fpoints[i][0] = b + 0.1*d;
						nge->fpoints[i][1] = b + 0.9*d;
					}
				}
				for(i=0; i<2; i++) { /* make one straight or first of two straights */
					b = ge->prev->fpoints[i][2];
					d = ge->fpoints[i][2] - b;
					ge->fpoints[i][0] = b + 0.1*d;
					ge->fpoints[i][1] = b + 0.9*d;
				}
			}
			continue; 
		}

		if(ge->frwd == ge) { /* points to itself, just remove the path completely */
			WARNING_3 fprintf(stderr, "glyph %s had a path made of fragments < %g points each, removed\n",
				g->name, minlen);

			next = freethisge(ge);
			continue;
		} 

		/* now close the gap by x and y */
		for(i=0; i<2; i++) {
			double gap;

			gap = ge->fpoints[i][2] - ge->prev->fpoints[i][2];
			if( fclosegap(ge, ge, i, gap, NULL) != 0.0 ) {
				double scale, base;

				/* not good, as the last resort just scale the next line */
				gap = ge->fpoints[i][2] - ge->prev->fpoints[i][2];

				if(ISDBG(FCONCISE)) 
					fprintf(stderr, "    last resort on %c: closing next by %g\n",
					(i==0 ? 'x' : 'y'), gap);

				nge = ge->frwd;
				base = nge->fpoints[i][2];
				dx = ge->fpoints[i][2] - base;
				if(fabs(dx) < FEPS)
					continue;

				scale = ((dx-gap) / dx);

				if(nge->type == GE_CURVE)
					for(k = 0; k<2; k++)
						nge->fpoints[i][k] = base + 
							scale * (nge->fpoints[i][k] - base);

				ge->fpoints[i][2] -= gap;
			}
		}

		/* OK, the gap is closed - remove this useless GENTRY */
		freethisge(ge);
	}
#undef TIMESLARGER
}

/* normalize curves to the form where their ends
 * can be safely used as derivatives
 */

static void
fnormalizec(
	     GLYPH * g
)
{
	GENTRY *ge;
	int midsame, frontsame, rearsame, i;
	double d, b;

	assertisfloat(g, "normalizing curves");

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type != GE_CURVE)
			continue;

		midsame = (fabs(ge->fx1-ge->fx2)<FEPS && fabs(ge->fy1-ge->fy2)<FEPS);
		frontsame = (fabs(ge->fx1-ge->prev->fx3)<FEPS && fabs(ge->fy1-ge->prev->fy3)<FEPS);
		rearsame = (fabs(ge->fx3-ge->fx2)<FEPS && fabs(ge->fy3-ge->fy2)<FEPS);

		if(midsame && (frontsame || rearsame) ) {
			/* essentially a line */
			for(i=0; i<2; i++) {
				b = ge->prev->fpoints[i][2];
				d = ge->fpoints[i][2] - b;
				ge->fpoints[i][0] = b + 0.1*d;
				ge->fpoints[i][1] = b + 0.9*d;
			}
		} else if(frontsame) {
			for(i=0; i<2; i++) {
				b = ge->prev->fpoints[i][2];
				d = ge->fpoints[i][1] - b;
				ge->fpoints[i][0] = b + 0.01*d;
			}
		} else if(rearsame) {
			for(i=0; i<2; i++) {
				b = ge->fpoints[i][2];
				d = ge->fpoints[i][0] - b;
				ge->fpoints[i][1] = b + 0.01*d;
			}
		} else
			continue;

		if(ISDBG(FCONCISE)) fprintf(stderr, "glyph %g, normalized entry %x\n", g->name, ge);
	}
}

/* find the point where two rays continuing vectors cross
 * rays are defined as beginning of curve1 and end of curve 2
 * returns 1 if they cross, 0 if they don't
 * If they cross returns the maximal scales for both vectors.
 * Expects that the curves are normalized.
 */

static int
fcrossrays(
	GENTRY *ge1,
	GENTRY *ge2,
	double *max1,
	double *max2
)
{
	struct ray {
		double x1, y1, x2, y2;
		int isvert;
		double k, b; /* lines are represented as y = k*x + b */
		double *maxp;
	} ray [3];
	double x, y;
	int i;

	ray[0].x1 = ge1->prev->fx3;
	ray[0].y1 = ge1->prev->fy3;
	ray[0].x2 = ge1->fx1;
	ray[0].y2 = ge1->fy1;
	ray[0].maxp = max1;

	ray[1].x1 = ge2->fx3;
	ray[1].y1 = ge2->fy3;
	ray[1].x2 = ge2->fx2;
	ray[1].y2 = ge2->fy2;
	ray[1].maxp = max2;

	for(i=0; i<2; i++) {
		if(ray[i].x1 == ray[i].x2) 
			ray[i].isvert = 1;
		else {
			ray[i].isvert = 0;
			ray[i].k = (ray[i].y2 - ray[i].y1) / (ray[i].x2 - ray[i].x1);
			ray[i].b = ray[i].y2 - ray[i].k * ray[i].x2;
		}
	}

	if(ray[0].isvert && ray[1].isvert) {
		if(ISDBG(FCONCISE)) fprintf(stderr, "crossrays: both vertical\n");
		return 0; /* both vertical, don't cross */
	}

	if(ray[1].isvert) {
		ray[2] = ray[0]; /* exchange them */
		ray[0] = ray[1];
		ray[1] = ray[2];
	}

	if(ray[0].isvert) {
		x = ray[0].x1;
	} else {
		if( fabs(ray[0].k - ray[1].k) < FEPS) {
			if(ISDBG(FCONCISE)) fprintf(stderr, "crossrays: parallel lines, k = %g, %g\n",
				ray[0].k, ray[1].k);
			return 0; /* parallel lines */
		}
		x = (ray[1].b - ray[0].b) / (ray[0].k - ray[1].k) ;
	}
	y = ray[1].k * x + ray[1].b;

	for(i=0; i<2; i++) {
		if(ray[i].isvert)
			*ray[i].maxp = (y - ray[i].y1) / (ray[i].y2 - ray[i].y1);
		else
			*ray[i].maxp = (x - ray[i].x1) / (ray[i].x2 - ray[i].x1);
		/* check if wrong sides of rays cross */
		if( *ray[i].maxp < 0 ) {
			if(ISDBG(FCONCISE)) fprintf(stderr, "crossrays: scale=%g @(%g,%g) (%g,%g)<-(%g,%g)\n",
				*ray[i].maxp, x, y, ray[i].x2, ray[i].y2, ray[i].x1, ray[i].y1);
			return 0;
		}
	}
	return 1;
}

/* find the area covered by the curve
 * (limited by the projections to the X axis)
 */

static double
fcvarea(
	GENTRY *ge
)
{
	double Ly, My, Ny, Py, Qx, Rx, Sx;
	double area;

	/* y = Ly*t^3 + My*t^2 + Ny*t + Py */
	Ly = -ge->prev->fy3 + 3*(ge->fy1 - ge->fy2) + ge->fy3;
	My = 3*ge->prev->fy3 - 6*ge->fy1 + 3*ge->fy2;
	Ny = 3*(-ge->prev->fy3 + ge->fy1);
	Py = ge->prev->fy3;

	/* dx/dt = Qx*t^2 + Rx*t + Sx */
	Qx = 3*(-ge->prev->fx3 + 3*(ge->fx1 - ge->fx2) + ge->fx3);
	Rx = 6*(ge->prev->fx3 - 2*ge->fx1 + ge->fx2);
	Sx = 3*(-ge->prev->fx3 + ge->fx1);

	/* area is integral[from 0 to 1]( y(t) * dx(t)/dt *dt) */
	area = 1./6.*(Ly*Qx) + 1./5.*(Ly*Rx + My*Qx) 
		+ 1./4.*(Ly*Sx + My*Rx + Ny*Qx) + 1./3.*(My*Sx + Ny*Rx + Py*Qx)
		+ 1./2.*(Ny*Sx + Py*Rx) + Py*Sx;

	return area;
}

/* find the value of point on the curve at the given parameter t,
 * along the given axis (0 - X, 1 - Y).
 */

static double
fcvval(
	GENTRY *ge,
	int axis,
	double t
)
{
	double t2, mt, mt2;

	/* val = A*(1-t)^3 + 3*B*(1-t)^2*t + 3*C*(1-t)*t^2 + D*t^3 */
	t2 = t*t;
	mt = 1-t;
	mt2 = mt*mt;
	
	return ge->prev->fpoints[axis][2]*mt2*mt 
		+ 3*(ge->fpoints[axis][0]*mt2*t + ge->fpoints[axis][1]*mt*t2)
		+ ge->fpoints[axis][2]*t*t2;
}

/* Check that the new curve has the point identified by the 
 * parameter t reasonably close to the corresponding point
 * in the old pair of curves which were joined in proportion k.
 * If old2 is NULL then just compare nge and old1 at the point t.
 * Returns 0 if OK, 1 if it's too far.
 */

static int
fckjoinedcv(
	GLYPH *g,
	double t,
	GENTRY *nge,
	GENTRY *old1,
	GENTRY *old2,
	double k
)
{
	GENTRY *oge;
	double ot;
	double off;
	double lim;
	int i;

	if(old2 == 0) {
		oge = old1;
		ot = t;
	} else if(t <= k && k!=0.) {
		oge = old1;
		ot = t/k;
	} else {
		oge = old2;
		ot = (t-k) / (1.-k);
	}

	if(ISDBG(FCONCISE))
		fprintf(stderr, "%s: t=%g ot=%g (%x) ", g->name, t, ot, oge);

	for(i=0; i<2; i++) {
		/* permitted tolerance is 5% */
		lim = fabs(nge->fpoints[i][2] - nge->prev->fpoints[i][2])*0.05;

		if(lim < 3.)
			lim = 3.; /* for small curves the tolerance is higher */
		if(lim > 10.)
			lim = 10.; /* for big curves the tolerance is limited anyway */

		off = fabs(fcvval(nge, i, t) - fcvval(oge, i, ot));

		if(off > lim) {
			if(ISDBG(FCONCISE))
				fprintf(stderr, "out of range d%c=%.2f(%.2f)\n", 
					(i==0 ? 'X' : 'Y'), off, lim);
			return 1;
		}

		if(ISDBG(FCONCISE))
			fprintf(stderr, "valid d%c=%.2f(%.2f)  ", (i==0 ? 'X' : 'Y'), off, lim);
	}
	if(ISDBG(FCONCISE))
		fprintf(stderr, "\n");
	return 0;
}

/* force conciseness: substitute 2 or more curves going in the
** same quadrant with one curve
** in floating point
*/

void
fforceconcise(
	     GLYPH * g
)
{
	GENTRY         *ge, *nge;
	GENTRY          tge;
	double          firstlen, lastlen, sumlen, scale;
	double          dxw1, dyw1, dxw2, dyw2;
	double          dxb1, dyb1, dxe1, dye1;
	double          dxb2, dyb2, dxe2, dye2;
	double          maxsc1, maxsc2;
	int             i;

	assertisfloat(g, "enforcing conciseness");

	fdelsmall(g, 0.05);
	assertpath(g->entries, __FILE__, __LINE__, g->name);
	fnormalizec(g);


	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type != GE_CURVE)
			continue;

		/* the whole direction of curve */
		dxw1 = ge->fx3 - ge->prev->fx3;
		dyw1 = ge->fy3 - ge->prev->fy3;

		while (1) {
			/* the whole direction of curve */
			dxw1 = ge->fx3 - ge->prev->fx3;
			dyw1 = ge->fy3 - ge->prev->fy3;

			/* directions of  ends of curve */
			dxb1 = ge->fx1 - ge->prev->fx3;
			dyb1 = ge->fy1 - ge->prev->fy3;
			dxe1 = ge->fx3 - ge->fx2;
			dye1 = ge->fy3 - ge->fy2;

			nge = ge->frwd;

			if (nge->type != GE_CURVE)
				break;

			dxw2 = nge->fx3 - ge->fx3;
			dyw2 = nge->fy3 - ge->fy3;

			dxb2 = nge->fx1 - ge->fx3;
			dyb2 = nge->fy1 - ge->fy3;
			dxe2 = nge->fx3 - nge->fx2;
			dye2 = nge->fy3 - nge->fy2;

			/* if curve changes direction */
			if (fsign(dxw1) != fsign(dxw2) || fsign(dyw1) != fsign(dyw2))
				break;

			/* if the arch is going in other direction */
			if (fsign(fabs(dxb1 * dyw1) - fabs(dyb1 * dxw1))
			    * fsign(fabs(dxe2 * dyw2) - fabs(dye2 * dxw2)) > 0)
				break;

			/* get possible scale limits within which we won't cross quadrants */
			if( fcrossrays(ge, nge, &maxsc1, &maxsc2) == 0 ) {
				if(ISDBG(FCONCISE)) {
					fprintf(stderr, "glyph %s has curves with strange ends\n", g->name);
					dumppaths(g, ge, nge);
				}
				break;
			}

			if(maxsc1 < 1. || maxsc2 < 1. ) /* would create a zigzag */
				break;

			ge->dir = fgetcvdir(ge);
			nge->dir = fgetcvdir(nge);

			if( ((ge->dir&CVDIR_FRONT)-CVDIR_FEQUAL) * ((nge->dir&CVDIR_REAR)-CVDIR_REQUAL) < 0 )
				/* would create a zigzag */
				break;

			firstlen = sqrt( dxe1*dxe1 + dye1*dye1 );
			lastlen = sqrt( dxb2*dxb2 + dyb2*dyb2 );
			sumlen = firstlen + lastlen;

			/* check the scale limits */
			if( sumlen/firstlen > maxsc1 || sumlen/lastlen > maxsc2 ) {
				if(ISDBG(FCONCISE)) 
					fprintf(stderr, "%s: %x, %x would be crossing in forceconcise\n", 
					g->name, ge, nge);
				break;
			}

			/* OK, it seems like we can attempt to join these two curves */
			tge.flags = ge->flags;
			tge.prev = ge->prev;
			tge.fx1 = ge->fx1;
			tge.fy1 = ge->fy1;
			tge.fx2 = nge->fx2;
			tge.fy2 = nge->fy2;
			tge.fx3 = nge->fx3;
			tge.fy3 = nge->fy3;

			dxb1 = tge.fx1 - tge.prev->fx3;
			dyb1 = tge.fy1 - tge.prev->fy3;
			dxe1 = tge.fx3 - tge.fx2;
			dye1 = tge.fy3 - tge.fy2;

			/* scale the first segment */
			scale = sumlen / firstlen;
			tge.fx1 = tge.prev->fx3 + scale * dxb1;
			tge.fy1 = tge.prev->fy3 + scale * dyb1;

			/* scale the last segment */
			scale = sumlen / lastlen;
			tge.fx2 = tge.fx3 - scale * dxe1;
			tge.fy2 = tge.fy3 - scale * dye1;

			/* now check if we got something sensible */

			/* check if some important points is too far from original */
			scale = firstlen / sumlen;
			{
				double pts[4] = { 0./*will be replaced*/, 0.5, 0.25, 0.75 };
				int i, bad;

				pts[0] = scale;
				bad = 0;

				for(i=0; i<sizeof(pts)/sizeof(pts[0]); i++)
					if(fckjoinedcv(g, pts[i], &tge, ge, nge, scale)) {
						bad = 1;
						break;
					}
				if(bad)
					break;
			}

			/* OK, it looks reasonably, let's apply it */
			if(ISDBG(FCONCISE)) 
				dumppaths(g, ge, nge);

			for(i=0; i<3; i++) {
				ge->fxn[i] = tge.fxn[i];
				ge->fyn[i] = tge.fyn[i];
			}

			freethisge(nge);
		}
	}
}

void
print_glyph(
	   int glyphno
)
{
	GLYPH          *g;
	GENTRY         *ge;
	int             x = 0, y = 0;
	int             i;
	int             grp, lastgrp= -1;

	g = &glyph_list[glyphno];

	fprintf(pfa_file, "/%s { \n", g->name);

	/* consider widths >MAXLEGALWIDTH as bugs */
	if( g->scaledwidth <= MAXLEGALWIDTH ) {
		fprintf(pfa_file, "0 %d hsbw\n", g->scaledwidth);
	} else {
		fprintf(pfa_file, "0 1000 hsbw\n");
		WARNING_2 fprintf(stderr, "glyph %s: width %d seems to be buggy, set to 1000\n",
			g->name, g->scaledwidth);
	}

#if 0
	fprintf(pfa_file, "%% contours: ");
	for (i = 0; i < g->ncontours; i++)
		fprintf(pfa_file, "%s(%d,%d) ", (g->contours[i].direction == DIR_OUTER ? "out" : "in"),
			g->contours[i].xofmin, g->contours[i].ymin);
	fprintf(pfa_file, "\n");

	if (g->rymin < 5000)
		fprintf(pfa_file, "%d lower%s\n", g->rymin, (g->flatymin ? "flat" : "curve"));
	if (g->rymax > -5000)
		fprintf(pfa_file, "%d upper%s\n", g->rymax, (g->flatymax ? "flat" : "curve"));
#endif

	if (g->hstems)
		for (i = 0; i < g->nhs; i += 2) {
			if (g->hstems[i].flags & ST_3) {
				fprintf(pfa_file, "%d %d %d %d %d %d hstem3\n",
					g->hstems[i].value,
				g->hstems[i + 1].value - g->hstems[i].value,
					g->hstems[i + 2].value,
					g->hstems[i + 3].value - g->hstems[i + 2].value,
					g->hstems[i + 4].value,
					g->hstems[i + 5].value - g->hstems[i + 4].value
					);
				i += 4;
			} else {
				fprintf(pfa_file, "%d %d hstem\n", g->hstems[i].value,
				g->hstems[i + 1].value - g->hstems[i].value);
			}
		}

	if (g->vstems)
		for (i = 0; i < g->nvs; i += 2) {
			if (g->vstems[i].flags & ST_3) {
				fprintf(pfa_file, "%d %d %d %d %d %d vstem3\n",
					g->vstems[i].value,
				g->vstems[i + 1].value - g->vstems[i].value,
					g->vstems[i + 2].value,
					g->vstems[i + 3].value - g->vstems[i + 2].value,
					g->vstems[i + 4].value,
					g->vstems[i + 5].value - g->vstems[i + 4].value
					);
				i += 4;
			} else {
				fprintf(pfa_file, "%d %d vstem\n", g->vstems[i].value,
				g->vstems[i + 1].value - g->vstems[i].value);
			}
		}

	for (ge = g->entries; ge != 0; ge = ge->next) {
		if(g->nsg>0) {
			grp=ge->stemid;
			if(grp >= 0 && grp != lastgrp)  {
				fprintf(pfa_file, "%d 4 callsubr\n", grp+g->firstsubr);
				lastgrp=grp;
			}
		}

		switch (ge->type) {
		case GE_MOVE:
			if (absolute)
				fprintf(pfa_file, "%d %d amoveto\n", ge->ix3, ge->iy3);
			else
				rmoveto(ge->ix3 - x, ge->iy3 - y);
			if (0)
				fprintf(stderr, "Glyph %s: print moveto(%d, %d)\n",
					g->name, ge->ix3, ge->iy3);
			x = ge->ix3;
			y = ge->iy3;
			break;
		case GE_LINE:
			if (absolute)
				fprintf(pfa_file, "%d %d alineto\n", ge->ix3, ge->iy3);
			else
				rlineto(ge->ix3 - x, ge->iy3 - y);
			x = ge->ix3;
			y = ge->iy3;
			break;
		case GE_CURVE:
			if (absolute)
				fprintf(pfa_file, "%d %d %d %d %d %d arcurveto\n",
					ge->ix1, ge->iy1, ge->ix2, ge->iy2, ge->ix3, ge->iy3);
			else
				rrcurveto(ge->ix1 - x, ge->iy1 - y,
					  ge->ix2 - ge->ix1, ge->iy2 - ge->iy1,
					  ge->ix3 - ge->ix2, ge->iy3 - ge->iy2);
			x = ge->ix3;
			y = ge->iy3;
			break;
		case GE_PATH:
			closepath();
			break;
		default:
			WARNING_1 fprintf(stderr, "**** Glyph %s: unknown entry type '%c'\n",
				g->name, ge->type);
			break;
		}
	}

	fprintf(pfa_file, "endchar } ND\n");
}

/* print the subroutines for this glyph, returns the number of them */
int
print_glyph_subs(
	   int glyphno,
	   int startid /* start numbering subroutines from this id */
)
{
	GLYPH *g;
	int i, grp;

	g = &glyph_list[glyphno];

	if(!hints || !subhints || g->nsg<1)
		return 0;

	g->firstsubr=startid;

#if 0
	fprintf(pfa_file, "%% %s %d\n", g->name, g->nsg);
#endif
	for(grp=0; grp<g->nsg; grp++) {
		fprintf(pfa_file, "dup %d {\n", startid++);
		for(i= (grp==0)? 0 : g->nsbs[grp-1]; i<g->nsbs[grp]; i++)
			fprintf(pfa_file, "\t%d %d %cstem\n", g->sbstems[i].low, 
				g->sbstems[i].high-g->sbstems[i].low,
				g->sbstems[i].isvert ? 'v' : 'h');
		fprintf(pfa_file, "\treturn\n\t} NP\n");
	}

	return g->nsg;
}

void
print_glyph_metrics(
	   int code,
	   int glyphno
)
{
	GLYPH *g;

	g = &glyph_list[glyphno];

	if(transform)
	  fprintf(afm_file, "C %d ; WX %d ; N %s ; B %d %d %d %d ;\n",
		  code, g->scaledwidth, g->name,
		  iscale(g->xMin), iscale(g->yMin), iscale(g->xMax), iscale(g->yMax));
	else
	  fprintf(afm_file, "C %d ; WX %d ; N %s ; B %d %d %d %d ;\n",
		  code, g->scaledwidth, g->name,
		  g->xMin, g->yMin, g->xMax, g->yMax);
}

/*
 SB:
 An important note about the BlueValues.

 The Adobe documentation says that the maximal width of a Blue zone
 is connected to the value of BlueScale, which is by default 0.039625.
 The BlueScale value defines, at which point size the overshoot
 suppression be disabled.

 The formula for it that is given in the manual is:

  BlueScale=point_size/240, for a 300dpi device

 that makes us wonder what is this 240 standing for. Incidentally
 240=72*1000/300, where 72 is the relation between inches and points,
 1000 is the size of the glyph matrix, and 300dpi is the resolution of
 the output device. Knowing that we can recalculate the formula for
 the font size in pixels rather than points:

  BlueScale=pixel_size/1000

 That looks a lot simpler than the original formula, does not it ?
 And the limitation about the maximal width of zone also looks
 a lot simpler after the transformation:

  max_width < 1000/pixel_size

 that ensures that even at the maximal pixel size when the overshoot
 suppression is disabled the zone width will be less than one pixel.
 This is important, failure to comply to this limit will result in
 really ugly fonts (been there, done that). But knowing the formula
 for the pixel width, we see that in fact we can use the maximal width
 of 24, not 23 as specified in the manual.

*/

#define MAXBLUEWIDTH (24)

/*
 * Find the indexes of the most frequent values
 * in the hystogram, sort them in ascending order, and save which one
 * was the best one (if asked).
 * Returns the number of values found (may be less than maximal because
 * we ignore the zero values)
 */

#define MAXHYST	(2000)		/* size of the hystogram */
#define HYSTBASE 500

static int
besthyst(
	 int *hyst,		/* the hystogram */
	 int base,		/* the base point of the hystogram */
	 int *best,		/* the array for indexes of best values */
	 int nbest,		/* its allocated size */
	 int width,		/* minimal difference between indexes */
	 int *bestindp		/* returned top point */
)
{
	unsigned char   hused[MAXHYST / 8 + 1];
	int             i, max, j, w, last = 0;
	int             nf = 0;

	width--;

	memset(hused, 0 , sizeof hused);

	max = 1;
	for (i = 0; i < nbest && max != 0; i++) {
		best[i] = 0;
		max = 0;
		for (j = 1; j < MAXHYST - 1; j++) {
			w = hyst[j];

			if (w > max && (hused[j>>3] & (1 << (j & 0x07))) == 0) {
				best[i] = j;
				max = w;
			}
		}
		if (max != 0) {
			if (max < last/2) {
				/* do not pick the too low values */
				break;
			}
			for (j = best[i] - width; j <= best[i] + width; j++) {
				if (j >= 0 && j < MAXHYST)
					hused[j >> 3] |= (1 << (j & 0x07));
			}
			last = max;
			best[i] -= base;
			nf = i + 1;
		}
	}

	if (bestindp)
		*bestindp = best[0];

	/* sort the indexes in ascending order */
	for (i = 0; i < nf; i++) {
		for (j = i + 1; j < nf; j++)
			if (best[j] < best[i]) {
				w = best[i];
				best[i] = best[j];
				best[j] = w;
			}
	}

	return nf;
}

/*
 * Find the next best Blue zone in the hystogram.
 * Return the weight of the found zone.
 */

static int
bestblue(
	 short *zhyst,		/* the zones hystogram */
	 short *physt,		/* the points hystogram */
	 short *ozhyst,		/* the other zones hystogram */
	 int *bluetab		/* where to put the found zone */
)
{
	int             i, j, w, max, ind, first, last;

	/* find the highest point in the zones hystogram */
	/* if we have a plateau, take its center */
	/* if we have multiple peaks, take the first one */

	max = -1;
	first = last = -10;
	for (i = 0; i <= MAXHYST - MAXBLUEWIDTH; i++) {
		w = zhyst[i];
		if (w > max) {
			first = last = i;
			max = w;
		} else if (w == max) {
			if (last == i - 1)
				last = i;
		}
	}
	ind = (first + last) / 2;

	if (max == 0)		/* no zones left */
		return 0;

	/* now we reuse `first' and `last' as inclusive borders of the zone */
	first = ind;
	last = ind + (MAXBLUEWIDTH - 1);

	/* our maximal width is far too big, so we try to make it narrower */
	w = max;
	j = (w & 1);		/* a pseudo-random bit */
	while (1) {
		while (physt[first] == 0)
			first++;
		while (physt[last] == 0)
			last--;
		if (last - first < (MAXBLUEWIDTH * 2 / 3) || (max - w) * 10 > max)
			break;

		if (physt[first] < physt[last]
		    || physt[first] == physt[last] && j) {
			if (physt[first] * 20 > w)	/* if weight is >5%,
							 * stop */
				break;
			w -= physt[first];
			first++;
			j = 0;
		} else {
			if (physt[last] * 20 > w)	/* if weight is >5%,
							 * stop */
				break;
			w -= physt[last];
			last--;
			j = 1;
		}
	}

	/* save our zone */
	bluetab[0] = first - HYSTBASE;
	bluetab[1] = last - HYSTBASE;

	/* invalidate all the zones overlapping with this one */
	/* the constant of 2 is determined by the default value of BlueFuzz */
	for (i = first - (MAXBLUEWIDTH - 1) - 2; i <= last + 2; i++)
		if (i >= 0 && i < MAXHYST) {
			zhyst[i] = 0;
			ozhyst[i] = 0;
		}
	return w;
}

/*
 * Try to find the Blue Values, bounding box and italic angle
 */

void
findblues(void)
{
	/* hystograms for upper and lower zones */
	short           hystl[MAXHYST];
	short           hystu[MAXHYST];
	short           zuhyst[MAXHYST];
	short           zlhyst[MAXHYST];
	int             nchars;
	int             i, j, k, w, max;
	GENTRY         *ge;
	GLYPH          *g;
	double          ang;

	/* find the lowest and highest points of glyphs */
	/* and by the way build the values for FontBBox */
	/* and build the hystogram for the ItalicAngle */

	/* re-use hystl for the hystogram of italic angle */

	bbox[0] = bbox[1] = 5000;
	bbox[2] = bbox[3] = -5000;

	for (i = 0; i < MAXHYST; i++)
		hystl[i] = 0;

	nchars = 0;

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		if (g->flags & GF_USED) {
			nchars++;

			g->rymin = 5000;
			g->rymax = -5000;
			for (ge = g->entries; ge != 0; ge = ge->next) {
				if (ge->type == GE_LINE) {

					j = ge->iy3 - ge->prev->iy3;
					k = ge->ix3 - ge->prev->ix3;
					if (j > 0)
						ang = atan2(-k, j) * 180.0 / M_PI;
					else
						ang = atan2(k, -j) * 180.0 / M_PI;

					k /= 100;
					j /= 100;
					if (ang > -45.0 && ang < 45.0) {
						/*
						 * be careful to not overflow
						 * the counter
						 */
						hystl[HYSTBASE + (int) (ang * 10.0)] += (k * k + j * j) / 4;
					}
					if (ge->iy3 == ge->prev->iy3) {
						if (ge->iy3 <= g->rymin) {
							g->rymin = ge->iy3;
							g->flatymin = 1;
						}
						if (ge->iy3 >= g->rymax) {
							g->rymax = ge->iy3;
							g->flatymax = 1;
						}
					} else {
						if (ge->iy3 < g->rymin) {
							g->rymin = ge->iy3;
							g->flatymin = 0;
						}
						if (ge->iy3 > g->rymax) {
							g->rymax = ge->iy3;
							g->flatymax = 0;
						}
					}
				} else if (ge->type == GE_CURVE) {
					if (ge->iy3 < g->rymin) {
						g->rymin = ge->iy3;
						g->flatymin = 0;
					}
					if (ge->iy3 > g->rymax) {
						g->rymax = ge->iy3;
						g->flatymax = 0;
					}
				}
				if (ge->type == GE_LINE || ge->type == GE_CURVE) {
					if (ge->ix3 < bbox[0])
						bbox[0] = ge->ix3;
					if (ge->ix3 > bbox[2])
						bbox[2] = ge->ix3;
					if (ge->iy3 < bbox[1])
						bbox[1] = ge->iy3;
					if (ge->iy3 > bbox[3])
						bbox[3] = ge->iy3;
				}
			}
		}
	}

	/* get the most popular angle */
	max = 0;
	w = 0;
	for (i = 0; i < MAXHYST; i++) {
		if (hystl[i] > w) {
			w = hystl[i];
			max = i;
		}
	}
	ang = (double) (max - HYSTBASE) / 10.0;
	WARNING_2 fprintf(stderr, "Guessed italic angle: %f\n", ang);
	if (italic_angle == 0.0)
		italic_angle = ang;

	/* build the hystogram of the lower points */
	for (i = 0; i < MAXHYST; i++)
		hystl[i] = 0;

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		if ((g->flags & GF_USED)
		    && g->rymin + HYSTBASE >= 0 && g->rymin < MAXHYST - HYSTBASE) {
			hystl[g->rymin + HYSTBASE]++;
		}
	}

	/* build the hystogram of the upper points */
	for (i = 0; i < MAXHYST; i++)
		hystu[i] = 0;

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		if ((g->flags & GF_USED)
		    && g->rymax + HYSTBASE >= 0 && g->rymax < MAXHYST - HYSTBASE) {
			hystu[g->rymax + HYSTBASE]++;
		}
	}

	/* build the hystogram of all the possible lower zones with max width */
	for (i = 0; i < MAXHYST; i++)
		zlhyst[i] = 0;

	for (i = 0; i <= MAXHYST - MAXBLUEWIDTH; i++) {
		for (j = 0; j < MAXBLUEWIDTH; j++)
			zlhyst[i] += hystl[i + j];
	}

	/* build the hystogram of all the possible upper zones with max width */
	for (i = 0; i < MAXHYST; i++)
		zuhyst[i] = 0;

	for (i = 0; i <= MAXHYST - MAXBLUEWIDTH; i++) {
		for (j = 0; j < MAXBLUEWIDTH; j++)
			zuhyst[i] += hystu[i + j];
	}

	/* find the baseline */
	w = bestblue(zlhyst, hystl, zuhyst, &bluevalues[0]);
	if (0)
		fprintf(stderr, "BaselineBlue zone %d%% %d...%d\n", w * 100 / nchars,
				bluevalues[0], bluevalues[1]);

	if (w == 0)		/* no baseline, something weird */
		return;

	/* find the upper zones */
	for (nblues = 2; nblues < 14; nblues += 2) {
		w = bestblue(zuhyst, hystu, zlhyst, &bluevalues[nblues]);

		if (0)
			fprintf(stderr, "Blue zone %d%% %d...%d\n", w * 100 / nchars, 
				bluevalues[nblues], bluevalues[nblues+1]);

		if (w * 20 < nchars)
			break;	/* don't save this zone */
	}

	/* find the lower zones */
	for (notherb = 0; notherb < 10; notherb += 2) {
		w = bestblue(zlhyst, hystl, zuhyst, &otherblues[notherb]);

		if (0)
			fprintf(stderr, "OtherBlue zone %d%% %d...%d\n", w * 100 / nchars,
				otherblues[notherb], otherblues[notherb+1]);


		if (w * 20 < nchars)
			break;	/* don't save this zone */
	}

}

/*
 * Find the actual width of the glyph and modify the
 * description to reflect it. Not guaranteed to do
 * any good, may make character spacing too wide.
 */

void
docorrectwidth(void)
{
	int             i;
	GENTRY         *ge;
	GLYPH          *g;
	int             xmin, xmax;
	int             maxwidth, minsp;

	/* enforce this minimal spacing,
	 * we limit the amount of the enforced spacing to avoid
	 * spacing the bold wonts too widely
	 */
	minsp = (stdhw>60 || stdhw<10)? 60 : stdhw;

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		g->oldwidth=g->scaledwidth; /* save the old width, will need for AFM */

		if (correctwidth && g->flags & GF_USED) {
			xmin = 5000;
			xmax = -5000;
			for (ge = g->entries; ge != 0; ge = ge->next) {
				if (ge->type != GE_LINE && ge->type != GE_CURVE) 
					continue;

				if (ge->ix3 <= xmin) {
					xmin = ge->ix3;
				}
				if (ge->ix3 >= xmax) {
					xmax = ge->ix3;
				}
			}

			maxwidth=xmax+minsp;
			if( g->scaledwidth < maxwidth ) {
				g->scaledwidth = maxwidth;
				WARNING_3 fprintf(stderr, "glyph %s: extended from %d to %d\n",
					g->name, g->oldwidth, g->scaledwidth );
			}
		}
	}

}

/*
 * Try to find the typical stem widths
 */

void
stemstatistics(void)
{
#define MINDIST	10 /* minimal distance between the widths */
	int             hyst[MAXHYST+MINDIST*2];
	int             best[12];
	int             i, j, k, w;
	int             nchars;
	int             ns;
	STEM           *s;
	GLYPH          *g;

	/* start with typical stem width */

	nchars=0;

	/* build the hystogram of horizontal stem widths */
	memset(hyst, 0, sizeof hyst);

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		if (g->flags & GF_USED) {
			nchars++;
			s = g->hstems;
			for (j = 0; j < g->nhs; j += 2) {
				if ((s[j].flags | s[j + 1].flags) & ST_END)
					continue;
				w = s[j + 1].value - s[j].value+1;
				if(w==20) /* split stems should not be counted */
					continue;
				if (w > 0 && w < MAXHYST - 1) {
					/*
					 * handle some fuzz present in
					 * converted fonts
					 */
					hyst[w+MINDIST] += MINDIST-1;
					for(k=1; k<MINDIST-1; k++) {
						hyst[w+MINDIST + k] += MINDIST-1-k;
						hyst[w+MINDIST - k] += MINDIST-1-k;
					}
				}
			}
		}
	}

	/* find 12 most frequent values */
	ns = besthyst(hyst+MINDIST, 0, best, 12, MINDIST, &stdhw);

	/* store data in stemsnaph */
	for (i = 0; i < ns; i++)
		stemsnaph[i] = best[i];
	if (ns < 12)
		stemsnaph[ns] = 0;

	/* build the hystogram of vertical stem widths */
	memset(hyst, 0, sizeof hyst);

	for (i = 0, g = glyph_list; i < numglyphs; i++, g++) {
		if (g->flags & GF_USED) {
			s = g->vstems;
			for (j = 0; j < g->nvs; j += 2) {
				if ((s[j].flags | s[j + 1].flags) & ST_END)
					continue;
				w = s[j + 1].value - s[j].value+1;
				if (w > 0 && w < MAXHYST - 1) {
					/*
					 * handle some fuzz present in
					 * converted fonts
					 */
					hyst[w+MINDIST] += MINDIST-1;
					for(k=1; k<MINDIST-1; k++) {
						hyst[w+MINDIST + k] += MINDIST-1-k;
						hyst[w+MINDIST - k] += MINDIST-1-k;
					}
				}
			}
		}
	}

	/* find 12 most frequent values */
	ns = besthyst(hyst+MINDIST, 0, best, 12, MINDIST, &stdvw);

	/* store data in stemsnaph */
	for (i = 0; i < ns; i++)
		stemsnapv[i] = best[i];
	if (ns < 12)
		stemsnapv[ns] = 0;

#undef MINDIST
}

/*
 * SB
 * A funny thing: TTF paths are going in reverse direction compared
 * to Type1. So after all (because the rest of logic uses TTF
 * path directions) we have to reverse the paths.
 *
 * It was a big headache to discover that.
 */

/* works on both int and float paths */

void
reversepathsfromto(
		   GENTRY * from,
		   GENTRY * to
)
{
	GENTRY         *ge, *nge, *pge;
	GENTRY         *cur, *next;
	int i, n, ilast[2];
	double flast[2], f;

	for (ge = from; ge != 0 && ge != to; ge = ge->next) {
		if(ge->type == GE_LINE || ge->type == GE_CURVE) {
			if (ISDBG(REVERSAL))
				fprintf(stderr, "reverse path 0x%x <- 0x%x, 0x%x\n", ge, ge->prev, ge->bkwd);

			/* cut out the path itself */
			pge = ge->prev; /* GE_MOVE */
			if (pge == 0) {
				fprintf(stderr, "**! No MOVE before line !!! Fatal. ****\n");
				exit(1);
			}
			nge = ge->bkwd->next; /* GE_PATH */
			pge->next = nge;
			nge->prev = pge;
			ge->bkwd->next = 0; /* mark end of chain */

			/* remember the starting point */
			if(ge->flags & GEF_FLOAT) {
				flast[0] = pge->fx3;
				flast[1] = pge->fy3;
			} else {
				ilast[0] = pge->ix3;
				ilast[1] = pge->iy3;
			}

			/* then reinsert them in backwards order */
			for(cur = ge; cur != 0; cur = next ) {
				next = cur->next; /* or addgeafter() will screw it up */
				if(cur->flags & GEF_FLOAT) {
					for(i=0; i<2; i++) {
						/* reverse the direction of path element */
						f = cur->fpoints[i][0];
						cur->fpoints[i][0] = cur->fpoints[i][1];
						cur->fpoints[i][1] = f;
						f = flast[i];
						flast[i] = cur->fpoints[i][2];
						cur->fpoints[i][2] = f;
					}
				} else {
					for(i=0; i<2; i++) {
						/* reverse the direction of path element */
						n = cur->ipoints[i][0];
						cur->ipoints[i][0] = cur->ipoints[i][1];
						cur->ipoints[i][1] = n;
						n = ilast[i];
						ilast[i] = cur->ipoints[i][2];
						cur->ipoints[i][2] = n;
					}
				}
				addgeafter(pge, cur);
			}

			/* restore the starting point */
			if(ge->flags & GEF_FLOAT) {
				pge->fx3 = flast[0];
				pge->fy3 = flast[1];
			} else {
				pge->ix3 = ilast[0];
				pge->iy3 = ilast[1];
			}

			ge = nge;
		}

	}
}

void
reversepaths(
	     GLYPH * g
)
{
	reversepathsfromto(g->entries, NULL);
}

/* add a kerning pair information, scales the value */

void
addkernpair(
	unsigned id1,
	unsigned id2,
	int unscval
)
{
	static unsigned char *bits = 0;
	static int lastid;
	GLYPH *g = &glyph_list[id1];
	int i, n;
	struct kern *p;

	if(unscval == 0 || id1 >= numglyphs || id2 >= numglyphs)
		return;

	if( (glyph_list[id1].flags & GF_USED)==0
	|| (glyph_list[id2].flags & GF_USED)==0 )
		return;

	if(bits == 0) {
		bits = calloc( BITMAP_BYTES(numglyphs), 1);
		if (bits == NULL) {
			fprintf (stderr, "****malloc failed %s line %d\n", __FILE__, __LINE__);
			exit(255);
		}
		lastid = id1;
	}

	if(lastid != id1) {
		/* refill the bitmap cache */
		memset(bits, 0,BITMAP_BYTES(numglyphs));
		p = g->kern;
		for(i=g->kerncount; i>0; i--) {
			n = (p++)->id;
			SET_BITMAP(bits, n);
		}
		lastid = id1;
	}

	if(IS_BITMAP(bits, id2))
		return; /* duplicate */

	if(g->kerncount <= g->kernalloc) {
		g->kernalloc += 8;
		p = realloc(g->kern, sizeof(struct kern) * g->kernalloc);
		if(p == 0) {
			fprintf (stderr, "** realloc failed, kerning data will be incomplete\n");
		}
		g->kern = p;
	}

	SET_BITMAP(bits, id2);
	p = &g->kern[g->kerncount];
	p->id = id2;
	p->val = iscale(unscval) - (g->scaledwidth - g->oldwidth);
	g->kerncount++;
	kerning_pairs++;
}

/* print out the kerning information */

void
print_kerning(
	FILE *afm_file
)
{
	int	i, j, n;
	GLYPH *g;
	struct kern *p;

	if( kerning_pairs == 0 ) 
		return;

	fprintf(afm_file, "StartKernData\n");
	fprintf(afm_file, "StartKernPairs %hd\n", kerning_pairs);

	for(i=0; i<numglyphs; i++)  {
		g = &glyph_list[i];
		if( (g->flags & GF_USED) ==0)
			continue;
		p = g->kern;
		for(j=g->kerncount; j>0; j--, p++) {
			fprintf(afm_file, "KPX %s %s %d\n", g->name, 
				glyph_list[ p->id ].name, p->val );
		}
	}

	fprintf(afm_file, "EndKernPairs\n");
	fprintf(afm_file, "EndKernData\n");
}


#if 0

/*
** This function is commented out because the information
** collected by it is not used anywhere else yet. Now
** it only collects the directions of contours. And the
** direction of contours gets fixed already in draw_glyf().
**
***********************************************
**
** Here we expect that the paths are already closed.
** We also expect that the contours do not intersect
** and that curves doesn't cross any border of quadrant.
**
** Find which contours go inside which and what is
** their proper direction. Then fix the direction
** to make it right.
**
*/

#define MAXCONT	1000

void
fixcontours(
	    GLYPH * g
)
{
	CONTOUR         cont[MAXCONT];
	short           ymax[MAXCONT];	/* the highest point */
	short           xofmax[MAXCONT];	/* X-coordinate of any point
						 * at ymax */
	short           ymin[MAXCONT];	/* the lowest point */
	short           xofmin[MAXCONT];	/* X-coordinate of any point
						 * at ymin */
	short           count[MAXCONT];	/* count of lines */
	char            dir[MAXCONT];	/* in which direction they must go */
	GENTRY         *start[MAXCONT], *minptr[MAXCONT], *maxptr[MAXCONT];
	int             ncont;
	int             i;
	int             dx1, dy1, dx2, dy2;
	GENTRY         *ge, *nge;

	/* find the contours and their most upper/lower points */
	ncont = 0;
	ymax[0] = -5000;
	ymin[0] = 5000;
	for (ge = g->entries; ge != 0; ge = ge->next) {
		if (ge->type == GE_LINE || ge->type == GE_CURVE) {
			if (ge->iy3 > ymax[ncont]) {
				ymax[ncont] = ge->iy3;
				xofmax[ncont] = ge->ix3;
				maxptr[ncont] = ge;
			}
			if (ge->iy3 < ymin[ncont]) {
				ymin[ncont] = ge->iy3;
				xofmin[ncont] = ge->ix3;
				minptr[ncont] = ge;
			}
		}
		if (ge->frwd != ge->next) {
			start[ncont++] = ge->frwd;
			ymax[ncont] = -5000;
			ymin[ncont] = 5000;
		}
	}

	/* determine the directions of contours */
	for (i = 0; i < ncont; i++) {
		ge = minptr[i];
		nge = ge->frwd;

		if (ge->type == GE_CURVE) {
			dx1 = ge->ix3 - ge->ix2;
			dy1 = ge->iy3 - ge->iy2;

			if (dx1 == 0 && dy1 == 0) {	/* a pathological case */
				dx1 = ge->ix3 - ge->ix1;
				dy1 = ge->iy3 - ge->iy1;
			}
			if (dx1 == 0 && dy1 == 0) {	/* a more pathological
							 * case */
				dx1 = ge->ix3 - ge->prev->ix3;
				dy1 = ge->iy3 - ge->prev->iy3;
			}
		} else {
			dx1 = ge->ix3 - ge->prev->ix3;
			dy1 = ge->iy3 - ge->prev->iy3;
		}
		if (nge->type == GE_CURVE) {
			dx2 = ge->ix3 - nge->ix1;
			dy2 = ge->iy3 - nge->iy1;
			if (dx1 == 0 && dy1 == 0) {	/* a pathological case */
				dx2 = ge->ix3 - nge->ix2;
				dy2 = ge->iy3 - nge->iy2;
			}
			if (dx1 == 0 && dy1 == 0) {	/* a more pathological
							 * case */
				dx2 = ge->ix3 - nge->ix3;
				dy2 = ge->iy3 - nge->iy3;
			}
		} else {
			dx2 = ge->ix3 - nge->ix3;
			dy2 = ge->iy3 - nge->iy3;
		}

		/* compare angles */
		cont[i].direction = DIR_INNER;
		if (dy1 == 0) {
			if (dx1 < 0)
				cont[i].direction = DIR_OUTER;
		} else if (dy2 == 0) {
			if (dx2 > 0)
				cont[i].direction = DIR_OUTER;
		} else if (dx2 * dy1 < dx1 * dy2)
			cont[i].direction = DIR_OUTER;

		cont[i].ymin = ymin[i];
		cont[i].xofmin = xofmin[i];
	}

	/* save the information that may be needed further */
	g->ncontours = ncont;
	if (ncont > 0) {
		g->contours = malloc(sizeof(CONTOUR) * ncont);
		if (g->contours == 0) {
			fprintf(stderr, "***** Memory allocation error *****\n");
			exit(255);
		}
		memcpy(g->contours, cont, sizeof(CONTOUR) * ncont);
	}
}

#endif

/*
 *
 */

