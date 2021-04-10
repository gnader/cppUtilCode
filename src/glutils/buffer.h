#ifndef GLUTILS_BUFFER_H
#define GLUTILS_BUFFER_H

/* buffer.h - v1.0
 *
 * LICENCE
 * Public Domain (www.unlicense.org)
 * This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 * software, either in source code form or as a compiled binary, for any purpose,
 * commercial or non-commercial, and by any means.
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the public
 * domain. We make this dedication for the benefit of the public at large and to
 * the detriment of our heirs and successors. We intend this dedication to be an
 * overt act of relinquishment in perpetuity of all present and future rights to
 * this software under copyright law.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "glutils.h"

class Buffer
{
public:
  Buffer(GLenum target);
  virtual ~Buffer();

  inline const GLuint &id() const { return mId; }
  inline const GLenum &target() const { return mTarget; }

  inline bool is_created() const { return mId != 0; }

  GLint size() const;

  // Binds the buffer to target.
  void bind() const;

  // Unbinds a specific target, i.e. binds a 0 id to the target.
  void unbind() const;

  // Creates video memory for the buffer.
  void set_data(GLsizeiptr size, const GLvoid *data, GLenum usage);

  template <typename T>
  void set_data(const T &data, GLenum usage)
  {
    set_data(static_cast<GLsizeiptr>(sizeof(T)), &data, usage);
  }

  template <typename T>
  void set_data(const std::vector<T> &data, GLenum usage)
  {
    set_data(static_cast<GLsizeiptr>(data.size() * sizeof(T)), data.data(), usage);
  }

  template <typename T, std::size_t Count>
  void set_data(const std::array<T, Count> &data, GLenum usage)
  {
    set_data(static_cast<GLsizeiptr>(Count * sizeof(T)), data.data(), usage);
  }

  // Writes data only to a defined area of the memory.
  void set_subdata(GLintptr offset, GLsizeiptr size, const GLvoid *data = nullptr);

  template <typename T>
  void set_subdata(const T &data, gl::GLintptr offset = 0)
  {
    set_subdata(offset, static_cast<GLsizeiptr>(sizeof(T)), &data);
  }

  template <typename T>
  void set_subdata(const std::vector<T> &data, gl::GLintptr offset = 0)
  {
    set_subdata(offset, static_cast<GLsizeiptr>(data.size() * sizeof(T)), data.data());
  }

  template <typename T, std::size_t Count>
  void set_subdata(const std::array<T, Count> &data, gl::GLintptr offset = 0)
  {
    set_subdata(offset, static_cast<GLsizeiptr>(Count * sizeof(T)), data.data());
  }

protected:
  GLuint mId;
  GLenum mTarget;
};

#endif // GLUTILS_BUFFER_H