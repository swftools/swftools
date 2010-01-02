#ifndef __abc_assets_h__
#define __abc_assets_h__
#include "abc.h"
#include "registry.h"

typedef struct _asset_resolver {
    SWF*swf;
    abc_asset_t**id2asset;
    dict_t*name2asset;
    U16 mainclass_id;
} asset_resolver_t;

asset_resolver_t* swf_ParseAssets(SWF*swf);
void swf_ResolveAssets(asset_resolver_t*swf, abc_file_t*file);
void swf_DumpAsset(FILE*fo, abc_asset_t*asset, const char*prefix);
TAG*swf_AssetsToTags(TAG*tag, asset_bundle_list_t*assets);

#endif //__abc_assets_h__
