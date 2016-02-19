#ifndef FREETYPE_SAMPLE_DESKTOPSTREAMFACTORY_H
#define FREETYPE_SAMPLE_DESKTOPSTREAMFACTORY_H

#include "core/StreamFactory.h"
#include "FileStream.h"

class DesktopStreamFactory : public StreamFactory
{
public:

    DesktopStreamFactory(const string& directoryName)
        : directoryName(directoryName)
    { }


    virtual shared_ptr<Stream> open(const string& fileName) override;

private:
    string directoryName;
};

#endif //FREETYPE_SAMPLE_DESKTOPSTREAMFACTORY_H

shared_ptr<Stream> DesktopStreamFactory::open(const string& fileName)
{
    return shared_ptr<Stream>(new FileStream(directoryName + "/" + fileName));
}
