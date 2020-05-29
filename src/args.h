/* args.h - A simple argument manager for comand line interfaces
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

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class ArgumentManager
{
  // internal Types
public:
  typedef vector<string> Value;

  class Argument
  {
    friend class ArgumentManager;

  public:
    Argument(const string &name, bool optional = false, size_t num = 1, const string &help = "")
        : mName(name), mAltName(""), mOptional(optional), mNum(num), mHelp(help)
    {
    }

    Argument(const string &name, const string &altName, bool optional = false, size_t num = 1, const string &help = "")
        : mName(name), mAltName(altName), mOptional(optional), mNum(num), mHelp(help)
    {
    }

    virtual ~Argument() {}

    inline Argument &alt_name(const string &altName)
    {
      mAltName = altName;
      return *this;
    }

    inline Argument &optional(bool flag)
    {
      mOptional = flag;
      return *this;
    }

    inline Argument &num_value(bool num)
    {
      mNum = num;
      return *this;
    }

    inline Argument &help(const string &text)
    {
      mHelp = text;
      return *this;
    }

    string to_string() const
    {
      string s = "* ";
      s += mName;
      if (mAltName.size() > 0)
        s += " ; " + mAltName;
      s += " | nval = ";
      s += std::to_string(mNum);
      s += mOptional ? ", [optional] : " : ", [required] : ";
      s += (mHelp + "\n");

      return s;
    }

  private:
    string mName;    //argument name, must start with "-""
    string mAltName; //alternative name, must start with "--""

    bool mOptional; //indicates whether the argument is optional
    size_t mNum;    //specifies the number of values attached to the argument

    string mHelp; //a description of the argument used for usage
  };

public:
  ArgumentManager(const string &programName = "", const string &description = "")
      : mBinName(""), mProgramName(programName), mDescription(description)
  {
  }

  virtual ~ArgumentManager() {}

  int parse(int argc, char **argv)
  {

    return mErrorMessages.size();
  }

  string usage() const
  {
    string usage = "";

    //write header
    if (mProgramName.size())
    {
      usage += (mProgramName + "\n");
      for (int i = 0; i < mProgramName.size(); ++i)
        usage += "=";
      usage += "\n";
      if (mDescription.size() > 0)
        usage += mDescription + "\n";
      usage += "\n";
    }

    //write the usage
    usage += "usage : " + mBinName;
    for (auto &arg : mArgs)
      if (!arg.mOptional)
        usage += " " + arg.mName + " <values ...>";
    usage += "\n";
    usage += "argument list :\n";
    for (auto &arg : mArgs)
      usage += " " + arg.to_string();

    return usage;
  }

  string error_messages() const
  {
    string msg = "";
    for (const auto &s : mErrorMessages)
      msg += (s + "\n");
    return msg;
  }

  Argument &add(const string &name, bool optional = false, size_t num = 1, const string &help = "")
  {
    size_t n = num == 0 ? 1 : num;
    Value defaultValue;
    defaultValue.reserve(n);
    for (int i = 0; i < n; ++i)
      defaultValue.emplace_back("0");

    return add(name, defaultValue, optional, help);
  }

  Argument &add(const string &name, const Value &defaultValue, bool optional = false, const string &help = "")
  {
    auto search = mIndices.find(name);
    if (search == mIndices.end())
    {
      size_t id = mIndices.size();
      mIndices[name] = id;
      mArgs.push_back(Argument(name, optional, defaultValue.size(), help));
      mValues.push_back(defaultValue);

      return mArgs[id];
    }

    //else update the found argument
    size_t id = search->second;
    mArgs[id].mOptional = optional;
    mArgs[id].mNum = defaultValue.size();
    mArgs[id].mHelp = help;
    mValues[id] = defaultValue;

    return mArgs[id];
  }

  Argument &add(const string &name, const string &altname, bool optional = false, size_t num = 1, const string &help = "")
  {
    return add(name, optional, num, help).alt_name(altname);
  }

  Argument &add(const string &name, const string &altname, const Value &defaultValue, bool optional = false, size_t num = 1, const string &help = "")
  {
    return add(name, defaultValue, num, help).alt_name(altname);
  }

  template <typename T>
  T value(const string &name, size_t id = 0) const
  {
    T out;

    auto search = mIndices.find(name);
    if (search != mIndices.end())
    {
      size_t index = search->second;
      if (id < mArgs[index].mNum && mArgs[index].mNum != 0)
        get_value<T>(index, id, out);
    }

    return out;
  }

  template <typename T>
  vector<T> values(const string &name) const
  {
    vector<T> out;
    values(name, out);

    return out;
  }

  template <typename T>
  void values(const string &name, vector<T> &out) const
  {
    out.clear(); //making sure the vector is empty

    auto search = mIndices.find(name);
    if (search != mIndices.end())
    {
      size_t index = search->second;
      size_t num = mArgs[index].mNum;
      out.reserve(num);
      for (size_t i = 0; i < num; ++i)
      {
        T temp;
        get_value<T>(index, i, temp);
        out.push_back(temp);
      }
    }
  }

private:
  template <typename T>
  inline void get_value(size_t argIndex, size_t valueIndex, T &out) const
  {
    if (typeid(out) == typeid(string))
      out = mValues[argIndex].at(valueIndex);
    else if (typeid(out) == typeid(int))
      out = stoi(mValues[argIndex].at(valueIndex));
    else if (typeid(out) == typeid(float))
      out = stof(mValues[argIndex].at(valueIndex));
    else if (typeid(out) == typeid(double))
      out = stod(mValues[argIndex].at(valueIndex));
    else if (typeid(out) == typeid(long))
      out = stol(mValues[argIndex].at(valueIndex));
    else if (typeid(out) == typeid(long long))
      out = stoll(mValues[argIndex].at(valueIndex));
    else if (typeid(out) == typeid(long double))
      out = stold(mValues[argIndex].at(valueIndex));
  }

  // class attributes
private:
  string mBinName;
  string mProgramName;
  string mDescription;

  unordered_map<string, size_t> mIndices;
  vector<Argument> mArgs;
  vector<Value> mValues;

  vector<string> mErrorMessages;
};
