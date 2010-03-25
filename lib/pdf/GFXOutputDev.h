#ifndef __gfxoutputdev_h__
#define __gfxoutputdev_h__

#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"

#include "config.h"
#include "InfoOutputDev.h"
#include "PDFDoc.h"
#include "GlobalParams.h"
#include "CommonOutputDev.h"

class GFXOutputState {
    public:
    int clipping;
    int textRender;
    char createsoftmask;
    char transparencygroup;
    char softmask;
    char softmask_alpha;
    char isolated;

    gfxbbox_t clipbbox;

    GFXOutputState();

    gfxresult_t* grouprecording; // for transparency groups
    gfxresult_t* softmaskrecording; // for soft masks

    gfxdevice_t* olddevice;
  
    double *dashPattern;
    int dashLength;
    double dashStart;

    GfxState*state;
};

typedef struct _feature
{
    char*string;
    struct _feature*next;
} feature_t;

void addGlobalFont(const char*filename);
void addGlobalLanguageDir(const char*dir);
void addGlobalFontDir(const char*dirname);

class GFXOutputGlobals {
public:
  feature_t*featurewarnings;

  int textmodeinfo; // did we write "Text will be rendered as polygon" yet?
  int jpeginfo; // did we write "File contains jpegs" yet?
  int pbminfo; // did we write "File contains jpegs" yet?
  int linkinfo; // did we write "File contains links" yet?

  GFXOutputGlobals();
  ~GFXOutputGlobals();
};

class GFXOutputDev:  public CommonOutputDev {
public:
  gfxdevice_t* device;

  GFXOutputDev(InfoOutputDev*info, PDFDoc*doc);
  virtual ~GFXOutputDev() ;

  virtual void setDevice(gfxdevice_t*dev);
  virtual void setMove(int x,int y);
  virtual void setClip(int x1,int y1,int x2,int y2);
  virtual void setParameter(const char*key, const char*value);
  
  // Start a page.
  virtual void startPage(int pageNum, GfxState *state, double x1, double y1, double x2, double y2) ;
  virtual void endPage();

  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown();

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar();
  
  virtual GBool interpretType3Chars();
  
  //virtual GBool useShadedFills() { return gTrue; }

  //----- link borders
  virtual void processLink(Link *link, Catalog *catalog);

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) ;
  virtual void restoreState(GfxState *state) ;

  //----- update graphics state

  virtual void updateLineDash(GfxState *state);
  virtual void updateFont(GfxState *state);
  virtual void updateFontMatrix(GfxState *state);
  virtual void updateFillColor(GfxState *state);
  virtual void updateStrokeColor(GfxState *state);
  virtual void updateLineWidth(GfxState *state);
  virtual void updateLineJoin(GfxState *state);
  virtual void updateLineCap(GfxState *state);
  virtual void updateFillOpacity(GfxState *state);
  virtual void updateStrokeOpacity(GfxState *state);
  virtual void updateFillOverprint(GfxState *state);
  virtual void updateStrokeOverprint(GfxState *state);
  virtual void updateTransfer(GfxState *state);
  
  virtual void updateAll(GfxState *state) 
  {
      updateFont(state);
      updateFillColor(state);
      updateStrokeColor(state);
      updateLineWidth(state);
      updateLineJoin(state);
      updateLineCap(state);
  };

  //----- path painting
  virtual void stroke(GfxState *state) ;
  virtual void fill(GfxState *state) ;
  virtual void eoFill(GfxState *state) ;

  //----- path clipping
  virtual void clip(GfxState *state) ;
  virtual void eoClip(GfxState *state) ;
  virtual void clipToStrokePath(GfxState *state);
 
  //----- shaded fills
  virtual GBool useTilingPatternFill();
  virtual GBool useShadedFills();

#if (xpdfMajorVersion < 3) || (xpdfMinorVersion < 2) || (xpdfUpdateVersion < 7)
  virtual void tilingPatternFill(GfxState *state, Object *str,
			     int paintType, Dict *resDict,
			     double *mat, double *bbox,
			     int x0, int y0, int x1, int y1,
			     double xStep, double yStep);
#else
  virtual void tilingPatternFill(GfxState *state, Gfx *gfx, Object *str,
			     int paintType, Dict *resDict,
			     double *mat, double *bbox,
			     int x0, int y0, int x1, int y1,
			     double xStep, double yStep);
#endif
  virtual GBool functionShadedFill(GfxState *state,
				   GfxFunctionShading *shading);
  virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading);
  virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading);

  //----- text drawing
  virtual void beginString(GfxState *state, GString *s) ;
  virtual void endString(GfxState *state) ;
  virtual void endTextObject(GfxState *state);
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, int nBytes, Unicode *u, int uLen);

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap);

  //----- transparency groups and soft masks (xpdf >= ~ 3.01.16)
  virtual void beginTransparencyGroup(GfxState *state, double *bbox,
				      GfxColorSpace *blendingColorSpace,
				      GBool isolated, GBool knockout,
				      GBool forSoftMask);
  virtual void endTransparencyGroup(GfxState *state);
  virtual void paintTransparencyGroup(GfxState *state, double *bbox);
  virtual void setSoftMask(GfxState *state, double *bbox, GBool alpha, Function *transferFunc, GfxColor *backdropColor);
  virtual void clearSoftMask(GfxState *state);
 
  //----- type 3 chars
  virtual GBool beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen);
  virtual void endType3Char(GfxState *state);

  virtual void type3D0(GfxState *state, double wx, double wy);
  virtual void type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury);

  char* searchForSuitableFont(GfxFont*gfxFont);

  void finish();

  virtual GBool useDrawForm();
  virtual void drawForm(Ref id);
  virtual GBool needNonText();

  //virtual void dump();
  //virtual void beginStringOp(GfxState *state);
  //virtual void drawString(GfxState *state, GString *s);
  //virtual void endStringOp(GfxState *state);
  //virtual GBool getVectorAntialias() { return gFalse; }
  //virtual void setVectorAntialias(GBool vaa) {}
  //virtual void psXObject(Stream *psStream, Stream *level1Stream) {}

  virtual void setPageMap(int*pagemap, int pagemap_len);

  private:
  gfxline_t* gfxPath_to_gfxline(GfxState*state, GfxPath*path, int closed, int user_movex, int user_movey);

  void transformXY(GfxState*state, double x, double y, double*nx, double*ny);

  void drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int *maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap);

  void strokeGfxline(GfxState *state, gfxline_t*line, int flags);
  void clipToGfxLine(GfxState *state, gfxline_t*line, char evenodd);
  void fillGfxLine(GfxState *state, gfxline_t*line, char evenodd);

  gfxfont_t* createGfxFont(GfxFont*xpdffont, FontInfo*src);

  void showfeature(const char*feature,char fully, char warn);
  void warnfeature(const char*feature,char fully);
  void infofeature(const char*feature);
  
  char* searchFont(const char*name);
  char* substituteFont(GfxFont*gfxFont, char*oldname);
  char* writeEmbeddedFontToFile(XRef*ref, GfxFont*font);

  /* for page mapping */
  int* page2page;
  int num_pages;


  int currentpage;
  char outer_clip_box; //whether the page clip box is still on
  InfoOutputDev*info;
  GFXOutputState states[64];
  int statepos;

  PDFDoc*doc;
  XRef*xref;

  int type3active; // are we between beginType3()/endType3()?

  GfxState *laststate;

  int user_movex,user_movey;
  int user_clipx1,user_clipx2,user_clipy1,user_clipy2;

  /* upper left corner of clipping rectangle (cropbox)- needs to be
     added to all drawing coordinates to give the impression that all
     pages start at (0,0)*/
  int clipmovex;
  int clipmovey;

  gfxline_t* current_text_stroke;
  gfxline_t* current_text_clip;
  gfxfont_t* current_gfxfont;
  FontInfo*current_fontinfo;
  gfxmatrix_t current_font_matrix;

  gfxfont_t* last_char_gfxfont;
  int last_char;
  double last_char_x;
  double last_char_y;
  char last_char_was_space;

  /* config */
  int config_use_fontconfig;
  int config_break_on_warning;
  int config_remapunicode;
  int config_transparent;
  int config_extrafontdata;
  int config_convertgradients;
  int config_disable_polygon_conversion;
  int config_multiply;
  int config_bigchar;
  int config_drawonlyshapes;
  int config_detectspaces;
  char* config_linkdatafile;
  double config_fontquality;
};

class GFXGlobalParams:  public GlobalParams {
    public:
    GFXGlobalParams();
    ~GFXGlobalParams();
    virtual DisplayFontParam *getDisplayFont(GString *fontName);
};

#endif //__gfxoutputdev_h__
