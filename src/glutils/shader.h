#ifndef GLUTILS_SHADER_H
#define GLUTILS_SHADER_H

/* shader.h - v1.0
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

#include <string>

#include "glutils.h"

class Shader
{
protected:
  static void read_source_from_file(const std::string &filename,
                                    std::string &source);

public:
  Shader(GLenum type = GL_FRAGMENT_SHADER);
  virtual ~Shader();

  inline unsigned int id() const { return mId; }
  inline bool is_valid() const { return mId != 0; }

  inline GLenum type() const { return mType; }
  std::string type_string() const;

  const std::string &filename() const { return mFilename; }

  bool compile() const;
  bool is_compiled() const;

  bool load_source_from_file(const std::string &filename = "");
  bool load_source_from_string(const std::string &source);
  bool reload();

  void print_info_log() const;

protected:
  unsigned int mId;
  GLenum mType;

  std::string mFilename;
};

#endif
