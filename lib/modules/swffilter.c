#include <stdlib.h>
#include <stdio.h>
#include "../rfxswf.h"

char *filtername[] = {
	"dropshadow", "blur", "glow", "bevel", "gradientglow",
	"convolution", "colormatrix", "gradientbevel", 0
};

void swf_SetFilter(TAG *tag, FILTER *filter)
{
	swf_SetU8(tag, filter->type);
	if (filter->type == FILTERTYPE_BLUR) {
		FILTER_BLUR *f = (FILTER_BLUR *)filter;
		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		U8 flags = f->passes << 3;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_GLOW) {
		FILTER_GLOW *f = (FILTER_GLOW *)filter;
		swf_SetRGBA(tag, &f->rgba);
		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		swf_SetFixed8(tag, f->strength);
		U8 flags = f->innerglow << 7 | f->knockout << 6 | f->composite << 5 | f->passes;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_DROPSHADOW) {
		FILTER_DROPSHADOW *f = (FILTER_DROPSHADOW *)filter;
		swf_SetRGBA(tag, &f->color);
		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		swf_SetFixed(tag, f->angle);
		swf_SetFixed(tag, f->distance);
		swf_SetFixed8(tag, f->strength);
		U8 flags = f->innershadow << 7 | f->knockout << 6 | f->composite << 5 | f->passes;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_GRADIENTGLOW) {
		FILTER_GRADIENTGLOW *f = (FILTER_GRADIENTGLOW *)filter;
		swf_SetU8(tag, f->gradient->num);

		int s;
		for (s = 0; s < f->gradient->num; s++)
			swf_SetRGBA(tag, &f->gradient->rgba[s]);
		for (s = 0; s < f->gradient->num; s++)
			swf_SetU8(tag, f->gradient->ratios[s]);

		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		swf_SetFixed(tag, f->angle);
		swf_SetFixed(tag, f->distance);
		swf_SetFixed8(tag, f->strength);
		U8 flags = f->innershadow << 7 | f->knockout << 6 | f->composite << 5 | f->ontop << 4 | f->passes;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_BEVEL) {
		FILTER_BEVEL *f = (FILTER_BEVEL *)filter;
		swf_SetRGBA(tag, &f->shadow);
		swf_SetRGBA(tag, &f->highlight);
		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		swf_SetFixed(tag, f->angle);
		swf_SetFixed(tag, f->distance);
		swf_SetFixed8(tag, f->strength);
		U8 flags = f->innershadow << 7 | f->knockout << 6 | f->composite << 5 | f->ontop << 4 | f->passes;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_GRADIENTBEVEL) {
		FILTER_GRADIENTBEVEL *f = (FILTER_GRADIENTBEVEL *)filter;
		swf_SetU8(tag, f->gradient->num);

		int s;
		for (s = 0; s < f->gradient->num; s++)
			swf_SetRGBA(tag, &f->gradient->rgba[s]);
		for (s = 0; s < f->gradient->num; s++)
			swf_SetU8(tag, f->gradient->ratios[s]);

		swf_SetFixed(tag, f->blurx);
		swf_SetFixed(tag, f->blury);
		swf_SetFixed(tag, f->angle);
		swf_SetFixed(tag, f->distance);
		swf_SetFixed8(tag, f->strength);
		U8 flags = f->innershadow << 7 | f->knockout << 6 | f->composite << 5 | f->ontop << 4 | f->passes;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_CONVOLUTION) {
		FILTER_CONVOLUTION *f = (FILTER_CONVOLUTION *)filter;
		swf_SetU8(tag, f->matrixx);
		swf_SetU8(tag, f->matrixy);
		swf_SetFloat(tag, f->divisor);
		swf_SetFloat(tag, f->bias);

		int s;
		for (s = 0; s < f->matrixx * f->matrixy; s++)
			swf_SetFloat(tag, f->matrix[s]);

		swf_SetRGBA(tag, &f->defaultcolor);
		U8 flags = f->clampmode << 1 | f->preservealpha;
		swf_SetU8(tag, flags);
	} else if (filter->type == FILTERTYPE_COLORMATRIX) {
		FILTER_COLORMATRIX *f = (FILTER_COLORMATRIX *)filter;

		int s;
		for (s = 0; s < 20; s++)
			swf_SetFloat(tag, f->matrix[s]);
	} else {
		fprintf(stderr, "Writing of filter type %02x not supported yet\n", filter->type);
	}
}

FILTER *swf_GetFilter(TAG *tag)
{
	U8 type = swf_GetU8(tag);
	FILTER *filter;

	if (type == FILTERTYPE_BLUR) {
		FILTER_BLUR *f = (FILTER_BLUR *)rfx_calloc(sizeof (FILTER_BLUR));
		f->type = type;
		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = (flags & 0xFF) >> 3;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_GLOW) {
		FILTER_GLOW *f = (FILTER_GLOW *)rfx_calloc(sizeof (FILTER_GLOW));
		f->type = type;
		swf_GetRGBA(tag, &f->rgba);
		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		f->strength = swf_GetFixed8(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = flags & 0x1F;
		f->innerglow = (flags >> 7) & 1;
		f->knockout = (flags >> 6) & 1;
		f->composite = (flags >> 5) & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_DROPSHADOW) {
		FILTER_DROPSHADOW *f = (FILTER_DROPSHADOW *)rfx_calloc(sizeof (FILTER_DROPSHADOW));
		f->type = type;
		swf_GetRGBA(tag, &f->color);
		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		f->angle = swf_GetFixed(tag);
		f->distance = swf_GetFixed(tag);
		f->strength = swf_GetFixed8(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = flags & 0x1F;
		f->innershadow = (flags >> 7) & 1;
		f->knockout = (flags >> 6) & 1;
		f->composite = (flags >> 5) & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_GRADIENTGLOW) {
		FILTER_GRADIENTGLOW *f = (FILTER_GRADIENTGLOW *)rfx_calloc(sizeof (FILTER_GRADIENTGLOW));
		f->type = type;
		f->gradient = (GRADIENT *)rfx_calloc(sizeof (GRADIENT));
		f->gradient->num = swf_GetU8(tag);
		f->gradient->rgba = (RGBA *)rfx_calloc(sizeof (RGBA) * f->gradient->num);
		f->gradient->ratios = (U8 *)rfx_calloc(sizeof (U8) * f->gradient->num);

		int s;
		for (s = 0; s < f->gradient->num; s++)
			swf_GetRGBA(tag, &f->gradient->rgba[s]);
		for (s = 0; s < f->gradient->num; s++)
			f->gradient->ratios[s] = swf_GetU8(tag);

		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		f->angle = swf_GetFixed(tag);
		f->distance = swf_GetFixed(tag);
		f->strength = swf_GetFixed8(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = flags & 0x0F;
		f->innershadow = (flags >> 7) & 1;
		f->knockout = (flags >> 6) & 1;
		f->composite = (flags >> 5) & 1;
		f->ontop = (flags >> 4) & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_BEVEL) {
		FILTER_BEVEL *f = (FILTER_BEVEL *)rfx_calloc(sizeof (FILTER_BEVEL));
		f->type = type;
		swf_GetRGBA(tag, &f->shadow);
		swf_GetRGBA(tag, &f->highlight);
		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		f->angle = swf_GetFixed(tag);
		f->distance = swf_GetFixed(tag);
		f->strength = swf_GetFixed8(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = flags & 0x0F;
		f->innershadow = (flags >> 7) & 1;
		f->knockout = (flags >> 6) & 1;
		f->composite = (flags >> 5) & 1;
		f->ontop = (flags >> 4) & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_GRADIENTBEVEL) {
		FILTER_GRADIENTBEVEL *f = (FILTER_GRADIENTBEVEL *)rfx_calloc(sizeof (FILTER_GRADIENTBEVEL));
		f->type = type;
		f->gradient = (GRADIENT *)rfx_calloc(sizeof (GRADIENT));
		f->gradient->num = swf_GetU8(tag);
		f->gradient->rgba = (RGBA *)rfx_calloc(sizeof (RGBA) * f->gradient->num);
		f->gradient->ratios = (U8 *)rfx_calloc(sizeof (U8) * f->gradient->num);

		int s;
		for (s = 0; s < f->gradient->num; s++)
			swf_GetRGBA(tag, &f->gradient->rgba[s]);
		for (s = 0; s < f->gradient->num; s++)
			f->gradient->ratios[s] = swf_GetU8(tag);

		f->blurx = swf_GetFixed(tag);
		f->blury = swf_GetFixed(tag);
		f->angle = swf_GetFixed(tag);
		f->distance = swf_GetFixed(tag);
		f->strength = swf_GetFixed8(tag);
		U8 flags = swf_GetU8(tag);
		f->passes = flags & 0x0F;
		f->innershadow = (flags >> 7) & 1;
		f->knockout = (flags >> 6) & 1;
		f->composite = (flags >> 5) & 1;
		f->ontop = (flags >> 4) & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_CONVOLUTION) {
		FILTER_CONVOLUTION *f = (FILTER_CONVOLUTION *)rfx_calloc(sizeof (FILTER_CONVOLUTION));
		f->type = type;
		f->matrixx = swf_GetU8(tag);
		f->matrixy = swf_GetU8(tag);
		f->divisor = swf_GetFloat(tag);
		f->bias = swf_GetFloat(tag);
		f->matrix = (float *)rfx_calloc(f->matrixx * f->matrixy * sizeof (float));

		int s;
		for (s = 0; s < f->matrixx * f->matrixy; s++)
			f->matrix[s] = swf_GetFloat(tag);

		swf_GetRGBA(tag, &f->defaultcolor);
		U8 flags = swf_GetU8(tag);
		f->clampmode = (flags >> 1) & 1;
		f->preservealpha = flags & 1;
		return (FILTER *)f;
	} else if (type == FILTERTYPE_COLORMATRIX) {
		FILTER_COLORMATRIX *f = (FILTER_COLORMATRIX *)rfx_calloc(sizeof (FILTER_COLORMATRIX));
		f->type = type;

		int s;
		for (s = 0; s < 20; s++)
			f->matrix[s] = swf_GetFloat(tag);
		return (FILTER *)f;
	} else {
		fprintf(stderr, "Reading of filter type %02x not supported yet\n", type);
	}

	return NULL;
}

FILTER *swf_NewFilter(U8 type)
{
	FILTER *f = 0;

	if (type == FILTERTYPE_BLUR) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_BLUR));
		memset(f, 0, sizeof (FILTER_BLUR));
	} else if (type == FILTERTYPE_GLOW) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_GLOW));
		memset(f, 0, sizeof (FILTER_GLOW));
	} else if (type == FILTERTYPE_DROPSHADOW) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_DROPSHADOW));
		memset(f, 0, sizeof (FILTER_DROPSHADOW));
	} else if (type == FILTERTYPE_GRADIENTGLOW) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_GRADIENTGLOW));
		memset(f, 0, sizeof (FILTER_GRADIENTGLOW));
	} else if (type == FILTERTYPE_BEVEL) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_BEVEL));
		memset(f, 0, sizeof (FILTER_BEVEL));
	} else if (type == FILTERTYPE_GRADIENTBEVEL) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_GRADIENTBEVEL));
		memset(f, 0, sizeof (FILTER_GRADIENTBEVEL));
	} else if (type == FILTERTYPE_CONVOLUTION) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_CONVOLUTION));
		memset(f, 0, sizeof (FILTER_CONVOLUTION));
	} else if (type == FILTERTYPE_COLORMATRIX) {
		f = (FILTER *)rfx_calloc(sizeof (FILTER_COLORMATRIX));
		memset(f, 0, sizeof (FILTER_COLORMATRIX));
	} else {
		fprintf(stderr, "Creation of filter type %02x not supported yet\n", type);
	}

	if (f)
		f->type = type;

	return f;
}

void swf_DeleteFilter(FILTER *f)
{
	if (f->type == FILTERTYPE_GRADIENTGLOW && ((FILTER_GRADIENTGLOW *)f)->gradient) {
		if (((FILTER_GRADIENTGLOW *)f)->gradient->rgba)
			free(((FILTER_GRADIENTGLOW *)f)->gradient->rgba);
		if (((FILTER_GRADIENTGLOW *)f)->gradient->ratios)
			free(((FILTER_GRADIENTGLOW *)f)->gradient->ratios);
		free(((FILTER_GRADIENTGLOW *)f)->gradient);
	}
	if (f->type == FILTERTYPE_GRADIENTBEVEL && ((FILTER_GRADIENTBEVEL *)f)->gradient) {
		if (((FILTER_GRADIENTBEVEL *)f)->gradient->rgba)
			free(((FILTER_GRADIENTBEVEL *)f)->gradient->rgba);
		if (((FILTER_GRADIENTBEVEL *)f)->gradient->ratios)
			free(((FILTER_GRADIENTBEVEL *)f)->gradient->ratios);
		free(((FILTER_GRADIENTBEVEL *)f)->gradient);
	}
	if (f->type == FILTERTYPE_CONVOLUTION && ((FILTER_CONVOLUTION *)f)->matrix)
		free(((FILTER_CONVOLUTION *)f)->matrix);
	free(f);
}
