#include "args.h"

#include <iostream>

int main(int argc, char **argv)
{
  ArgumentManager args("example program", "an example program shocasing how to use the agrs.h class");

  args.add("-a1", 1, false, "this is a required option #1");

  args.add("-a2", "--alt2", 3, true, "this is an optional option #2");
  args.add("-a3")->optional(true)->help("this is an optional option #3");

  //this will fail and adds an error, option names must start with - or --
  args.add("a4", true, 4, "this is an optional option #4");

  if (args.parse(argc, argv))
    std::cout << args.error_messages() << std::endl
              << std::endl;

  std::cout << args.usage() << std::endl;

  return 0;
}