// -*- C++ -*-
#pragma once

#include <cstddef>

using std::size_t;

class Stream
{
public:

    virtual ~Stream()
    { }

    virtual const unsigned int size() const = 0;

    virtual int read(void* target, const size_t bytes) = 0;
};
