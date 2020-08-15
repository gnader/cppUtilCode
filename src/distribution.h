/* distribution.h - v0.1
 *
 * LICENCE
 * Public Domain (www.unlicense.org)
 * This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 * software, either in source code form or as a compiled binary, for any purpose,
 * commercial or non-commercial, and by any means.
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the public
 * domain. We make this dedication for the benefit of the public at large and to
 * the detriment of our heirs and successors. We intend this dedication to be an
 * overt act of relinquishment in perpetuity of all present and future rights to
 * this software under copyright law.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * CREDITS
 *  written in 2020 by Georges NADER
 */
#pragma once

#include <vector>
#include <random>

#include <Eigen/Sparse>

template <typename Scalar = double>
class RealDistribution
{
protected:
  typedef Eigen::Matrix<Scalar, -1, 1> Vector;
  typedef Eigen::SparseMatrix<Scalar> SparseMatrix;
  typedef Eigen::SimplicialLDLT<SparseMatrix> SolverType;

public:
  RealDistribution(Scalar min, Scalar max)
      : mMin(min), mMax(max), mSolverReady(false)
  {
    default_cdf();
  }

  RealDistribution(Scalar *pdf, int n, Scalar min, Scalar max)
      : mMin(min), mMax(max), mSolverReady(false)
  {
    compute_cdf(pdf, n);
  }

  virtual ~RealDistribution() {} // empty destructor

  void set_pdf(Scalar *pdf, int n)
  {
    mSolverReady = (grid_size() == n);
    compute_cdf(pdf, n);
  }

protected:
  int grid_size()
  {
    return mCdf.cols() - 1;
  }

  void init_solver(int n)
  {
    //build laplacian matrix;
    typedef Eigen::Triplet<Scalar, int> Triplet;

    //build laplacian matrix
    std::vector<Triplet> triplets;
    triplets.reserve(n * 3);
    triplets.push_back(Triplet(1., 0, 1));
    triplets.push_back(Triplet(-1., 0, 0));
    for (int i = 1; i < n - 1; ++i)
    {
      triplets.push_back(Triplet(1., i, i - 1));
      triplets.push_back(Triplet(1., i, i + 1));
      triplets.push_back(Triplet(-2., i, i));
    }
    triplets.push_back(Triplet(1., n - 1, n - 2));
    triplets.push_back(Triplet(-1., n - 1, n - 1));

    mLaplacian.resize(n, n);
    mLaplacian.setFromTriplets(triplets.begin(), triplets.end());

    mSolver.compute(mLaplacian);

    mSolverReady = true;
  }

  void default_cdf()
  {
    mCdf.resize(2, 1);
    mCdf(0) = 0;
    mCdf(1) = 1;
  }

  void compute_cdf(Scalar *pdf, int n)
  {
    if (!mSolverReady)
      init_solver();

    const Scalar dx = 1. / Scalar(n);

    Vector b;
    b.resize(n, 1);
    for (int i = 0; i < n; ++i)
      b(i) = pdf[i] - 1.;

    //compute psi
    Vector psi = mSolver.solve(b * dx * dx);

    //compute the cdf
    mCdf.resize(n + 1, 1);
    mCdf(0) = 0.;
    mCdf(n) = 0.;
    for (int i = 1; i < n; ++i)
      mCdf(i) = dx * (i + (psi(i) - psi(i - 1)));
  }

protected:
  Scalar mMin, mMax;
  Vector mCdf;

  bool mSolverReady;
  SparseMatrix mLaplacian;
  SolverType mSolver;

  std::uniform_real_distribution<Scalar> uniform;
};