#ifndef __vectorgraphicsoutputdev_h__
#define __vectorgraphicsoutputdev_h__

#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../gfxtools.h"
#include "../kdtree.h"

#include "CommonOutputDev.h"
#include "InfoOutputDev.h"
#include "CharOutputDev.h"
#include "PDFDoc.h"
#include "GlobalParams.h"

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

void addGlobalFont(const char*filename);
void addGlobalLanguageDir(const char*dir);
void addGlobalFontDir(const char*dirname);

class GFXLink;
  
void drawchar_callback(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix);
void addfont_callback(gfxdevice_t*dev, gfxfont_t*font);

class VectorGraphicOutputDev:  public CommonOutputDev {
public:
  gfxdevice_t* device;

  VectorGraphicOutputDev(InfoOutputDev*info, PDFDoc*doc, int*page2page, int num_pages, int x, int y, int x1, int y1, int x2, int y2);
  virtual ~VectorGraphicOutputDev() ;

  virtual void setDevice(gfxdevice_t*dev);
  virtual void setParameter(const char*key, const char*value);
  
  // Start a page.
  virtual void beginPage(GfxState *state, int pageNum);
  virtual void endPage();
    
  virtual GBool checkPageSlice(Page *page, double hDPI, double vDPI,
			       int rotate, GBool useMediaBox, GBool crop,
			       int sliceX, int sliceY, int sliceW, int sliceH,
			       GBool printing, Catalog *catalog,
			       GBool (*abortCheckCbk)(void *data) = NULL,
			       void *abortCheckCbkData = NULL);
  //----- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown();

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar();
  
  //virtual GBool useShadedFills() { return gTrue; }

  //----- link borders
  virtual void processLink(Link *link, Catalog *catalog);
  virtual void setDefaultCTM(double *ctm);

  //----- save/restore graphics state
  virtual void saveState(GfxState *state) ;
  virtual void restoreState(GfxState *state) ;

  //----- update graphics state
  virtual void updateLineDash(GfxState *state);
  virtual void updateFont(GfxState *state);
  virtual void updateFontMatrix(GfxState *state);
  
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

virtual POPPLER_TILING_PATERN_RETURN tilingPatternFill(GfxState *state,
			     POPPLER_TILING_PATERN_GFX
			     Object *str,
			     int paintType, Dict *resDict,
			     double *mat, double *bbox,
			     int x0, int y0, int x1, int y1,
			     double xStep, double yStep);
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

  private:
  gfxline_t* gfxPath_to_gfxline(GfxState*state, GfxPath*path, int closed);

  void drawGeneralImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height, GfxImageColorMap*colorMap, GBool invert,
				   GBool inlineImg, int mask, int *maskColors,
				   Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GfxImageColorMap*maskColorMap);

  void strokeGfxline(GfxState *state, gfxline_t*line, int flags);
  void clipToGfxLine(GfxState *state, gfxline_t*line, char evenodd);
  void fillGfxLine(GfxState *state, gfxline_t*line, char evenodd);

  int currentpage;
  char outer_clip_box; //whether the page clip box is still on
  GFXOutputState states[64];
  int statepos;

  int type3active; // are we between beginType3()/endType3()?
  GfxState *laststate;

  gfxline_t* current_text_stroke;
  gfxline_t* current_text_clip;
  gfxfont_t* current_gfxfont;
  FontInfo*current_fontinfo;
  gfxmatrix_t current_font_matrix;

  /* config */
  int config_break_on_warning;
  int config_remapunicode;
  int config_transparent;
  int config_convertgradients;
  int config_disable_polygon_conversion;
  int config_multiply;
  int config_drawonlyshapes;
  int config_textonly;

  gfxdevice_t char_output_dev;
  CharOutputDev*charDev;
  friend void drawchar_callback(gfxdevice_t*dev, gfxfont_t*font, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix);
  friend void addfont_callback(gfxdevice_t*dev, gfxfont_t*font);
  gfxfont_t*gfxfont_from_callback;
  int glyphnr_from_callback;
  gfxcolor_t textcolor_from_callback;
  gfxmatrix_t textmatrix_from_callback;
};

#endif //__vectorgraphicsoutputdev_h__
