#include <iostream>

#include "sampling.h"

#include <Eigen/Dense>

int main(int argc, char **argv)
{
  std::vector<Eigen::Vector2f> fSamples;
  Sampling::Regular::grid<Eigen::Vector2f>(Eigen::Vector2f::Zero(), Eigen::Vector2f(0.f, 0.1f), Eigen::Vector2f(0.1f, 0.0f), 11, 11, fSamples);

  for (auto s : fSamples)
    std::cout << s.transpose() << std::endl;

  return 0;
}