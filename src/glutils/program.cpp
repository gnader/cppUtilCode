#include "program.h"

#include "glutils.h"
#include "shader.h"

Program::Program()
    : mId(0)
{
  mId = glCreateProgram();
}

Program::~Program()
{
  glDeleteProgram(id());
}

void Program::attach_shader(Shader *s)
{
  if (s == nullptr)
    return;

  if (is_valid())
    glAttachShader(id(), s->id());

  mShaders.insert(s);
}

void Program::detach_shader(Shader *s)
{
  if (s == nullptr)
    return;

  if (is_valid())
    glDetachShader(id(), s->id());

  mShaders.erase(s);
}

bool Program::link() const
{
  glLinkProgram(id());
  bool status = is_linked();
  if (!status)
    print_info_log();
  return status;
}

bool Program::is_linked() const
{
  int status = 0;
  if (is_valid())
    glGetProgramiv(id(), GL_LINK_STATUS, &status);
  return status;
}

void Program::activate() const
{
  glUseProgram(id());
}

void Program::desactivate() const
{
  glUseProgram(0);
}

int Program::getAttributeLocation(const char *name) const
{
  return glGetAttribLocation(id(), name);
}

int Program::getUniformLocation(const char *name) const
{
  return glGetUniformLocation(id(), name);
}

void Program::print_info_log() const
{
  int length = 0;
  glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &length);

  if (length > 0)
  {
    std::vector<char> log(length);
    glGetProgramInfoLog(id(), length, &length, log.data());
  }
}
