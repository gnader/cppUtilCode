#ifndef GLUTILS_SHADER_H
#define GLUTILS_SHADER_H

#include <string>

#include "gl.h"

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
