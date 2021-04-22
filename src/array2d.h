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

/**
 * @Brief
 * A 2d column major array class with an interface similar to std::array
 * 
 * example:
 * -------
 * array2d<float, 2, 3> arr0 {1, 2, 3, 4, 5, 6};
 * array2d<float, 2, 3> arr2 {{1, 2, 3,}, {4, 5, 6}};
 */

//TODO : add an iterator over the elemets of a row

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

  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  typedef std::array<value_type, ROW> column_type;
  typedef std::array<column_type, COL> data_type;

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
  /**
   * @Brief
   * Returns a reference to the element at specified location {col, row}, with bounds checking.
   * If {col, row} is not within the range of the container, an exception of type std::out_of_range is thrown.
  **/
  reference at(size_t col, std::size_t row)
  {
    return mData.at(col).at(row);
  }

  const_reference at(size_t col, std::size_t row) const
  {
    return mData.at(col).at(row);
  }

  /**
   * @Brief
   * Returns a reference to the element at specified location i, with bounds checking.
   * If i is not within the range of the container, an exception of type std::out_of_range is thrown.
   * 
   * i is the index of the element in a column-major ordering.
  **/
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

  /**
   * @Brief
   * Returns a reference to the element at specified location {col, row}. No bounds checking is performed.
  **/
  reference operator()(size_t col, size_t row) noexcept
  {
    return mData[col][row];
  }

  const_reference operator()(size_t col, size_t row) const noexcept
  {
    return mData[col][row];
  }

  /**
   * @Brief
   * Returns a reference to the element at specified location i. No bounds checking is performed.
   * 
   * i is the index of the element in a column-major ordering.
  **/
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

  /**
   * @Brief
   * Returns a reference to the first element in the container.
  **/
  reference front()
  {
    return at(0, 0);
  }

  const_reference front() const
  {
    return at(0, 0);
  }

  /**
   * @Brief
   * Returns a reference to the last element in the container.
  **/
  reference back()
  {
    return at(COL - 1, ROW - 1);
  }

  const_reference back() const
  {
    return at(COL - 1, ROW - 1);
  }

  /**
   * @Brief
   * Returns pointer to the underlying array serving as element storage. 
   * The pointer is such that range [data(); data() + size()) is always a valid range, 
   * even if the container is empty (data() is not dereferenceable in that case).
  **/
  pointer data()
  {
    return mData.data()->data();
  }

  const_pointer data() const
  {
    return mData.data()->data();
  }

  /**
   * @Brief
   * Returns a reference to the ith column of the 2d array, with bounds checking.
   * If i is not within the range of the columns, an exception of type std::out_of_range is thrown.
  **/
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
  /**
   * @Brief
   * Returns an iterator to the first element of the array.
  **/
  iterator begin() noexcept
  {
    return mData.front().begin();
  }

  const_iterator begin() const noexcept
  {
    return mData.front().begin();
  }

  /**
   * @Brief
   * Returns an iterator to the last element of the array.
  **/
  iterator end() noexcept
  {
    return mData.back().end();
  }

  const_iterator end() const noexcept
  {
    return mData.back().end();
  }

  /**
   * @Brief
   * Returns an iterator to the first element of the ith coloum.
   * If i is not within the range of the columns, an exception of type std::out_of_range is thrown.
  **/
  iterator begin_col(size_t i)
  {
    return mData.at(i).begin();
  }

  const_iterator begin_col(size_t i) const
  {
    return mData.at(i).begin();
  }

  /**
   * @Brief
   * Returns an iterator to the last element of the ith column.
   * If i is not within the range of the columns, an exception of type std::out_of_range is thrown.
   * 
   * if i is the last column, than it is equal to end().
   * if i is not the last columns, that is is the equal to begin_col(i+1)
  **/
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
  /**
   * @Brief
   * Returns the 2d size of the array.
  **/
  inline size_type size() const noexcept { return {COL, ROW}; }

  /**
   * @Brief
   * Returns the size of a column.
  **/
  inline size_t size_col() const noexcept { return ROW; }

  /**
   * @Brief
   * Returns the size of a row.
  **/
  inline size_t size_row() const noexcept { return COL; }

  /**
   * @Brief
   * Returns the number of elements in the 2d array.
  **/
  inline size_t num() const noexcept { return COL * ROW; }
  inline size_t max_size() const noexcept { return COL * ROW; }

  /**
   * @Brief
   * Returns the number of columns.
  **/
  inline size_t num_col() const noexcept { return COL; }

  /**
   * @Brief
   * Returns the number of row.
  **/
  inline size_t num_row() const noexcept { return ROW; }

  /**
   * @Brief
   * checks whether the number of elemets is 0.
  **/
  inline bool empty() const noexcept { return num() == 0; }

  //============================================
  //                operations
  //============================================
  /**
   * @Brief
   * Assigns the given value value to all elements in the array.
  **/
  void fill(const T &value)
  {
    iterator itr = begin();
    iterator finish = end();
    while (itr != finish)
      *itr++ = value;
  }

protected:
  data_type mData;
};

#endif