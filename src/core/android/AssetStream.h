// -*- C++ -*-
#pragma once

#include <core/Stream.h>

class AAsset;

class AssetStream : public Stream
{
public:

    //!
    //!
    //!
    AssetStream(AAsset* asset);

    //!
    //!
    //!
    ~AssetStream();

    //!
    //!
    //!
    const unsigned int size() const;

    //!
    //!
    //!
    int read(void* target, const size_t bytes);

private:

    //!
    AAsset* asset;
};

