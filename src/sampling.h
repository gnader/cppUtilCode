/* sampling.h - v0.1
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

namespace Sampling
{
  class Regular
  {
  public:
    template <typename T>
    static void line(const T &x0, const T &dx, int n, std::vector<T> &samples)
    {
      samples.reserve(n);
      for (int i = 0; i < n; ++i)
        samples.emplace_back(x0 + i * dx);
    }

    template <typename T>
    static void grid(const T &p0, const T &dx, const T &dy, int n, int m, std::vector<T> &samples)
    {
      samples.reserve(n * m);
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
          samples.emplace_back(p0 + i * dx + j * dy);
    }

    template <typename T>
    static void circle(const T &c, float r, int n, std::vector<T> &samples)
    {
      const float _2PI = 6.28318530718;
      const float dtheta = _2PI / float(n);

      samples.reserve(n);
      for (int i = 0; i < n; ++i)
      {
        T p;
        p(0) = c(0) + r * std::cos(i * dtheta);
        p(1) = c(1) + r * std::sin(i * dtheta);
        samples.emplace_back(p);
      }
    }

    template <typename T>
    static void disk(const T &c, float r, int n, std::vector<T> &samples)
    {
      const float _2PI = 6.28318530718;
      const float _PHI = 1.61803398875; // golden ration (1+sqrt(5))/2

      samples.reserve(n);
      for (int i = 0; i < n; ++i)
      {
        float radius = sqrt(float(i) / n);
        float theta = _2PI * float(i) / _PHI;

        T p;
        p(0) = c(0) + r * radius * cos(theta);
        p(1) = c(1) + r * radius * sin(theta);
      }
    }
  };
} // namespace Sampling
