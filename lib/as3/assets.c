#include <assert.h>
#include "../rfxswf.h"
#include "assets.h"

asset_resolver_t* swf_ParseAssets(SWF*swf)
{
    NEW(asset_resolver_t,assets);
    assets->name2asset = dict_new2(&charptr_type);
    assets->id2asset = malloc(sizeof(abc_asset_t*)*65536);

    TAG*tag = swf->firstTag;
    while(tag) {
	if(swf_isDefiningTag(tag)) {
	    NEW(abc_asset_t, asset);
	    list_append(asset->tags, tag);
	    assets->id2asset[swf_GetDefineID(tag)] = asset;
	}
	tag = tag->next;
    }

    tag = swf->firstTag;
    while(tag) {
	if(swf_isDefiningTag(tag)) {
	    abc_asset_t*asset = assets->id2asset[swf_GetDefineID(tag)];
	    assert(asset);
	    int num = swf_GetNumUsedIDs(tag);
	    int*positions = malloc(sizeof(int)*num);
	    swf_GetUsedIDs(tag, positions);
	    int t;
	    for(t=0;t<num;t++) {
		U16 id = GET16(&tag->data[positions[t]]);
		abc_asset_t*a = assets->id2asset[id];
		if(!a) {
		    fprintf(stderr, "Error: ID %d referenced, but not defined\n", id);
		} else {
		    list_append(asset->dependencies, a);
		}
	    }
	} else if(swf_isPseudoDefiningTag(tag)) {
	    abc_asset_t*asset = assets->id2asset[swf_GetDefineID(tag)];
	    if(asset) {
		list_append(asset->tags, tag);
	    }
	} else if(tag->id == ST_SYMBOLCLASS) {
	    int t, num = swf_GetU16(tag);
	    for(t=0;t<num;t++) {
		U16 id = swf_GetU16(tag);
		if(!id) {
		    assets->mainclass_id = id;
		} else {
		    abc_asset_t*asset = assets->id2asset[id];
		    if(!asset) {
			fprintf(stderr, "Error: ID %d referenced, but not defined\n", id);
		    } else {
			char*name = swf_GetString(tag);
			dict_put(assets->name2asset, name, asset);
		    }
		}
	    }
	}
	tag = tag->next;
    }
    return assets;
}

void swf_ResolveAssets(asset_resolver_t*assets, abc_file_t*file)
{
    int num = assets->name2asset->num;
    int resolved = 0;
    int t;
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
	char*fullname = abc_class_fullname(cls);
	abc_asset_t*a = (abc_asset_t*)dict_lookup(assets->name2asset, fullname);
	if(a) {
	    resolved++;
	    cls->asset = a;
	}
	free(fullname);
    }
}

static void dump_asset_list(FILE*fo, abc_asset_list_t*l, const char*prefix)
{
    while(l) {
	TAG_list_t*t = l->abc_asset->tags;
	while(t) {
	    TAG*tag = t->TAG;
	    fprintf(fo, "%s[tag] %s defines ID %d\n", prefix, swf_TagGetName(tag), swf_GetDefineID(tag));
	    t = t->next;
	}
	char*prefix2 = allocprintf("%s    ", prefix);
	dump_asset_list(fo, l->abc_asset->dependencies, prefix2);
	free(prefix2);
	l = l->next;
    }
}

void swf_DumpAsset(FILE*fo, abc_asset_t*asset, const char*prefix)
{
    abc_asset_list_t*l = 0;
    list_append(l, asset);
    dump_asset_list(fo, l, prefix);
    list_free(l);
}

static TAG* write_tag(TAG*prev, TAG*tag, dict_t*written)
{
    if(!dict_contains(written, tag)) {
	dict_put(written, tag, 0);
	if(prev) {
	    prev->next = tag;
	}
	tag->prev = prev;
	tag->next = 0;
	prev = tag;
    }
    return prev;
}
static TAG*write_asset(TAG*tag, abc_asset_t*a, dict_t*written)
{
    TAG_list_t*tags = a->tags;
    abc_asset_list_t*deps = a->dependencies;
    while(deps) {
	tag = write_asset(tag, deps->abc_asset, written);
	deps = deps->next;
    }
    while(tags) {
	tag = write_tag(tag, tags->TAG, written);
	tags = tags->next;
    }
    return tag;
}

void swf_WriteABCSymbols(TAG*tag, abc_file_t*file)
{
    int num = 0;
    int t;
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
	abc_asset_t*a = cls->asset;
	if(a && a->tags) {
	    num++;
	}
    }
    swf_SetU16(tag, num);
    for(t=0;t<file->classes->num;t++) {
	abc_class_t*cls = (abc_class_t*)array_getvalue(file->classes, t);
	abc_asset_t*a = cls->asset;
	if(a && a->tags) {
	    U16 id = swf_GetDefineID(a->tags->TAG);
	    char*fullname = abc_class_fullname(cls);
	    swf_SetU16(tag, id);
	    swf_SetString(tag, fullname);
	}
    }
}

TAG*swf_AssetsToTags(TAG*itag, asset_bundle_list_t*assets)
{
    char* bitmap = rfx_calloc(sizeof(char)*65536);
    asset_bundle_list_t*l = assets;
    dict_t*written = dict_new2(&ptr_type);
    while(l) {
	if(l->asset_bundle->used) {
	    abc_file_t*file = l->asset_bundle->file;
	    int t;
	    TAG* tag = 0;
	    for(t=0;t<file->classes->num;t++) {
		abc_asset_t*a = ((abc_class_t*)array_getvalue(file->classes, t))->asset;
		if(a) {
		    tag = write_asset(tag, a, written);
		}
	    }
	    
	    tag = swf_InsertTag(tag, ST_DOABC);
	    swf_WriteABC(tag, file);
	    tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
	    swf_WriteABCSymbols(tag, file);

	    TAG*first = tag;
	    while(first && first->prev) 
		first=first->prev;

	    SWF swf;
	    memset(&swf, 0, sizeof(SWF));
	    swf.firstTag = first;
	    swf_Relocate(&swf, bitmap);
	    if(!itag) {
		itag = first;
	    } else {
		itag->next = first;
		first->prev = itag;
		itag = tag;
	    }
	}
	l = l->next;
    }
    dict_destroy(written);
    free(bitmap);
    return itag;
}
