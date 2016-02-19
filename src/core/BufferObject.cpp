#include <core/BufferObject.h>
#include <core/OpenGL.h>

BufferObject::BufferObject(const uint32 type, const uint32 size)
        : type(type), size(size)
{
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, size, NULL, GL_DYNAMIC_DRAW);
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &id);
}

shared_ptr<BufferObject> BufferObject::createVertexBuffer(const uint32 size)
{
    return shared_ptr<BufferObject>(new BufferObject(GL_ARRAY_BUFFER, size));
}

shared_ptr<BufferObject> BufferObject::createIndexBuffer(const uint32 size)
{
    return shared_ptr<BufferObject>(new BufferObject(GL_ELEMENT_ARRAY_BUFFER, size));
}

void BufferObject::bind() const
{
    glBindBuffer(type, id);
}

void BufferObject::unbind() const
{
    glBindBuffer(type, 0);
}

const uint32 BufferObject::getSize() const
{
    return size;
}

void BufferObject::fill(const uint32 offset, const uint32 size, const void* data) const
{
    glBufferSubData(type, offset, size, data);
}
