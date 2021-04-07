#ifndef GLUTILS_PROGRAM_H
#define GLUTILS_PROGRAM_H

/* program.h - v1.0
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

#include <unordered_set>

//forward declaration
class Shader;

class Program
{
public:
  Program();
  virtual ~Program();

  inline unsigned int id() const { return mId; }
  inline bool is_valid() const { return mId != 0; }

  void attach_shader(Shader *s);
  void detach_shader(Shader *s);

  bool link() const;
  bool is_linked() const;

  void activate() const;
  void desactivate() const;

  int getAttributeLocation(const char *name) const;
  int getUniformLocation(const char *name) const;

  void print_info_log() const;

protected:
  unsigned int mId;
  std::unordered_set<Shader *> mShaders;
};

#endif