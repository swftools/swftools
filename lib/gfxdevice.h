#define GFX_SUBPIXEL 2560

typedef float gfxcoord_t;
    
typedef enum {gfx_moveTo, gfx_lineTo, gfx_splineTo} gfx_linetype;

typedef struct _gfxline
{
    gfx_linetype type;
    gfxcoord_t x,y;
    gfxcoord_t sx,sy;
    struct _gfxline*next; /*NULL=end*/
} gfxline_t;

typedef struct _gfxglyph
{
    gfxline_t*line;
    gfxcoord_t advance;

    int unicode; // array?
    char*name;
} gfxglyph_t;

typedef struct _gfxfont
{
    char*name;
    int num_glyphs;
    int max_unicode;
    gfxglyph_t*glyphs;
    int* unicode2glyph;
} gfxfont_t;

typedef struct _gfxcolor
{
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} gfxcolor_t;

typedef struct _gfxmatrix
{
    float m00,m10,tx;
    float m01,m11,ty;
} gfxmatrix_t;

typedef struct _gfximage
{
    gfxcolor_t*data;
    int width;
    int height;
} gfximage_t;

typedef enum {gfxgradient_radial, gfxgradient_linear} gfxgradienttype_t;

typedef struct _gfxgradient
{
    gfxcolor_t color;
    float pos; // 0.0 - 1.0
    struct _gfxgradient*next; //NULL=end
} gfxgradient_t;

typedef struct _gfxcxform
{
    float rr,rg,rb,ra;
    float gr,gg,gb,ga;
    float br,bg,bb,ba;
    float ar,ag,ab,aa;
    gfxcolor_t t;
} gfxcxform_t;

typedef struct _gfxbbox
{
    gfxcoord_t xmin, ymin, xmax, ymax;
} gfxbbox_t;

typedef struct _gfxdevice
{
    int (*setparameter)(struct _gfxdevice*dev, const char*key, const char*value);

    int (*startpage)(struct _gfxdevice*dev, int xmin, int ymin, int xmax, int ymax); /*xmin/ymin?*/

    void (*startClip)(struct _gfxdevice*dev, gfxline_t*line);
    void (*endClip)(struct _gfxdevice*dev);
    void (*stroke)(struct _gfxdevice*dev, gfxline_t*line, int width, gfxcolor_t*color, int cap_style, int joint_style);
    void (*fill)(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color);
    void (*fillbitmap)(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform); //cxform? tiling?
    void (*fillgradient)(struct _gfxdevice*dev, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix); //?

    void (*addfont)(struct _gfxdevice*dev, char*fontid, gfxfont_t*font, gfxmatrix_t*matrix);
    void (*drawchar)(struct _gfxdevice*dev, char*fontid, int glyph, int x, int y);

    void (*drawLink)(struct _gfxdevice*dev, int x1, int y1, int x2, int y2, char*action);
    
    int (*endpage)(struct _gfxdevice*dev); //?
    
    int (*finish)(struct _gfxdevice*dev);

    void* internal;
} gfxdevice_t;

