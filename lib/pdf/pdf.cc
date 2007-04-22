#include "../gfxdevice.h"
#include "../gfxsource.h"
#include "../log.h"
#include "config.h"
#include "GlobalParams.h"
#include "InfoOutputDev.h"
#include "GFXOutputDev.h"
#include "../mem.h"
#include "pdf.h"
#define NO_ARGPARSER
#include "../args.h"

static parameter_t* device_config = 0;
static parameter_t* device_config_next = 0;

int jpeg_dpi = 0;
int ppm_dpi = 0;

static double zoom = 72; /* xpdf: 86 */

static char* global_page_range = 0;

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
    PDFDoc*doc;
    InfoOutputDev*info;
    GFXOutputDev*outputDev;
    pdf_page_info_t*pages;
} pdf_doc_internal_t;

typedef struct _pdf_page_internal
{
} pdf_page_internal_t;

typedef struct _dev_output_internal
{
    GFXOutputDev*outputDev;
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

void render2(gfxpage_t*page, gfxdevice_t*output)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;

    if(!pi) {
	msg("<fatal> pdf_page_render: Parent PDF this page belongs to doesn't exist yet/anymore");
	return;
    }

    if(!pi->pages[page->nr-1].has_info) {
	msg("<fatal> pdf_page_render: page %d was previously set as not-to-render via the \"pages\" option", page->nr);
	return;
    }

    if(pi->protect) {
	gfxdevice_t*dev = pi->outputDev->device;
        dev->setparameter(dev, "protect", "1");
    }
    pi->outputDev->setInfo(pi->info);
    pi->outputDev->setXRef(pi->doc, pi->doc->getXRef());
    pi->doc->displayPage((OutputDev*)pi->outputDev, page->nr, zoom, zoom, /*rotate*/0, true, true, /*doLinks*/(int)1);
    pi->doc->processLinks((OutputDev*)pi->outputDev, page->nr);
}

    
void pdfpage_render(gfxpage_t*page, gfxdevice_t*output)
{
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    pi->outputDev->setDevice(output);
    pi->outputDev->setMove(0,0);
    pi->outputDev->setClip(0,0,0,0);
    render2(page, output);
    pi->outputDev->setDevice(0);
}

void pdfpage_rendersection(gfxpage_t*page, gfxdevice_t*output, gfxcoord_t x, gfxcoord_t y, gfxcoord_t _x1, gfxcoord_t _y1, gfxcoord_t _x2, gfxcoord_t _y2)
{
    int x1=(int)_x1,y1=(int)_y1,x2=(int)_x2,y2=(int)_y2;
    pdf_doc_internal_t*pi = (pdf_doc_internal_t*)page->parent->internal;
    pi->outputDev->setDevice(output);
    pi->outputDev->setMove((int)x,(int)y);
    if((x1|y1|x2|y2)==0) x2++;
    pi->outputDev->setClip((int)x1,(int)y1,(int)x2,(int)y2);
    render2(page, output);
    pi->outputDev->setDevice(0);
}

void pdf_doc_destroy(gfxdocument_t*gfx)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;

    delete i->doc; i->doc=0;
    free(i->pages); i->pages = 0;
    
    if(i->info) {
	delete i->info;i->info=0;
    }

    free(gfx->internal);gfx->internal=0;
    free(gfx);gfx=0;

    if(global_page_range) {
	free(global_page_range);
	global_page_range = 0;
    }
}

void pdf_doc_set_parameter(gfxdocument_t*gfx, char*name, char*value)
{
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)gfx->internal;
    if(!strcmp(name, "pagemap")) {
	GFXOutputDev*o = i->outputDev;
	int pdfpage=0, outputpage=0;
	sscanf(value,"%d:%d", &pdfpage, &outputpage);
	o->preparePage(pdfpage, outputpage);
    } else {
	msg("<warning> Ignored parameter: %s=%s", name, value);
    }
}

gfxpage_t* pdf_doc_getpage(gfxdocument_t*doc, int page)
{
    pdf_doc_internal_t*di= (pdf_doc_internal_t*)doc->internal;

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

void storeDeviceParameter(char*name, char*value)
{
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

void pdf_set_parameter(char*name, char*value)
{
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
    } else {
	storeDeviceParameter(name,value);
    }
}

static void printInfoString(Dict *infoDict, char *key, char *fmt) {
  Object obj;
  GString *s1, *s2;
  int i;

  if (infoDict->lookup(key, &obj)->isString()) {
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
      printf(fmt, s2->getCString());
      delete s2;
    } else {
      printf(fmt, s1->getCString());
    }
  }
  obj.free();
}

static void printInfoDate(Dict *infoDict, char *key, char *fmt) {
  Object obj;
  char *s;

  if (infoDict->lookup(key, &obj)->isString()) {
    s = obj.getString()->getCString();
    if (s[0] == 'D' && s[1] == ':') {
      s += 2;
    }
    printf(fmt, s);
  }
  obj.free();
}



gfxdocument_t*pdf_open(char*filename)
{
    gfxdocument_t*pdf_doc = (gfxdocument_t*)malloc(sizeof(gfxdocument_t));
    memset(pdf_doc, 0, sizeof(gfxdocument_t));
    pdf_doc_internal_t*i= (pdf_doc_internal_t*)malloc(sizeof(pdf_doc_internal_t));
    memset(i, 0, sizeof(pdf_doc_internal_t));
    pdf_doc->internal = i;
    char*userPassword=0;
    
    filename = strdup(filename);

    char*x = 0;
    if((x = strchr(filename, '|'))) {
	*x = 0;
	userPassword = x+1;
    }
    
    GString *fileName = new GString(filename);
    GString *userPW;
    Object info;

    // read config file
    if(!globalParams)
        globalParams = new GlobalParams("");

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

    // print doc info
    i->doc->getDocInfo(&info);
    if (info.isDict() &&
      (getScreenLogLevel()>=LOGLEVEL_NOTICE)) {
      printInfoString(info.getDict(), "Title",        "Title:        %s\n");
      printInfoString(info.getDict(), "Subject",      "Subject:      %s\n");
      printInfoString(info.getDict(), "Keywords",     "Keywords:     %s\n");
      printInfoString(info.getDict(), "Author",       "Author:       %s\n");
      printInfoString(info.getDict(), "Creator",      "Creator:      %s\n");
      printInfoString(info.getDict(), "Producer",     "Producer:     %s\n");
      printInfoDate(info.getDict(),   "CreationDate", "CreationDate: %s\n");
      printInfoDate(info.getDict(),   "ModDate",      "ModDate:      %s\n");
      printf("Pages:        %d\n", i->doc->getNumPages());
      printf("Linearized:   %s\n", i->doc->isLinearized() ? "yes" : "no");
      printf("Encrypted:    ");
      if (i->doc->isEncrypted()) {
        printf("yes (print:%s copy:%s change:%s addNotes:%s)\n",
               i->doc->okToPrint() ? "yes" : "no",
               i->doc->okToCopy() ? "yes" : "no",
               i->doc->okToChange() ? "yes" : "no",
               i->doc->okToAddNotes() ? "yes" : "no");
      } else {
        printf("no\n");
      }
    }
    info.free();
                   
    pdf_doc->num_pages = i->doc->getNumPages();
    i->protect = 0;
    if (i->doc->isEncrypted()) {
          if(!i->doc->okToCopy()) {
              printf("PDF disallows copying.\n");
              return 0;
          }
          if(!i->doc->okToChange() || !i->doc->okToAddNotes())
              i->protect = 1;
    }

    InfoOutputDev*io = new InfoOutputDev();
    int t;
    i->pages = (pdf_page_info_t*)malloc(sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    memset(i->pages,0,sizeof(pdf_page_info_t)*pdf_doc->num_pages);
    for(t=1;t<=pdf_doc->num_pages;t++) {
	if(!global_page_range || is_in_range(t, global_page_range)) {
	    i->doc->displayPage((OutputDev*)io, t, zoom, zoom, /*rotate*/0, /*usemediabox*/true, /*crop*/true, /*doLinks*/(int)1);
	    i->doc->processLinks((OutputDev*)io, t);
	    i->pages[t-1].xMin = io->x1;
	    i->pages[t-1].yMin = io->y1;
	    i->pages[t-1].xMax = io->x2;
	    i->pages[t-1].yMax = io->y2;
	    i->pages[t-1].width = io->x2 - io->x1;
	    i->pages[t-1].height = io->y2 - io->y1;
	    i->pages[t-1].number_of_images = io->num_images;
	    i->pages[t-1].number_of_links = io->num_links;
	    i->pages[t-1].number_of_fonts = io->num_fonts;
	    i->pages[t-1].has_info = 1;
	}
    }
    i->info = io;
    i->outputDev = new GFXOutputDev(device_config);

    pdf_doc->get = 0;
    pdf_doc->destroy = pdf_doc_destroy;
    pdf_doc->set_parameter = pdf_doc_set_parameter;
    pdf_doc->getpage = pdf_doc_getpage;


    return pdf_doc;

}

gfxsource_t*gfxsource_pdf_create()
{
    gfxsource_t*src = (gfxsource_t*)malloc(sizeof(gfxsource_t));
    memset(src, 0, sizeof(gfxsource_t));
    src->set_parameter = pdf_set_parameter;
    src->open = pdf_open;
    return src;
}
