#include "popplercompat.h"
#include "CommonOutputDev.h"
#include "../log.h"
#include "../gfxdevice.h"

int config_break_on_warning = 0;

CommonOutputDev::CommonOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2)
{
    this->info = info;
    this->doc = doc;
    this->xref = doc->getXRef();

    if(x2<x1) {int x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {int y3=y1;y1=y2;y2=y3;}

    this->user_movex = x;
    this->user_movey = y;
    this->user_clipx1 = x1;
    this->user_clipy1 = y1;
    this->user_clipx2 = x2;
    this->user_clipy2 = y2;
}

void CommonOutputDev::startPage(int pageNum, GfxState*state)
{
    double x1,y1,x2,y2;
    PDFRectangle *r = this->page->getCropBox();
    state->transform(r->x1,r->y1,&x1,&y1);
    state->transform(r->x2,r->y2,&x2,&y2);
    if(x2<x1) {double x3=x1;x1=x2;x2=x3;}
    if(y2<y1) {double y3=y1;y1=y2;y2=y3;}
    
    this->movex = -(int)x1 - this->user_clipx1 + this->user_movex;
    this->movey = -(int)y1 - this->user_clipy1 + this->user_movey;

    if(this->user_clipx1|this->user_clipy1|this->user_clipx2|this->user_clipy2) {
	this->width = this->user_clipx2 - this->user_clipx1;
	this->height = this->user_clipy2 - this->user_clipy1;
    } else {
	this->width = x2-x1;
	this->height = y2-y1;
    }
    beginPage(state, pageNum);
}

void CommonOutputDev::transformXY(GfxState*state, double x, double y, double*nx, double*ny)
{
    state->transform(x,y,nx,ny);
    *nx += movex;
    *ny += movey;
}
void CommonOutputDev::transformXY_stateless(double x, double y, int*xout, int*yout)
{
    cvtUserToDev(x, y, xout, yout);
    *xout += movex;
    *yout += movey;
}
    
GBool CommonOutputDev::interpretType3Chars() 
{
    return gTrue;
}

GFXOutputGlobals::GFXOutputGlobals()
{
    this->featurewarnings = 0;
    this->jpeginfo = 0;
    this->textmodeinfo = 0;
    this->linkinfo = 0;
    this->pbminfo = 0;
}
GFXOutputGlobals::~GFXOutputGlobals()
{
    feature_t*f = this->featurewarnings;
    while(f) {
	feature_t*next = f->next;
	if(f->string) {
	    free(f->string);f->string =0;
	}
	f->next = 0;
	free(f);
	f = next;
    }
    this->featurewarnings = 0;
}

static GFXOutputGlobals*gfxglobals=0;

static void showfeature(const char*feature, char fully, char warn)
{
    if(!gfxglobals)
	gfxglobals = new GFXOutputGlobals();

    feature_t*f = gfxglobals->featurewarnings;
    while(f) {
	if(!strcmp(feature, f->string))
	    return;
	f = f->next;
    }
    f = (feature_t*)malloc(sizeof(feature_t));
    f->string = strdup(feature);
    f->next = gfxglobals->featurewarnings;
    gfxglobals->featurewarnings = f;
    if(warn) {
	msg("<warning> %s not yet %ssupported!",feature,fully?"fully ":"");
    } else {
	msg("<notice> File contains %s",feature);
    }
}
void warnfeature(const char*feature,char fully)
{
    showfeature(feature,fully,1);
    if(config_break_on_warning) {
	msg("<fatal> Aborting conversion due to unsupported feature");
	exit(1);
    }
}
void infofeature(const char*feature)
{
    showfeature(feature,0,0);
}
GFXOutputGlobals* getGfxGlobals()
{
    if(!gfxglobals)
	gfxglobals = new GFXOutputGlobals();
    return gfxglobals;
}

gfxcolor_t gfxstate_getfillcolor(GfxState * state)
{
    GfxRGB rgb;
    double opaq = state->getFillOpacity();
    state->getFillRGB(&rgb);
    gfxcolor_t col;
    col.r = colToByte(rgb.r);
    col.g = colToByte(rgb.g);
    col.b = colToByte(rgb.b);
    col.a = (unsigned char)(opaq*255);
    return col;
}

bool text_matrix_is_skewed(GfxState * state)
{
    double*ctm = state->getCTM();
    double*tm = state->getTextMat();
    double m00 = ctm[0]*tm[0] + ctm[2]*tm[1];
    double m01 = ctm[1]*tm[0] + ctm[3]*tm[1];
    double m10 = ctm[0]*tm[2] + ctm[2]*tm[3];
    double m11 = ctm[1]*tm[2] + ctm[3]*tm[3];
    return (m00<0 || m10<0);
}

