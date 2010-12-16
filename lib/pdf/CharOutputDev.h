#ifndef __charoutputdev_h__
#define __charoutputdev_h__

#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../kdtree.h"

#include "InfoOutputDev.h"
#include "PDFDoc.h"
#include "GlobalParams.h"
#include "CommonOutputDev.h"

extern void addGlobalFont(const char*filename);
extern void addGlobalLanguageDir(const char*dir);
extern void addGlobalFontDir(const char*dirname);

class GFXLink;

class CharOutputDev:  public CommonOutputDev {
public:
  gfxdevice_t* device;

  CharOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2);
  virtual ~CharOutputDev();

  virtual void setDevice(gfxdevice_t*dev);
  virtual void setParameter(const char*key, const char*value);
  
  virtual void beginPage(GfxState *state, int pageNum);
  virtual void endPage();

  virtual GBool upsideDown();
  virtual GBool useDrawChar();

  virtual void processLink(Link *link, Catalog *catalog);
  virtual void saveState(GfxState *state) ;
  virtual void restoreState(GfxState *state) ;

  //----- update graphics state
  virtual void updateFont(GfxState *state);
  virtual void updateTextMat(GfxState *state);
  
  //----- text drawing
  virtual void beginString(GfxState *state, GString *s) ;
  virtual void endString(GfxState *state) ;
  virtual void endTextObject(GfxState *state);
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, int nBytes, Unicode *u, int uLen);

  //----- type 3 chars
  virtual GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);

  virtual void type3D0(GfxState *state, double wx, double wy);
  virtual void type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury);

  virtual GBool needNonText();

  private:
  
  int currentpage;
  int type3active; // are we between beginType3()/endType3()?

  gfxline_t* current_text_stroke;
  gfxline_t* current_text_clip;

  // state variables for space char detection
  int last_char;
  double last_char_x;
  double last_char_y;
  double last_char_x_fontsize;
  double last_char_y_fontsize;
  double last_char_advance;
  double last_average_advance;
  double last_ascent;
  double last_descent;
  char last_char_was_space;
    
  GFXLink*last_link;
  GFXLink*previous_link;
  kdtree_t*links;
  
  /* config */
  int config_use_fontconfig;
  int config_remapunicode;
  int config_extrafontdata;
  int config_bigchar;
  int config_detectspaces;
  int config_space_between_lines;
  double config_fontquality;
  char* config_linkdatafile;
};

class GFXLink {
    double x1,y1,x2,y2;
    int size;
    int buf_size;
    char*text;
  public:
    const char*action;
    GFXLink*last;
    void draw(CharOutputDev*out, gfxdevice_t*dev);
    void addchar(int unicode);
    GFXLink(GFXLink*last, const char*action, double x1, double y1, double x2, double y2);
    ~GFXLink();
};

class GFXGlobalParams:  public GlobalParams {
    public:
    GFXGlobalParams();
    ~GFXGlobalParams();
    virtual DisplayFontParam *getDisplayFont(GString *fontName);
};

#endif //__charoutputdev_h__
