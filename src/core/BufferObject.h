#pragma once

#include <memory>

using std::shared_ptr;

typedef unsigned int uint32;

class BufferObject
{
public:

    //!
    //!
    //!
    static shared_ptr<BufferObject> createVertexBuffer(const uint32 size);

    //!
    //!
    //!
    static shared_ptr<BufferObject> createIndexBuffer(const uint32 size);

public:

    //!
    //!
    //!
    ~BufferObject();

    //!
    //!
    //!
    void bind() const;

    //!
    //!
    //!
    void unbind() const;

    //!
    //!
    //!
    const uint32 getSize() const;

    //!
    //!
    //!
    void fill(const uint32 offset, const uint32 size, const void* data) const;

private:

    //!
    //!
    //!
    BufferObject(const uint32 type, const uint32 size);

private:

    //!
    const uint32 type;

    //!
    const uint32 size;

    //!
    uint32 id;
};