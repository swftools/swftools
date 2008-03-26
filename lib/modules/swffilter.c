#include <stdlib.h>
#include <stdio.h>
#include "../rfxswf.h"

char* filtername[] = {"dropshadow","blur","glow","bevel","gradientglow","convolution","colormatrix","gradientbevel", 0};

void swf_SetFilter(TAG*tag, FILTER*filter)
{
    swf_SetU8(tag, filter->type);
    if(filter->type == FILTERTYPE_BLUR) {
	FILTER_BLUR*f = (FILTER_BLUR*)filter;
	swf_SetFixed(tag, f->blurx);
	swf_SetFixed(tag, f->blury);
	U8 flags = f->passes << 3;
	swf_SetU8(tag, flags);
    } else if(filter->type == FILTERTYPE_GLOW) {
	FILTER_GLOW*f = (FILTER_GLOW*)filter;
    } else if(filter->type == FILTERTYPE_DROPSHADOW) {
	FILTER_DROPSHADOW*f = (FILTER_DROPSHADOW*)filter;
	swf_SetRGBA(tag, &f->color);
	swf_SetFixed(tag, f->blurx);
	swf_SetFixed(tag, f->blury);
	swf_SetFixed(tag, f->angle);
	swf_SetFixed(tag, f->distance);
	swf_SetFixed8(tag, f->strength);
	U8 flags = f->innershadow<<7|f->knockout<<6|f->composite<<5|f->passes;
	swf_SetU8(tag, flags);
    } else if(filter->type == FILTERTYPE_GRADIENTGLOW) {
	FILTER_GRADIENTGLOW*f = (FILTER_GRADIENTGLOW*)filter;
	swf_SetU8(tag, f->gradient->num);
	int s;
	for(s=0;s<f->gradient->num;s++)
	    swf_SetRGBA(tag, &f->gradient->rgba[s]);
	for(s=0;s<f->gradient->num;s++)
	    swf_SetU8(tag, f->gradient->ratios[s]);

	swf_SetFixed(tag, f->blurx);
	swf_SetFixed(tag, f->blury);
	swf_SetFixed(tag, f->angle);
	swf_SetFixed(tag, f->distance);
	swf_SetFixed8(tag, f->strength);
	U8 flags = f->passes|f->innershadow<<7|f->knockout<<6|f->composite<<5|f->ontop<<4;
	swf_SetU8(tag, flags);
    } else if(filter->type == FILTERTYPE_BEVEL) {
	FILTER_BEVEL*f = (FILTER_BEVEL*)filter;
	swf_SetRGBA(tag, &f->shadow);
	swf_SetRGBA(tag, &f->highlight);
	swf_SetFixed(tag, f->blurx);
	swf_SetFixed(tag, f->blury);
	swf_SetFixed(tag, f->angle);
	swf_SetFixed(tag, f->distance);
	swf_SetFixed8(tag, f->strength);
	U8 flags = f->passes|f->innershadow<<7|f->knockout<<6|f->composite<<5|f->ontop<<4;
	swf_SetU8(tag, flags);
    } else {
	fprintf(stderr, "Writing of filter type %02x not supported yet\n", filter->type);
    }
}

FILTER*swf_GetFilter(TAG*tag)
{
    U8 type = swf_GetU8(tag);
    FILTER*filter;
    if(type == FILTERTYPE_BLUR) {
	FILTER_BLUR* f = (FILTER_BLUR*)rfx_calloc(sizeof(FILTER_BLUR));
	f->type = type;
	f->blurx = swf_GetFixed(tag);
	f->blury = swf_GetFixed(tag);
	U8 flags = swf_GetU8(tag);
	f->passes = (flags&15)<<3;
	return (FILTER*)f;
    } else if(type == FILTERTYPE_GLOW) {
	FILTER_GLOW* f = (FILTER_GLOW*)rfx_calloc(sizeof(FILTER_GLOW));
	f->type = type;
	swf_GetRGBA(tag, &f->rgba);
	f->blurx = swf_GetFixed(tag);
	f->blury = swf_GetFixed(tag);
	f->strength = swf_GetFixed8(tag);
	U8 flags = swf_GetU8(tag);
	f->passes = flags&31;
	f->innerglow = (flags>>7)&1;
	f->knockout = (flags>>6)&1;
	f->composite = (flags>>5)&1;
	return (FILTER*)f;
    } else if(type == FILTERTYPE_GRADIENTGLOW) {
	FILTER_GRADIENTGLOW* f = (FILTER_GRADIENTGLOW*)rfx_calloc(sizeof(FILTER_GRADIENTGLOW));
	f->type = type;
	f->gradient = (GRADIENT*)rfx_calloc(sizeof(GRADIENT));
	f->gradient->num = swf_GetU8(tag);
	f->gradient->rgba = (RGBA*)rfx_calloc(sizeof(RGBA)*f->gradient->num);
	f->gradient->ratios = (U8*)rfx_calloc(sizeof(U8)*f->gradient->num);
	int s;
	for(s=0;s<f->gradient->num;s++)
	    swf_GetRGBA(tag, &f->gradient->rgba[s]);
	for(s=0;s<f->gradient->num;s++)
	    f->gradient->ratios[s] = swf_GetU8(tag);
	
	f->blurx = swf_GetFixed(tag);
	f->blury = swf_GetFixed(tag);
	f->angle = swf_GetFixed(tag);
	f->distance = swf_GetFixed(tag);
	f->strength = swf_GetFixed8(tag);
	U8 flags = swf_GetU8(tag);
	f->passes = flags&15;
	f->innershadow = (flags>>7)&1;
	f->knockout = (flags>>6)&1;
	f->composite = (flags>>5)&1;
	f->ontop = (flags>>4)&1;
	return (FILTER*)f;
    } else if(type == FILTERTYPE_DROPSHADOW) {
	FILTER_DROPSHADOW* f = (FILTER_DROPSHADOW*)rfx_calloc(sizeof(FILTER_DROPSHADOW));
	f->type = type;
	swf_GetRGBA(tag, &f->color);
	f->blurx = swf_GetFixed(tag);
	f->blury = swf_GetFixed(tag);
	f->angle = swf_GetFixed(tag);
	f->distance = swf_GetFixed(tag);
	f->strength = swf_GetFixed8(tag);
	U8 flags = swf_GetU8(tag);
	f->passes = flags&31;
	f->innershadow = (flags>>7)&1;
	f->knockout = (flags>>6)&1;
	f->composite = (flags>>5)&1;
	return (FILTER*)f;
    } else if(type == FILTERTYPE_BEVEL) {
	FILTER_BEVEL* f = (FILTER_BEVEL*)rfx_calloc(sizeof(FILTER_BEVEL));
	f->type = type;
	swf_GetRGBA(tag, &f->shadow);
	swf_GetRGBA(tag, &f->highlight);
	f->blurx = swf_GetFixed(tag);
	f->blury = swf_GetFixed(tag);
	f->angle = swf_GetFixed(tag);
	f->distance = swf_GetFixed(tag);
	f->strength = swf_GetFixed8(tag);
	U8 flags = swf_GetU8(tag);
	f->passes = flags&15;
	f->innershadow = (flags>>7)&1;
	f->knockout = (flags>>6)&1;
	f->composite = (flags>>5)&1;
	f->ontop = (flags>>4)&1;
	return (FILTER*)f;
    } else {
	fprintf(stderr, "Reading of filter type %02x not supported yet\n", type);
    }
    return 0;
}

FILTER*swf_NewFilter(U8 type)
{
    FILTER*f = 0;
    if(type == FILTERTYPE_BLUR)
	f = (FILTER*)rfx_calloc(sizeof(FILTER_BLUR));
    else if(type == FILTERTYPE_GRADIENTGLOW)
	f = (FILTER*)rfx_calloc(sizeof(FILTER_GRADIENTGLOW));
    else if(type == FILTERTYPE_DROPSHADOW)
	f = (FILTER*)rfx_calloc(sizeof(FILTER_DROPSHADOW));
    else if(type == FILTERTYPE_BEVEL)
	f = (FILTER*)rfx_calloc(sizeof(FILTER_BEVEL));
    else 
	fprintf(stderr, "Creation of filter type %02x not supported yet\n", type);
    if(f)
	f->type = type;
    return f;
}

void swf_DeleteFilter(FILTER*f)
{
    //FIXME
    free(f);
}
