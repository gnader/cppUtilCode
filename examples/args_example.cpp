#include "args.h"

#include <vector>
#include <string>
#include <iostream>

template <typename T>
void get_type(T t)
{
  std::cout << typeid(t).name() << std::endl;
}

int main(int argc, char **argv)
{
  ArgumentManager args;

  args.add("-t", "--t", true, 1, "this is a test");
  args.add("-v", {"1", "2"}, false, "this is very important arg");

  std::cout << args.usage() << std::endl;

  return 0;
}