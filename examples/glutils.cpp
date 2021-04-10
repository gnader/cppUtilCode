#include <iostream>
using namespace std;

#include <GLFW/glfw3.h>

#include "glutils/buffer.h"
#include "glutils/program.h"
#include "glutils/shader.h"

// #include <vector>

std::vector<float> pos = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
std::vector<float> tex = {0, 0, 1, 0, 1, 1, 0, 1};

std::vector<int> indices = {0, 3, 2, 0, 2, 1};

std::string src_v = "#version 450 core \n in vec2 vtx_pos; \n in vec2 vtx_tex; \n out vec2 tex; \n void main(void) { tex = vtx_tex; gl_Position = vec4(vtx_pos.x, vtx_pos.y, 0., 1.0); }";
std::string src_f = "#version 450 core \n uniform sampler2D t; \n in vec2 tex; out vec4 colour; \n void main(void) { colour = vec4(texture(t, tex*3).rgb , 1.0); }";

void error(int errnum, const char *errmsg)
{
  std::cerr << errnum << ": " << errmsg << std::endl;
}

int main(int argc, char **argv)
{
  glfwSetErrorCallback(error);

  if (!glfwInit())
    return 1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_VISIBLE, false);

  auto window = glfwCreateWindow(320, 240, "", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  glfwHideWindow(window);

  glfwMakeContextCurrent(window);

  glbinding::Binding::initialize(glfwGetProcAddress, true);

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  Shader vert(GL_VERTEX_SHADER), frag(GL_FRAGMENT_SHADER);
  vert.load_source_from_string(src_v);
  frag.load_source_from_string(src_f);

  if (vert.compile())
    std::cout << "vertex shader compiled successfully" << std::endl;

  if (frag.compile())
    std::cout << "fragment shader compiled successfully" << std::endl;

  Program prog;
  prog.attach_shader(&vert);
  prog.attach_shader(&frag);

  if (prog.link())
    std::cout << "program linked successfully" << std::endl;

  Buffer buffP(GL_ARRAY_BUFFER), buffT(GL_ARRAY_BUFFER), buffI(GL_ELEMENT_ARRAY_BUFFER);

  std::cout << buffP.size() << " -> ";
  buffP.set_data<float>(pos, GL_STATIC_DRAW);
  std::cout << buffP.size() << std::endl;

  std::cout << buffT.size() << " -> ";
  buffT.set_data<float>(tex, GL_STATIC_DRAW);
  std::cout << buffT.size() << std::endl;

  std::cout << buffI.size() << " -> ";
  buffI.set_data<int>(indices, GL_STATIC_DRAW);
  std::cout << buffI.size() << std::endl;

  return 0;
}