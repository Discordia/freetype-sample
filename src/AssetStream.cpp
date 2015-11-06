// -*- C++ -*-
#include "AssetStream.h"

#include <android/asset_manager.h>

AssetStream::AssetStream(AAsset* asset)
        : asset(asset)
{
}

AssetStream::~AssetStream()
{
    AAsset_close(asset);
}

const unsigned int AssetStream::size() const
{
    return static_cast<unsigned int>(AAsset_getLength(asset));
}

int AssetStream::read(void* target, const size_t bytes)
{
    return AAsset_read(asset, target, bytes);
}

