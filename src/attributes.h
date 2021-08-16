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

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// BaseAttributeArray class ===================================================================

class BaseAttributeArray
{
public:
  // Constructor
  BaseAttributeArray() : mName("") {}
  BaseAttributeArray(const char *name) : mName(name) {}

  //destructor
  virtual ~BaseAttributeArray() {}

  // Return the name of the attribute
  const std::string &name() const
  {
    return mName;
  }

  // Return the size of the array
  virtual size_t size() const = 0;

  // Reserve memory for n elements.
  virtual void reserve(size_t n) = 0;

  // Resize storage to hold n elements.
  virtual void resize(size_t n) = 0;

  // Extend the number of elements by n elements.
  virtual void increase_size(size_t n = 1) = 0;

  // Empty the container storage
  virtual void clear() = 0;

  // Free unused memory.
  virtual void shrink_to_fit() = 0;

  // Swap the storate space of two elements
  virtual void swap(size_t i, size_t j) = 0;

  // Return a deep copy of self.
  virtual BaseAttributeArray *clone() const = 0;

  // Return the type_info of the attribute
  virtual const std::type_info &type() const = 0;

protected:
  std::string mName;
};

// AttributeArray class ========================================================================

template <class T>
class AttributeArray : public BaseAttributeArray
{
public:
  typedef T ValueType;
  typedef std::vector<ValueType> ContainerType;

  typedef typename ContainerType::reference Ref;
  typedef typename ContainerType::const_reference ConstRef;

  AttributeArray(T t = T())
      : BaseAttributeArray(), mDefault(t)
  {
  }

  AttributeArray(const char *name, T t = T())
      : BaseAttributeArray(name), mDefault(t)
  {
  }

  virtual ~AttributeArray() {}

  virtual size_t size() const
  {
    return mData.size();
  }

  virtual void reserve(size_t n)
  {
    mData.reserve(n);
  }

  virtual void resize(size_t n)
  {
    mData.resize(n, mDefault);
  }

  virtual void increase_size(size_t n = 1)
  {
    for (size_t i = 0; i < n; ++i)
      mData.push_back(mDefault);
  }

  virtual void clear() = 0
  {
    mData.clear();
  }

  virtual void shrink_to_fit()
  {
    mData.shrink_to_fit();
  }

  virtual void swap(size_t i, size_t j)
  {
    ValueType temp(mData[i]);
    mData[i] = mData[j];
    mData[j] = temp;
  }

  virtual BaseAttributeArray *clone() const
  {
    AttributeArray<ValueType> *ptr = new AttributeArray<ValueType>(mName.c_str(), mDefault);
    ptr->mData = mData;
    return ptr;
  }

  virtual const std::type_info &type() const
  {
    return typeid(ValueType);
  }

  ValueType *data()
  {
    return mData.data();
  }

  const ValueType *data() const
  {
    return mData.data();
  }

  ContainerType &vector()
  {
    return mData;
  }

  const ContainerType &vector() const
  {
    return mData;
  }

  Ref operator()(int i)
  {
    return mData[i];
  }

  ConstRef operator()(int i) const
  {
    return mData[i];
  }

  Ref operator[](int i)
  {
    return mData[i];
  }

  ConstRef operator[](int i) const
  {
    return mData[i];
  }

protected:
  ContainerType mData;

private:
  ValueType mDefault;
};

// ElementAttribute class =====================================================================

template <class T>
class ElementAttribute
{
public:
  typedef typename AttributeArray<T>::ContainerType ContainerType;
  typedef typename AttributeArray<T>::Ref Ref;
  typedef typename AttributeArray<T>::ConstRef ConstRef;

public:
  ElementAttribute(AttributeArray<T> *ptr = nullptr)
      : mArray(ptr)
  {
  }

  virtual ~ElementAttribute()
  {
    reset();
  }

  void reset()
  {
    mArray = nullptr;
  }

  operator bool() const
  {
    return mArray != nullptr;
  }

  Ref operator[](int i)
  {
    return (*mArray)[i];
  }

  ConstRef operator[](int i) const
  {
    return (*mArray)[i];
  }

  T *data()
  {
    return mArray->data();
  }

  const T *data() const
  {
    return mArray->data();
  }

  ContainerType &vector()
  {
    return mArray->vector();
  }

  const ContainerType &vector() const
  {
    return mArray->vector();
  }

private:
  AttributeArray<T> *mArray;
};

// ElementAttributeList class =================================================================

class ElementAttributeList
{
private:
  typedef std::unordered_map<std::string, BaseAttributeArray *> DictionnaryType;

public:
  // default constructor
  ElementAttributeList()
      : mSize(0)
  {
  }

  ElementAttributeList(size_t size)
      : mSize(size)
  {
  }

  // copy constructor : performs a deep copy of all the element attributes
  ElementAttributeList(const ElementAttributeList &other)
  {
    operator=(other);
  }

  // destructor : delete all properties
  virtual ~ElementAttributeList()
  {
    clear();
  }

  // assign operator : performs a deep copy of all element attributes
  ElementAttributeList &operator=(const ElementAttributeList &other)
  {
    if (this != &other)
    {
      clear();
      mDict.reserve(other.num_attributes());
      mSize = other.size();
      for (const auto &it : other.mDict)
        mDict[it.first] = it.second->clone();
    }

    return *this;
  }

  size_t num_attributes() const
  {
    return mDict.size();
  }

  std::vector<std::string> attributes() const
  {
    std::vector<std::string> names;
    names.reserve(num_attributes());
    for (const auto &it : mDict)
      names.emplace_back(it.first);
    return names;
  }

  const std::type_info &type(const std::string &name) const
  {
    return mDict.at(name)->type();
  }

  template <class T>
  ElementAttribute<T> get(const std::string &name) const
  {
    DictionnaryType::const_iterator it = mDict.find(name);
    if (it == mDict.end())
    {
      std::cerr << "[ElementAttributeList::get()] : attribute with name \"" << name << "\" does not exist.\n";
      return ElementAttribute<T>(); // points to null attribute
    }

    return ElementAttribute<T>(dynamic_cast<AttributeArray<T> *>((*it).second));
  }

  template <class T>
  ElementAttribute<T> add(const std::string &name, const T &t = T())
  {

    if (mDict.find(name) != mDict.end())
    {
      std::cerr << "[ElementAttributeList::add()] : attribute with name \"" << name << "\" already exists.\n";
      return ElementAttribute<T>(); // points to null attribute;
    }

    AttributeArray<T> *ptr = new AttributeArray<T>(name.c_str(), t);
    ptr->resize(mSize);
    bool ok = mDict.insert({name, ptr}).second;
    if (!ok)
    {
      std::cerr << "[ElementAttributeList::add()] : unable to add attribute with name \"" << name << "\".\n";
      return ElementAttribute<T>(); // points to null attribute;
    }

    return ElementAttribute<T>(dynamic_cast<AttributeArray<T> *>(mDict.at(name)));
  }

  bool remove(const std::string &name)
  {
    DictionnaryType::iterator it = mDict.find(name);
    if (it == mDict.end())
    {
      std::cerr << "[ElementAttributeList::remove()] : attribute with name \"" << name << "\" does not exist.\n";
      return false;
    }

    delete (*it).second;
    (*it).second = nullptr;
    size_t n = mDict.erase(name);

    if (n == 0)
      std::cerr << "[ElementAttributeList::remove()] : unable to remove attribute with name \"" << name << "\".\n";

    return n > 0;
  }

  void clear()
  {
    for (auto &it : mDict)
    {
      delete it.second;
      it.second = nullptr;
    }

    mDict.clear();
  }

  // return attribute size
  size_t size() const
  {
    return mSize;
  }

  // Reserve memory for n elements.
  void reserve(size_t n)
  {
    mSize = n;
    for (auto &it : mDict)
      it.second->reserve(n);
  }

  // Resize storage to hold n elements.
  void resize(size_t n)
  {
    mSize = n;
    for (auto &it : mDict)
      it.second->resize(n);
  }

  // Extend the number of elements by n.
  void increase_size(size_t n = 1)
  {
    for (auto &it : mDict)
      it.second->increase_size(n);
  }

  // Free unused memory.
  void shrink_to_fit()
  {
    for (auto &it : mDict)
      it.second->shrink_to_fit();
  }

  // swap two elements
  void swap(size_t i, size_t j)
  {
    for (auto &it : mDict)
      it.second->swap(i, j);
  }

private:
  DictionnaryType mDict;
  size_t mSize;
};

#endif