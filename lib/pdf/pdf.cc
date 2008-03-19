#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../devices/rescale.h"
#include "../log.h"
#include "config.h"
#include "GlobalParams.h"
#include "InfoOutputDev.h"
#include "GFXOutputDev.h"
#include "FullBitmapOutputDev.h"
#include "BitmapOutputDev.h"
#include "../mem.h"
#include "pdf.h"
#define NO_ARGPARSER
#include "../args.h"

static double zoom = 72; /* xpdf: 86 */
static int jpeg_dpi = 0;
static int ppm_dpi = 0;
static int multiply = 1;
static char* global_page_range = 0;

static parameter_t* device_config = 0;
static parameter_t* device_config_next = 0;

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
    int protect;
    int nocopy;
    PDFDoc*doc;
    Object docinfo;
    InfoOutputDev*info;
    CommonOutputDev*outputDev;
    pdf_page_info_t*pages;
    gfxdevice_t* middev;
    char*filename;
} pdf_doc_internal_t;

typedef struct _pdf_page_internal
{
} pdf_page_internal_t;

typedef struct _dev_output_internal
{
    CommonOutputDev*outputDev;
} dev_output_internal_t;


static char* dirseparator()
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

void render2(gfxpage_t*page, gfxdevice_t*dev)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    
    if(pi->middev) {
	gfxdevice_rescale_setdevice(pi->middev, dev);
	pi->middev->setparameter(pi->middev, "protect", "1");
	dev = pi->middev;
    } 
	
    if(!pi) {
	msg("<fatal> pdf_page_render: Parent PDF this page belongs to doesn't exist yet/anymore");
	return;
    }

    if(!pi->pages[page->nr-1].has_info) {
	msg("<fatal> pdf_page_render: page %d was previously set as not-to-render via the \"pages\" option", page->nr);
	return;
    }

    pi->outputDev->setDevice(dev);

    if(pi->protect) {
        dev->setparameter(dev, "protect", "1");
    }
    
    /* pass global parameters to output device */
    parameter_t*p = device_config;
    while(p) {
	dev->setparameter(dev, p->name, p->value);
	p = p->next;
    }
    pi->doc->displayPage((OutputDev*)pi->outputDev, page->nr, zoom*multiply, zoom*multiply, /*rotate*/0, true, true, /*doLinks*/(int)1);
    pi->doc->processLinks((OutputDev*)pi->outputDev, page->nr);
    pi->outputDev->setDevice(0);
    if(pi->middev) {
	gfxdevice_rescale_setdevice(pi->middev, 0x00000000);
    }
}

    
void pdfpage_render(gfxpage_t*page, gfxdevice_t*output)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    pi->outputDev->setMove(0,0);
    pi->outputDev->setClip(0,0,0,0);
    render2(page, output);
}

void pdfpage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;

    int x1=(int)_x1,y1=(int)_y1,x2=(int)_x2,y2=(int)_y2;
    if((x1|y1|x2|y2)==0) x2++;

    pi->outputDev->setMove((int)x*multiply,(int)y*multiply);
    pi->outputDev->setClip((int)x1*multiply,(int)y1*multiply,(int)x2*multiply,(int)y2*multiply);
    render2(page, output);
}

void pdf_doc_destroy(gfxdocument_t*gfx)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;

    if(i->outputDev) {
	delete i->outputDev;i->outputDev=0;
    }
    if(i->middev) {
	gfxdevice_rescale_setdevice(i->middev, 0x00000000);
	i->middev->finish(i->middev);
    }
    delete i->doc; i->doc=0;
    free(i->pages); i->pages = 0;

    i->docinfo.free();

    if(i->filename) {
	free(i->filename);i->filename=0;
    }
    
    if(i->info) {
	delete i->info;i->info=0;
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

void pdf_doc_set_parameter(gfxdocument_t*gfx, const char*name, const char*value)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;
    CommonOutputDev*o = i->outputDev;
    if(!strcmp(name, "pagemap")) {
	int pdfpage=0, outputpage=0;
	sscanf(value,"%d:%d", &pdfpage, &outputpage);
	o->preparePage(pdfpage, outputpage);
    } else {
        o->setParameter(name, value);
    }
}

gfxpage_t* pdf_doc_getpage(gfxdocument_t*doc, int page)
{
    pdf_doc_internal_t*di= (pdf_doc_internal_t*)doc->internal;

    if(page < 1 || page > doc->num_pages)
        return 0;
    if(di->nocopy) {
        msg("<error> PDF disallows copying.");
        return 0;
    }
    
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

static char*getInfoString(Dict *infoDict, char *key)
{
    Object obj;
    GString *s1, *s2;
    int i;

    if (infoDict && infoDict->lookup(key, &obj)->isString()) {
	s1 = obj.getString();
	if ((s1->getChar(0) & 0xff) == 0xfe &&
	    (s1->getChar(1) & 0xff) == 0xff) {
	    s2 = new GString();
	    for (i = 2; i < obj.getString()->getLength(); i += 2) {
	      if (s1->getChar(i) == '\0') {
		s2->append(s1->getChar(i+1));
	      } else {
		delete s2;
		s2 = new GString("<unicode>");
		break;
	      }
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

static char*getInfoDate(Dict *infoDict, char *key) 
{
    Object obj;
    char *s;

    if (infoDict && infoDict->lookup(key, &obj)->isString()) {
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
        sprintf(buf, "%.1f", i->doc->getPDFVersion());
        return strdup(buf);
    }
    return 0;
}


static void storeDeviceParameter(const char*name, const char*value)
{
    parameter_t*o = device_config;
    while(o) {
        if(!strcmp(name, o->name)) {
            /* overwrite old value */
            free(o->value);
            o->value = strdup(value);
            return;
        }
        o = o->next;
    }
    parameter_t*p = new parameter_t();
    p->name = strdup(name);
    p->value = strdup(value);
    p->next = 0;

    if(device_config_next) {
	device_config_next->next = p;
	device_config_next = p;
    } else {
	device_config = p;
	device_config_next = p;
    }
}

typedef struct _gfxsource_internal
{
    int config_bitmap_optimizing;
    int config_full_bitmap_optimizing;
} gfxsource_internal_t;

static void pdf_set_parameter(gfxsource_t*src, const char*name, const char*value)
{
    gfxsource_internal_t*i = (gfxsource_internal_t*)src->internal;
    msg("<verbose> setting parameter %s to \"%s\"", name, value);
    if(!strncmp(name, "fontdir", strlen("fontdir"))) {
        addGlobalFontDir(value);
    } else if(!strcmp(name, "pages")) {
	global_page_range = strdup(value);
    } else if(!strncmp(name, "font", strlen("font"))) {
	addGlobalFont(value);
    } else if(!strncmp(name, "languagedir", strlen("languagedir"))) {
        addGlobalLanguageDir(value);
    } else if(!strcmp(name, "zoom")) {
	char buf[80];
	zoom = atof(value);
	sprintf(buf, "%f", (double)jpeg_dpi/(double)zoom);
	storeDeviceParameter("jpegsubpixels", buf);
	sprintf(buf, "%f", (double)ppm_dpi/(double)zoom);
	storeDeviceParameter("ppmsubpixels", buf);
    } else if(!strcmp(name, "jpegdpi")) {
	char buf[80];
	jpeg_dpi = atoi(value);
	sprintf(buf, "%f", (double)jpeg_dpi/(double)zoom);
	storeDeviceParameter("jpegsubpixels", buf);
    } else if(!strcmp(name, "ppmdpi")) {
	char buf[80];
	ppm_dpi = atoi(value);
	sprintf(buf, "%f", (double)ppm_dpi/(double)zoom);
	storeDeviceParameter("ppmsubpixels", buf);
    } else if(!strcmp(name, "poly2bitmap")) {
        i->config_bitmap_optimizing = atoi(value);
    } else if(!strcmp(name, "bitmapfonts") || !strcmp(name, "bitmap")) {
        i->config_full_bitmap_optimizing = atoi(value);
    } else if(!strcmp(name, "multiply")) {
        multiply = atoi(value);
    } else if(!strcmp(name, "help")) {
	printf("\nPDF device global parameters:\n");
	printf("fontdir=<dir>   a directory with additional fonts\n");
	printf("font=<filename> an dditional font filename\n");
	printf("pages=<range>   the range of pages to convert (example: pages=1-100,210-)\n");
	printf("zoom=<dpi>      the resultion (default: 72)\n");
    }	
    storeDeviceParameter(name,value);
}

static gfxdocument_t*pdf_open(gfxsource_t*src, const char*filename)
{
    gfxsource_internal_t*isrc = (gfxsource_internal_t*)src->internal;
    gfxdocument_t*pdf_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(pdf_doc, 0, sizeof(gfxdocument_t));
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)malloc(sizeof(pdf_doc_internal_t));
    memset(i, 0, sizeof(pdf_doc_internal_t));
    pdf_doc->internal = i;
    char*userPassword=0;
    
    i->filename = strdup(filename);

    char*x = 0;
    if((x = strchr(filename, '|'))) {
	*x = 0;
	userPassword = x+1;
    }
    
    GString *fileName = new GString(filename);
    GString *userPW;

    // open PDF file
    if (userPassword && userPassword[0]) {
      userPW = new GString(userPassword);
    } else {
      userPW = NULL;
    }
    i->doc = new PDFDoc(fileName, userPW);
    if (userPW) {
      delete userPW;
    }
    if (!i->doc->isOk()) {
        printf("xpdf reports document as broken.\n");
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
          if(!i->doc->okToChange() || !i->doc->okToAddNotes())
              i->protect = 1;
    }

    i->info = new InfoOutputDev(i->doc->getXRef());
    int t;
    i->pages = (pdf_page_info_t*)malloc(sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    memset(i->pages,0,sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    for(t=1;t<=pdf_doc->num_pages;t++) {
	if(!global_page_range || is_in_range(t, global_page_range)) {
	    i->doc->displayPage((OutputDev*)i->info, t, zoom, zoom, /*rotate*/0, /*usemediabox*/true, /*crop*/true, /*doLinks*/(int)1);
	    i->doc->processLinks((OutputDev*)i->info, t);
	    i->pages[t-1].xMin = i->info->x1;
	    i->pages[t-1].yMin = i->info->y1;
	    i->pages[t-1].xMax = i->info->x2;
	    i->pages[t-1].yMax = i->info->y2;
	    i->pages[t-1].width = i->info->x2 - i->info->x1;
	    i->pages[t-1].height = i->info->y2 - i->info->y1;
	    i->pages[t-1].number_of_images = i->info->num_images;
	    i->pages[t-1].number_of_links = i->info->num_links;
	    i->pages[t-1].number_of_fonts = i->info->num_fonts;
	    i->pages[t-1].has_info = 1;
	}
    }

    if(isrc->config_full_bitmap_optimizing) {
	FullBitmapOutputDev*outputDev = new FullBitmapOutputDev(i->info, i->doc);
	i->outputDev = (CommonOutputDev*)outputDev;
    } else if(isrc->config_bitmap_optimizing) {
	BitmapOutputDev*outputDev = new BitmapOutputDev(i->info, i->doc);
	i->outputDev = (CommonOutputDev*)outputDev;
    } else {
	GFXOutputDev*outputDev = new GFXOutputDev(i->info, i->doc);
	i->outputDev = (CommonOutputDev*)outputDev;
    }

    /* pass global parameters to PDF driver*/
    parameter_t*p = device_config;
    while(p) {
	i->outputDev->setParameter(p->name, p->value);
	p = p->next;
    }

    i->middev = 0;
    if(multiply>1) {
    	i->middev = (gfxdevice_t*)malloc(sizeof(gfxdevice_t));
	gfxdevice_rescale_init(i->middev, 0x00000000, 0, 0, 1.0 / multiply);
    }

    pdf_doc->get = 0;
    pdf_doc->destroy = pdf_doc_destroy;
    pdf_doc->set_parameter = pdf_doc_set_parameter;
    pdf_doc->getinfo = pdf_doc_getinfo;
    pdf_doc->getpage = pdf_doc_getpage;


    return pdf_doc;

}
    
void pdf_destroy(gfxsource_t*src)
{
    if(!src->internal)
	return;
    gfxsource_internal_t*i = (gfxsource_internal_t*)src->internal;
    free(src->internal);src->internal=0;
    
    parameter_t*p = device_config;
    while(p) {
	parameter_t*next = p->next;
	p->next = 0;free(p);
	p = next;
    }
    delete globalParams;globalParams = 0;
}

gfxsource_t*gfxsource_pdf_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->set_parameter = pdf_set_parameter;
    src->open = pdf_open;
    src->destroy = pdf_destroy;
    src->internal = malloc(sizeof(gfxsource_internal_t));
    memset(src->internal, 0, sizeof(gfxsource_internal_t));

    if(!globalParams) {
        globalParams = new GFXGlobalParams();
	//globalparams_count++;
    }
    

    return src;
}
