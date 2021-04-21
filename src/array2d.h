/**
  *
  * MIT License
  *
  * Copyright (c) 2021 Georges Nader
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#ifndef ARRAY_2D
#define ARRAY_2D

#include <array>

template <typename T, std::size_t COL, std::size_t ROW>
class array2d
{
public:
  //============================================
  //              Member Types
  //============================================
  typedef T value_type;
  typedef std::array<std::size_t, 2> size_type;

  typedef value_type &reference;
  typedef const value_type &const_reference;

  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  typedef std::array<value_type, ROW> column_type;
  typedef std::array<value_type, COL> row_type;
  typedef std::array<std::array<value_type, ROW>, COL> data_type;

public:
  //============================================
  //              Initialisation
  //============================================
  array2d() = default;
  array2d(const array2d &) = default;
  array2d(array2d &&) = default;

  virtual ~array2d() = default;

  array2d(const std::initializer_list<T> &list)
  {
    pointer ptr = data();
    if (list.size() == COL * ROW)
      for (const_reference l : list)
        *ptr++ = l;
  }

  array2d(const std::initializer_list<std::initializer_list<T>> &list)
  {
    pointer ptr = data();
    for (const std::initializer_list<T> &col : list)
      for (const_reference l : col)
        *ptr++ = l;
  }

  array2d &operator=(const array2d &) = default;
  array2d &operator=(array2d &&) = default;

  //============================================
  //                Data Access
  //============================================
  reference at(std::size_t col, std::size_t row)
  {
    return mData.at(col).at(row);
  }

  const_reference at(std::size_t col, std::size_t row) const
  {
    return mData.at(col).at(row);
  }

  reference at(std::size_t i)
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return at(col, row);
  }

  const_reference at(std::size_t i) const
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return at(col, row);
  }

  reference operator()(std::size_t col, std::size_t row) noexcept
  {
    return mData[col][row];
  }

  const_reference operator()(std::size_t col, std::size_t row) const noexcept
  {
    return mData[col][row];
  }

  const_reference operator()(std::size_t i) const noexcept
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return mData[col][row];
  }

  reference operator()(std::size_t i) noexcept
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return mData[col][row];
  }

  const_reference operator[](std::size_t i) const noexcept
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return mData[col][row];
  }

  reference front()
  {
    return at(0, 0);
  }

  const_reference front() const
  {
    return at(0, 0);
  }

  reference back()
  {
    return at(COL - 1, ROW - 1);
  }

  const_reference back() const
  {
    return at(COL - 1, ROW - 1);
  }

  pointer data()
  {
    return mData.data()->data();
  }

  const_pointer data() const
  {
    return mData.data()->data();
  }

  //============================================
  //                iterators
  //============================================

  //============================================
  //                capacity
  //============================================
  inline size_type size() const noexcept { return {COL, ROW}; }

  inline size_t col_size() const noexcept { return COL; }
  inline size_t row_size() const noexcept { return ROW; }

  inline size_t element_size() const noexcept { return COL * ROW; }

  inline bool empty() const noexcept { return element_size() == 0; }

  //============================================
  //                operations
  //============================================

protected:
  data_type mData;
};

#endif