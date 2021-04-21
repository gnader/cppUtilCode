#include "array2d.h"

#include <iostream>
#include <iterator>

int main(int argc, char **argv)
{
  array2d<int, 2, 3> test;
  test = {{1, 2, 3},
          {4, 5, 6}};
  // array2d<int, 2, 3> test{1, 2, 3, 40, 5, 6};

  std::cout << test(0, 0) << ", " << test(1, 0) << std::endl;
  std::cout << test(0, 1) << ", " << test(1, 1) << std::endl;
  std::cout << test(0, 2) << ", " << test(1, 2) << std::endl;

  int *ptr = test.data();

  std::cout << *ptr << std::endl;
  std::cout << *(ptr + 4) << std::endl;

  return 0;
}