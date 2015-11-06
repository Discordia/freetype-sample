#ifndef FREETYPE_SAMPLE_STREAMFACTORY_H
#define FREETYPE_SAMPLE_STREAMFACTORY_H

#include <Stream.h>

#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

class StreamFactory
{
public:
    StreamFactory() {}
    virtual ~StreamFactory() {}

    virtual shared_ptr<Stream> open(const string& fileName) = 0;
};

#endif //FREETYPE_SAMPLE_STREAMFACTORY_H
