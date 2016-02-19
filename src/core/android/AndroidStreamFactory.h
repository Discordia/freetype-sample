#ifndef FREETYPE_SAMPLE_ANDROIDSTREAMFACTORY_H
#define FREETYPE_SAMPLE_ANDROIDSTREAMFACTORY_H

#include <android/asset_manager.h>

#include "core/StreamFactory.h"
#include "AssetStream.h"

class AndroidStreamFactory : public StreamFactory
{
public:
    AndroidStreamFactory(AAssetManager* assetManager)
        : assetManager(assetManager)
    {}


    virtual shared_ptr<Stream> open(const string& fileName) override;

private:
    AAssetManager* assetManager;
};

#endif //FREETYPE_SAMPLE_ANDROIDSTERAMFACTORY_H

shared_ptr<Stream> AndroidStreamFactory::open(const string& fileName)
{
    AAsset* asset = AAssetManager_open(assetManager, fileName.c_str(), AASSET_MODE_UNKNOWN);
    return shared_ptr<Stream>(new AssetStream(asset));
}
