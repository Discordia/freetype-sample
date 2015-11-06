// -*- C++ -*-
#pragma once

#include <Stream.h>

#include <fstream>
#include <string>

using std::string;
using std::ifstream;

class FileStream : public Stream
{
public:

    FileStream(const string& path);
    ~FileStream();

    const unsigned int size() const;
    int read(void* target, const size_t bytes);

private:
    ifstream file;
    unsigned int fileSize;
};
