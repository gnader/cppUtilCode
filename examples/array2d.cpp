#include "array2d.h"

#include <iostream>
#include <iterator>

int main(int argc, char **argv)
{
  // array2d<int, 2, 3> test;
  // test = {{1, 2, 3},
  //         {4, 5, 6}};
  std::array2d<int, 2, 3> test{1, 2, 3, 4, 5, 6};

  for (auto v : test)
    std::cout << v << ", ";
  std::cout << std::endl;

  return 0;
}