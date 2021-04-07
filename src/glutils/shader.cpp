#include "shader.h"

#include <fstream>

void Shader::read_source_from_file(const std::string &filename, std::string &source)
{
  //initialise source
  source = "";

  //open file
  std::fstream file(filename.c_str(), std::ios::in);
  if (!file.is_open())
  {
    return;
  }

  //read file
  std::string buffer;
  while (std::getline(file, buffer))
    source += buffer;

  //close file
  file.close();
}

Shader::Shader(GLenum type)
    : mId(0), mType(type), mFilename("")
{
  mId = glCreateShader(mType);
}

Shader::~Shader()
{
  glDeleteShader(mId);
}

std::string Shader::type_string() const
{
  switch (mType)
  {
  case GL_FRAGMENT_SHADER:
    return "Fragment Shader";
    break;
  case GL_VERTEX_SHADER:
    return "Vertex Shader";
    break;
  case GL_GEOMETRY_SHADER:
    return "Geometry Shader";
    break;
  case GL_TESS_CONTROL_SHADER:
    return "Tesselation Control Shader";
    break;
  case GL_TESS_EVALUATION_SHADER:
    return "Teselation Evaluation Shader";
    break;
  case GL_COMPUTE_SHADER:
    return "Compute Shader";
    break;
  default:
    return "Unknow Type Shader";
    break;
  }
}

bool Shader::compile() const
{
  if (is_valid())
    glCompileShader(mId);

  bool status = is_compiled();
  if (!status)
    print_info_log();

  return status;
}

bool Shader::is_compiled() const
{
  int status = 0;

  if (is_valid())
    glGetShaderiv(mId, GL_COMPILE_STATUS, &status);

  return status;
}

bool Shader::load_source_from_file(const std::string &filename)
{
  if (filename != "")
    mFilename = filename;

  std::string source;
  Shader::read_source_from_file(mFilename, source);
  bool status = load_source_from_string(source);

  return status;
}

bool Shader::load_source_from_string(const std::string &source)
{
  if (is_valid())
  {
    const GLchar *src = source.c_str();
    glShaderSource(mId, 1, (const GLchar **)&src, 0);
  }

  return compile();
}

bool Shader::reload()
{
  return load_source_from_file(mFilename);
}

void Shader::print_info_log() const
{
  int length = 0;
  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &length);

  if (length > 0)
  {
    std::vector<char> log(length);
    glGetShaderInfoLog(id(), length, &length, log.data());
  }
}
