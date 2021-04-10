#include "timer.h"

#include <iostream>

void fun_test()
{
  int cp = 0;
  for (int i = 0; i < 100; ++i)
  {
    cp += 1;
    std::cout << "." << std::flush;
  }
  std::cout << std::endl;
}

int main(int argc, char **argv)
{
  float dt;
  COMPUTE_TIME(fun_test(), dt)
  std::cout << dt << "ms" << std::endl;

  float avgt;
  BENCH_TIME(fun_test(), 100, avgt)
  std::cout
      << dt << "ms" << std::endl;

  return 0;
}