#include "argmgr.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  ArgumentManager args("ArgumentManager", "1.5", "Georges Nader", "An example of using the ArgumentManager class to handle CLI examples.");

  args.positional(-1, false, "an undefined number of positional argument");

  args.add("-arg0", 1, true, "a required named argument with 1 parameter. No default value specified");
  args.add("-arg1", {"1", "2"}, false, "an optional named argument with 2 parameters. Default values [1, 2]");
  args.add("-arg2", -1, false, "an optional named argument with undifiend number of parameters.");
  args.add("-arg3", 0, false, "a flag type argument.");

  args.parse(argc, argv);

  args.info();
  args.usage();
  args.content();

  return 0;
}