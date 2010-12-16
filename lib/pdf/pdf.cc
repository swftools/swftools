#include <stdio.h>
#include <string.h>
#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../devices/rescale.h"
#include "../log.h"
#include "../../config.h"
#ifdef HAVE_POPPLER
  #include <poppler-config.h>
#else
  #include "xpdf/config.h"
#endif
#include "GlobalParams.h"
#include "InfoOutputDev.h"
#include "CharOutputDev.h"
#include "FullBitmapOutputDev.h"
#include "BitmapOutputDev.h"
#include "VectorGraphicOutputDev.h"
#include "../mem.h"
#include "pdf.h"
#define NO_ARGPARSER
#include "../args.h"
#include "../utf8.h"

static double zoom = 72; /* xpdf: 86 */
static int zoomtowidth = 0;
static double multiply = 1.0;
static char* global_page_range = 0;
static int threadsafe = 0;

static int globalparams_count=0;

typedef struct _pdf_page_info
{
    int xMin, yMin, xMax, yMax;
    int width,height;
    int number_of_images;
    int number_of_links;
    int number_of_fonts;
    char has_info;
} pdf_page_info_t;

typedef struct _pdf_doc_internal
{
    char config_bitmap_optimizing;
    char config_full_bitmap_optimizing;
    char config_only_text;
    char config_print;
    gfxparams_t* parameters;

    int protect;
    int nocopy;
    int noprint;
   
    GString*fileName;
    GString*userPW;
    PDFDoc*doc;

    Object docinfo;
    InfoOutputDev*info;

    pdf_page_info_t*pages;
    char*filename;

    /* page map */
    int*pagemap;
    int pagemap_size;
    int pagemap_pos;

    gfxsource_t*parent;
} pdf_doc_internal_t;

typedef struct _pdf_page_internal
{
} pdf_page_internal_t;

typedef struct _dev_output_internal
{
    CommonOutputDev*outputDev;
} dev_output_internal_t;


typedef struct _gfxsource_internal
{
    gfxparams_t* parameters;
} gfxsource_internal_t;


static const char* dirseparator()
{
#ifdef WIN32
    return "\\";
#else
    return "/";
#endif
}

void pdfpage_destroy(gfxpage_t*pdf_page)
{
    pdf_page_internal_t*i= (pdf_page_internal_t*)pdf_page->internal;
    free(pdf_page->internal);pdf_page->internal = 0;
    free(pdf_page);pdf_page=0;
}

static void render2(gfxpage_t*page, gfxdevice_t*dev, int x,int y, int x1,int y1,int x2,int y2)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    gfxsource_internal_t*i = (gfxsource_internal_t*)pi->parent->internal;

    if(!pi->config_print && pi->nocopy) {msg("<fatal> PDF disallows copying");exit(0);}
    if(pi->config_print && pi->noprint) {msg("<fatal> PDF disallows printing");exit(0);}

    CommonOutputDev*outputDev = 0;
    if(pi->config_full_bitmap_optimizing) {
	FullBitmapOutputDev*d = new FullBitmapOutputDev(pi->info, pi->doc, pi->pagemap, pi->pagemap_pos, x, y, x1, y1, x2, y2);
	outputDev = (CommonOutputDev*)d;
    } else if(pi->config_bitmap_optimizing) {
	BitmapOutputDev*d = new BitmapOutputDev(pi->info, pi->doc, pi->pagemap, pi->pagemap_pos, x, y, x1, y1, x2, y2);
	outputDev = (CommonOutputDev*)d;
    } else if(pi->config_only_text) {
	CharOutputDev*d = new CharOutputDev(pi->info, pi->doc, pi->pagemap, pi->pagemap_pos, x, y, x1, y1, x2, y2);
	outputDev = (CommonOutputDev*)d;
    } else {
	VectorGraphicOutputDev*d = new VectorGraphicOutputDev(pi->info, pi->doc, pi->pagemap, pi->pagemap_pos, x, y, x1, y1, x2, y2);
	outputDev = (CommonOutputDev*)d;
    }

    /* pass global parameters to PDF driver*/
    gfxparam_t*p = i->parameters->params;
    while(p) {
	outputDev->setParameter(p->key, p->value);
	p = p->next;
    }
    p = pi->parameters->params;
    while(p) {
	outputDev->setParameter(p->key, p->value);
	p = p->next;
    }

    gfxdevice_t* middev=0;
    if(multiply!=1.0) {
    	middev = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
	gfxdevice_rescale_init(middev, 0x00000000, 0, 0, 1.0 / multiply);
        gfxdevice_rescale_setdevice(middev, dev);
	dev = middev;
    } 
	
    if(!pi) {
	msg("<fatal> pdf_page_render: Parent PDF this page belongs to doesn't exist yet/anymore");
	return;
    }

    if(!pi->pages[page->nr-1].has_info) {
	msg("<fatal> pdf_page_render: page %d was previously set as not-to-render via the \"pages\" option", page->nr);
	return;
    }

    if(pi->protect) {
        dev->setparameter(dev, "protect", "1");
    }

    outputDev->setDevice(dev);
    pi->doc->processLinks((OutputDev*)outputDev, page->nr);
    pi->doc->displayPage((OutputDev*)outputDev, page->nr, zoom*multiply, zoom*multiply, /*rotate*/0, true, true, pi->config_print);
    outputDev->finishPage();
    outputDev->setDevice(0);
    delete outputDev;

    if(middev) {
	gfxdevice_rescale_setdevice(middev, 0x00000000);
	middev->finish(middev);
    }

}

    
void pdfpage_render(gfxpage_t*page, gfxdevice_t*output)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    render2(page, output, 0,0, 0,0,0,0);
}

void pdfpage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;

    int x1=(int)_x1,y1=(int)_y1,x2=(int)_x2,y2=(int)_y2;
    if((x1|y1|x2|y2)==0) x2++;

    render2(page, output, (int)x*multiply,(int)y*multiply,
                          (int)x1*multiply,(int)y1*multiply,(int)x2*multiply,(int)y2*multiply);
}

void pdf_doc_destroy(gfxdocument_t*gfx)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;

    if (i->userPW) {
	delete i->userPW;i->userPW = 0;
    }
    if (i->fileName) {
	/* will be freed by PDFDoc::~PDFDoc */
	i->fileName = 0;
    }
   
    if(i->doc) {
	delete i->doc; i->doc=0;
    }
    free(i->pages); i->pages = 0;
    
    if(i->pagemap) {
	free(i->pagemap);
    }

    i->docinfo.free();

    if(i->filename) {
	free(i->filename);i->filename=0;
    }
    
    if(i->info) {
	delete i->info;i->info=0;
    }
    if(i->parameters) {
	gfxparams_free(i->parameters);
	i->parameters=0;
    }

    free(gfx->internal);gfx->internal=0;
    free(gfx);gfx=0;

    if(global_page_range) {
	free(global_page_range);
	global_page_range = 0;
    }
    
    /*globalparams_count--;
    if(!globalparams_count) {
	delete globalParams;
	globalParams = 0;
	globalparams_count = 0;
    }*/
}

static void add_page_to_map(gfxdocument_t*gfx, int pdfpage, int outputpage)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;
    if(pdfpage < 0)
	return;
    if(pdfpage >= i->pagemap_size) {
	int oldlen = i->pagemap_size;
	i->pagemap_size = oldlen + 1024;
	if(pdfpage > i->pagemap_size)
	    i->pagemap_size = pdfpage+1;

        if(i->pages) {
            i->pagemap = (int*)malloc(i->pagemap_size*sizeof(int));
        } else {
	    i->pagemap = (int*)realloc(i->pages, i->pagemap_size*sizeof(int));
        }
	memset(&i->pagemap[oldlen], -1, (i->pagemap_size-oldlen)*sizeof(int));
    }
    i->pagemap[pdfpage] = outputpage;
    if(pdfpage > i->pagemap_pos)
	i->pagemap_pos = pdfpage;
}

void pdf_doc_setparameter(gfxdocument_t*gfx, const char*name, const char*value)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;
    if(!strcmp(name, "pagemap")) {
	int pdfpage=0, outputpage=0;
	sscanf(value,"%d:%d", &pdfpage, &outputpage);
        add_page_to_map(gfx, pdfpage, outputpage);
    } else if(!strcmp(name, "poly2bitmap")) {
        i->config_bitmap_optimizing = atoi(value);
    } else if(!strcmp(name, "bitmapfonts") || !strcmp(name, "bitmap")) {
        i->config_full_bitmap_optimizing = atoi(value);
    } else if(!strcmp(name, "asprint")) {
        i->config_print = atoi(value);
    } else if(!strcmp(name, "onlytext")) {
        i->config_only_text = atoi(value);
    } else {
        gfxparams_store(i->parameters, name, value);
    }
}

gfxpage_t* pdf_doc_getpage(gfxdocument_t*doc, int page)
{
    pdf_doc_internal_t*di= (pdf_doc_internal_t*)doc->internal;
    if(threadsafe) {
	/* for multi-thread operation, we need to create a new PDFDoc instance
	   for each thread */
	di->doc = 0;
    }
    if(!di->doc) {
	di->doc = new PDFDoc(di->fileName, di->userPW);
    }

    if(page < 1 || page > doc->num_pages)
        return 0;
    
    gfxpage_t* pdf_page = (gfxpage_t*)malloc(sizeof(gfxpage_t));
    pdf_page_internal_t*pi= (pdf_page_internal_t*)malloc(sizeof(pdf_page_internal_t));
    memset(pi, 0, sizeof(pdf_page_internal_t));
    pdf_page->internal = pi;

    pdf_page->destroy = pdfpage_destroy;
    pdf_page->render = pdfpage_render;
    pdf_page->rendersection = pdfpage_rendersection;
    pdf_page->width = di->pages[page-1].width;
    pdf_page->height = di->pages[page-1].height;

    pdf_page->parent = doc;
    pdf_page->nr = page;
    return pdf_page;
}

static char*getInfoString(Dict *infoDict, const char *key)
{
    Object obj;
    GString *s1, *s2;
    int i;
    unsigned int u;

    if (infoDict && infoDict->lookup((char*)key, &obj)->isString()) {
	s1 = obj.getString();
	if ((s1->getChar(0) & 0xff) == 0xfe &&
	    (s1->getChar(1) & 0xff) == 0xff) {
	    s2 = new GString();
	    for (i = 2; i < obj.getString()->getLength(); i += 2) {
              u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
              s2->append(getUTF8(u));
	    }
	    char*ret = strdup(s2->getCString());
	    delete s2;
	    obj.free();
	    return ret;
	} else {
	    char*ret = strdup(s1->getCString());
	    obj.free();
	    return ret;
	}
    }
    return strdup("");
}

static char*getInfoDate(Dict *infoDict, const char *key) 
{
    Object obj;
    char *s;

    if (infoDict && infoDict->lookup((char*)key, &obj)->isString()) {
	s = obj.getString()->getCString();
	if (s[0] == 'D' && s[1] == ':') {
	  s += 2;
	}
	char*ret = strdup(s);
	obj.free();
	return ret;
    }
    return strdup("");
}

char* pdf_doc_getinfo(gfxdocument_t*doc, const char*name)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)doc->internal;
    if(!strcmp(name, "title")) return getInfoString(i->docinfo.getDict(), "Title");
    else if(!strcmp(name, "subject")) return getInfoString(i->docinfo.getDict(), "Subject");
    else if(!strcmp(name, "keywords")) return getInfoString(i->docinfo.getDict(), "Keywords");
    else if(!strcmp(name, "author")) return getInfoString(i->docinfo.getDict(), "Author");
    else if(!strcmp(name, "creator")) return getInfoString(i->docinfo.getDict(), "Creator");
    else if(!strcmp(name, "producer")) return getInfoString(i->docinfo.getDict(), "Producer");
    else if(!strcmp(name, "creationdate")) return getInfoDate(i->docinfo.getDict(), "CreationDate");
    else if(!strcmp(name, "moddate")) return getInfoDate(i->docinfo.getDict(), "ModDate");
    else if(!strcmp(name, "linearized")) return strdup(i->doc->isLinearized() ? "yes" : "no");
    else if(!strcmp(name, "tagged")) return strdup(i->doc->getStructTreeRoot()->isDict() ? "yes" : "no");
    else if(!strcmp(name, "encrypted")) return strdup(i->doc->isEncrypted() ? "yes" : "no");
    else if(!strcmp(name, "oktoprint")) return strdup(i->doc->okToPrint() ? "yes" : "no");
    else if(!strcmp(name, "oktocopy")) return strdup(i->doc->okToCopy() ? "yes" : "no");
    else if(!strcmp(name, "oktochange")) return strdup(i->doc->okToChange() ? "yes" : "no");
    else if(!strcmp(name, "oktoaddnotes")) return strdup(i->doc->okToAddNotes() ? "yes" : "no");
    else if(!strcmp(name, "version")) { 
        char buf[32];
#ifdef HAVE_POPPLER
        sprintf(buf, "%d.%d", i->doc->getPDFMajorVersion(), i->doc->getPDFMinorVersion());
#else
        sprintf(buf, "%.1f", i->doc->getPDFVersion());
#endif
        return strdup(buf);
    }
    return strdup("");
}


/* shortcut to InfoOutputDev.cc */
extern int config_unique_unicode;
extern int config_poly2bitmap_pass1;
extern int config_skewedtobitmap_pass1;
extern int config_addspace;
extern int config_fontquality;
extern int config_bigchar;
extern int config_marker_glyph;
extern int config_normalize_fonts;
extern int config_remove_font_transforms;
extern int config_remove_invisible_outlines;
extern int config_break_on_warning;

static void pdf_setparameter(gfxsource_t*src, const char*name, const char*value)
{
    gfxsource_internal_t*i = (gfxsource_internal_t*)src->internal;
        
    gfxparams_store(i->parameters, name, value);

    msg("<verbose> setting parameter %s to \"%s\"", name, value);
    if(!strncmp(name, "fontdir", strlen("fontdir"))) {
        addGlobalFontDir(value);
    } else if(!strcmp(name, "addspacechars")) {
	config_addspace = atoi(value);
	gfxparams_store(i->parameters, "detectspaces", "0");
    } else if(!strcmp(name, "detectspaces")) {
	config_addspace = atoi(value);
    } else if(!strcmp(name, "unique_unicode")) {
	config_unique_unicode = atoi(value);
    } else if(!strcmp(name, "poly2bitmap")) {
        config_poly2bitmap_pass1 = atoi(value);
    } else if(!strcmp(name, "marker_glyph")) {
	config_marker_glyph = atoi(value);
    } else if(!strcmp(name, "normalize_fonts")) {
	config_normalize_fonts = atoi(value);
    } else if(!strcmp(name, "skewedtobitmap")) {
	config_skewedtobitmap_pass1 = atoi(value);
    } else if(!strcmp(name, "remove_font_transforms")) {
	config_remove_font_transforms = atoi(value);
    } else if(!strcmp(name, "breakonwarning")) {
	config_break_on_warning = atoi(value);
    } else if(!strcmp(name, "remove_invisible_outlines")) {
	config_remove_invisible_outlines = atoi(value);
    } else if(!strcmp(name, "fontquality")) {
	config_fontquality = atoi(value);
    } else if(!strcmp(name, "bigchar")) {
	config_bigchar = atoi(value);
    } else if(!strcmp(name, "pages")) {
	global_page_range = strdup(value);
    } else if(!strncmp(name, "font", strlen("font")) && name[4]!='q') {
	addGlobalFont(value);
    } else if(!strncmp(name, "languagedir", strlen("languagedir"))) {
        addGlobalLanguageDir(value);
    } else if(!strcmp(name, "threadsafe")) {
	threadsafe = atoi(value);
    } else if(!strcmp(name, "zoomtowidth")) {
	zoomtowidth = atoi(value);
    } else if(!strcmp(name, "zoom")) {
	char buf[80];
	zoom = atof(value);
    } else if(!strcmp(name, "jpegdpi") || !strcmp(name, "ppmdpi")) {
	msg("<error> %s not supported anymore. Please use jpegsubpixels/ppmsubpixels");
    } else if(!strcmp(name, "multiply")) {
        multiply = atof(value);
    } else if(!strcmp(name, "help")) {
	printf("\nPDF device global parameters:\n");
	printf("fontdir=<dir>     a directory with additional fonts\n");
	printf("font=<filename>   an additional font filename\n");
	printf("pages=<range>     the range of pages to convert (example: pages=1-100,210-)\n");
	printf("zoom=<dpi>        the resultion (default: 72)\n");
	printf("languagedir=<dir> Add an xpdf language directory\n");
	printf("multiply=<times>  Render everything at <times> the resolution\n");
	printf("poly2bitmap       Convert graphics to bitmaps\n");
	printf("bitmap            Convert everything to bitmaps\n");
    }	
}

void pdf_doc_prepare(gfxdocument_t*doc, gfxdevice_t*dev)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)doc->internal;
    i->info->dumpfonts(dev);
}

static gfxdocument_t*pdf_open(gfxsource_t*src, const char*filename)
{
    gfxsource_internal_t*isrc = (gfxsource_internal_t*)src->internal;
    gfxdocument_t*pdf_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(pdf_doc, 0, sizeof(gfxdocument_t));
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)malloc(sizeof(pdf_doc_internal_t));
    memset(i, 0, sizeof(pdf_doc_internal_t));
    i->parent = src;
    i->parameters = gfxparams_new();
    pdf_doc->internal = i;
    char*userPassword=0;
    
    i->filename = strdup(filename);

    char*x = 0;
    if((x = strchr((char*)filename, '|'))) {
	*x = 0;
	userPassword = x+1;
    }
    
    i->fileName = new GString(filename);

    // open PDF file
    if (userPassword && userPassword[0]) {
      i->userPW = new GString(userPassword);
    } else {
      i->userPW = NULL;
    }
    i->doc = new PDFDoc(i->fileName, i->userPW);
    if (!i->doc->isOk()) {
        return 0;
    }

    // get doc info
    i->doc->getDocInfo(&i->docinfo);
    
    pdf_doc->num_pages = i->doc->getNumPages();
    i->protect = 0;
    if (i->doc->isEncrypted()) {
          if(!i->doc->okToCopy()) {
              i->nocopy = 1;
          }
          if(!i->doc->okToPrint()) {
              i->noprint = 1;
          }
          if(!i->doc->okToChange() || !i->doc->okToAddNotes())
              i->protect = 1;
    }
	
    if(zoomtowidth && i->doc->getNumPages()) {
	Page*page = i->doc->getCatalog()->getPage(1);
	PDFRectangle *r = page->getCropBox();
	double width_before = r->x2 - r->x1;
	zoom = 72.0 * zoomtowidth / width_before;
	msg("<notice> Rendering at %f DPI. (Page width at 72 DPI: %f, target width: %d)", zoom, width_before, zoomtowidth);
    }

    i->info = new InfoOutputDev(i->doc->getXRef());
    int t;
    i->pages = (pdf_page_info_t*)malloc(sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    memset(i->pages,0,sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    for(t=1;t<=pdf_doc->num_pages;t++) {
	if(!global_page_range || is_in_range(t, global_page_range)) {
	    i->doc->displayPage((OutputDev*)i->info, t, zoom, zoom, /*rotate*/0, /*usemediabox*/true, /*crop*/true, i->config_print);
	    i->doc->processLinks((OutputDev*)i->info, t);
	    i->pages[t-1].xMin = i->info->x1;
	    i->pages[t-1].yMin = i->info->y1;
	    i->pages[t-1].xMax = i->info->x2;
	    i->pages[t-1].yMax = i->info->y2;
	    i->pages[t-1].width = i->info->x2 - i->info->x1;
	    i->pages[t-1].height = i->info->y2 - i->info->y1;
	    i->pages[t-1].number_of_images = i->info->num_ppm_images + i->info->num_jpeg_images;
	    i->pages[t-1].number_of_links = i->info->num_links;
	    i->pages[t-1].number_of_fonts = i->info->num_fonts;
	    i->pages[t-1].has_info = 1;
	}
    }

    pdf_doc->get = 0;
    pdf_doc->destroy = pdf_doc_destroy;
    pdf_doc->setparameter = pdf_doc_setparameter;
    pdf_doc->getinfo = pdf_doc_getinfo;
    pdf_doc->getpage = pdf_doc_getpage;
    pdf_doc->prepare = pdf_doc_prepare;

    /* pass global parameters to PDF driver*/
    gfxparam_t*p = isrc->parameters->params;
    while(p) {
	pdf_doc->setparameter(pdf_doc, p->key, p->value);
	p = p->next;
    }
    return pdf_doc;
}
    
void pdf_destroy(gfxsource_t*src)
{
    if(!src->internal)
	return;
    gfxsource_internal_t*i = (gfxsource_internal_t*)src->internal;
   
    gfxparams_free(i->parameters);
    i->parameters=0;
    
    free(src->internal);src->internal=0;

    delete globalParams;globalParams = 0;
    free(src);
}

gfxsource_t*gfxsource_pdf_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->setparameter = pdf_setparameter;
    src->open = pdf_open;
    src->destroy = pdf_destroy;
    gfxsource_internal_t*i = (gfxsource_internal_t*)rfx_calloc(sizeof(gfxsource_internal_t));
    src->internal = (void*)i;
    i->parameters = gfxparams_new();

    if(!globalParams) {
        globalParams = new GFXGlobalParams();
	//globalparams_count++;
    }
    

    return src;
}
