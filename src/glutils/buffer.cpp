#include "buffer.h"

Buffer::Buffer(GLenum target)
    : mId(0), mTarget(target)
{
  glCreateBuffers(1, &mId);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &mId);
}

GLint Buffer::size() const
{
  GLint sz = 0;
  bind();
  glGetBufferParameteriv(mTarget, GL_BUFFER_SIZE, &sz);
  return sz;
}

// Binds the buffer to target.
void Buffer::bind() const
{
  glBindBuffer(mTarget, mId);
}

// Unbinds a specific target, i.e. binds a 0 id to the target.
void Buffer::unbind() const
{
  glBindBuffer(mTarget, 0);
}

// Creates video memory for the buffer.
void Buffer::set_data(GLsizeiptr size, const GLvoid *data, GLenum usage)
{
  bind();
  glBufferData(mTarget, size, data, usage);
}

// Writes data only to a defined area of the memory.
void Buffer::set_subdata(GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
  bind();
  glBufferSubData(mTarget, offset, size, data);
}
