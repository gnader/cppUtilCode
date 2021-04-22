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

template <typename T, size_t COL, size_t ROW>
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

  typedef std::array<value_type, ROW> column_type;
  typedef std::array<reference, COL> row_type;
  typedef std::array<column_type, COL> data_type;

  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  typedef typename column_type::iterator iterator;
  typedef typename column_type::const_iterator const_iterator;

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
    iterator itr = begin();
    for (const_reference l : list)
      *itr++ = l;
  }

  array2d(const std::initializer_list<std::initializer_list<T>> &list)
  {
    iterator itr = begin();
    for (const std::initializer_list<T> &col : list)
      for (const_reference l : col)
        *itr++ = l;
  }

  array2d &operator=(const array2d &) = default;
  array2d &operator=(array2d &&) = default;

  //============================================
  //                Data Access
  //============================================
  reference at(size_t col, std::size_t row)
  {
    return mData.at(col).at(row);
  }

  const_reference at(size_t col, std::size_t row) const
  {
    return mData.at(col).at(row);
  }

  reference at(size_t i)
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return at(col, row);
  }

  const_reference at(size_t i) const
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return at(col, row);
  }

  reference operator()(size_t col, size_t row) noexcept
  {
    return mData[col][row];
  }

  const_reference operator()(size_t col, size_t row) const noexcept
  {
    return mData[col][row];
  }

  const_reference operator()(size_t i) const noexcept
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return mData[col][row];
  }

  reference operator()(size_t i) noexcept
  {
    std::size_t col = i / ROW;
    std::size_t row = i - col * ROW;

    return mData[col][row];
  }

  const_reference operator[](size_t i) const noexcept
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

  column_type &col(size_t i)
  {
    return mData.at(i);
  }

  const column_type &col(size_t i) const
  {
    return mData.at(i);
  }

  //============================================
  //                iterators
  //============================================
  iterator begin() noexcept
  {
    return mData.front().begin();
  }

  const_iterator begin() const noexcept
  {
    return mData.front().begin();
  }

  iterator end() noexcept
  {
    return mData.back().end();
  }

  const_iterator end() const noexcept
  {
    return mData.back().end();
  }

  iterator begin_col(size_t i)
  {
    return mData.at(i).begin();
  }

  const_iterator begin_col(size_t i) const
  {
    return mData.at(i).begin();
  }

  iterator end_col(size_t i)
  {
    return mData.at(i).end();
  }

  const_iterator end_col(size_t i) const
  {
    return mData.at(i).end();
  }

  //============================================
  //                capacity
  //============================================
  inline size_type size() const noexcept { return {COL, ROW}; }
  inline size_t size_col() const noexcept { return ROW; }
  inline size_t size_row() const noexcept { return COL; }

  inline size_t max_size() const noexcept { return COL * ROW; }
  inline size_t num() const noexcept { return COL * ROW; }
  inline size_t num_col() const noexcept { return COL; }
  inline size_t num_row() const noexcept { return ROW; }

  inline bool empty() const noexcept { return num() == 0; }

  //============================================
  //                operations
  //============================================
  void fill(const T &value)
  {
    iterator itr = begin();
    iterator finish = end();
    while (itr != finish)
      *itr++ = value;
  }

public:
  data_type mData;
};

#endif