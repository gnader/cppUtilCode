/* args.h - v1.0
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

/**
 * Brief
 * A simple class that manages CLI arguments.
 * 
 */

//TODO
//  - Add the possiblity to validate parsed input
//  - Better management of errors (warnings vs. errors)
//  - Add support for positional arguments

#pragma once

#include <locale>
#include <unordered_map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>

using namespace std;

class ArgumentManager
{
  //======================//
  //    Internal Types    //
  //===============================================================================================//
public:
  class Argument
  {
    friend class ArgumentManager;

  public:
    inline Argument *optional(bool flag)
    {
      mOptional = flag;
      return this;
    }

    inline Argument *help(const string &text)
    {
      mHelp = text;
      return this;
    }

    virtual ~Argument() {}

  private:
    Argument()
        : mName(""), mAltName(""), mOptional(true), mHelp("")
    {
    }

    Argument(const string &name, bool optional = false, const string &help = "")
        : mName(name), mAltName(""), mOptional(optional), mHelp(help)
    {
    }

    Argument(const string &name, const string &altName, bool optional = false, const string &help = "")
        : mName(name), mAltName(altName), mOptional(optional), mHelp(help)
    {
    }

    string to_string() const
    {
      string s = "* ";
      s += mName;
      if (mAltName.size() > 0)
        s += ", " + mAltName;
      if (mHelp.size() > 0)
        s += (mAltName.size() == 0) ? "\t\t" : "\t";
      s += mHelp;
      s += "\n";

      return s;
    }

  private:
    string mName;    //argument name
    string mAltName; //alternative name

    bool mOptional; //indicates whether the argument is optional

    string mHelp; //a description of the argument used for usage
  };

  class Value
  {
    friend class ArgumentManager;

  public:
    virtual ~Value() {}

  private:
    Value()
        : found(false)
    {
    }

    Value(int nval, const string &init)
        : found(false)
    {
      value.reserve(nval);
      for (size_t i = 0; i < nval; ++i)
        value.emplace_back(init);
    }

    Value(const vector<string> &init)
        : value(init)
    {
    }

  private:
    bool found;
    vector<string> value;
  };
  //===============================================================================================//
public:
  ArgumentManager(const string &programName = "", const string &description = "")
      : mBinName(""), mProgramName(programName), mDescription(description)
  {
    add("-h", "--help", 0, true, "output the program's usage");
  }

  virtual ~ArgumentManager() {}

  //======================//
  //      parse cli       //
  //===============================================================================================//
  size_t parse(int argc, char **argv)
  {
    vector<string> tokens(argv, argv + argc);

    //get binary name
    size_t pos = tokens[0].find_last_of("/\\");
    mBinName = tokens[0].substr(pos + 1);

    for (size_t i = 1; i < tokens.size();)
    {
      if (!is_valid_name(tokens[i]))
      {
        mErrorMessages.push_back(tokens[i] + " is not a valid option");
        i += 1;
        continue;
      }

      auto search = mIndices.find(tokens[i]);
      if (search == mIndices.end())
      {
        mErrorMessages.push_back(tokens[i] + " is not a known option");
        i += 1;
        continue;
      }

      size_t index = search->second;
      mValues[index].found = true;
      for (size_t j = 0; j < mValues[index].value.size(); ++j)
      {
        size_t current = i + j + 1;
        if (current >= tokens.size() || is_valid_name(tokens[current]))
          mErrorMessages.push_back(tokens[i] + " has less values than expected");
        else
          mValues[index].value.at(j) = tokens[current];
      }

      i += mValues[index].value.size() + 1;
    }

    return mErrorMessages.size();
  }

  //======================//
  //   prog information   //
  //===============================================================================================//
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
    usage += "usage : " + mBinName + " [Options]\n";
    usage += "Required options:\n";
    for (auto &arg : mArgs)
      if (!arg.mOptional)
        usage += " " + arg.to_string();
    usage += "Optional options:\n";
    for (auto &arg : mArgs)
      if (arg.mOptional)
        usage += " " + arg.to_string();

    return usage;
  }

  string error_messages() const
  {
    string msg = "";
    int i = 0;
    for (const auto &s : mErrorMessages)
      msg += (" " + to_string(++i) + ".  " + s + "\n");
    return msg;
  }

  //======================//
  //     add Argument     //
  //===============================================================================================//
  Argument *add(const string &name, size_t num = 1, bool optional = false, const string &help = "")
  {
    Value init(num, "0");
    return add(name, init, optional, help);
  }

  Argument *add(const string &name, const Value &init, bool optional = false, const string &help = "")
  {

    if (!is_valid_name(name))
    {
      mErrorMessages.push_back(name + " is not a valid option name, options must start with - or -- followed by a letter");
      return nullptr;
    }

    auto search = mIndices.find(name);
    if (search == mIndices.end())
    {
      size_t id = mArgs.size();
      mIndices[name] = id;
      mArgs.push_back(Argument(name, optional, help));
      mValues.push_back(init);

      return &mArgs[id];
    }

    mErrorMessages.push_back(name + " option already exists.");
    return nullptr;
  }

  Argument *add(const string &name, const string &altname, size_t num = 1, bool optional = false, const string &help = "")
  {
    Value init(num, "0");
    return add(name, altname, init, optional, help);
  }

  Argument *add(const string &name, const string &altname, const Value &init, bool optional = false, const string &help = "")
  {
    if (!is_valid_name(name) || !is_valid_name(altname))
    {
      mErrorMessages.push_back(name + " is not a valid option name, options must start with - or -- followed by a letter.");
      return nullptr;
    }

    auto search1 = mIndices.find(name);
    auto search2 = mIndices.find(altname);

    if (search1 == mIndices.end() && search2 == mIndices.end())
    {
      size_t id = mArgs.size();
      mIndices[name] = id;
      mIndices[altname] = id;
      mArgs.push_back(Argument(name, altname, optional, help));
      mValues.push_back(init);

      return &mArgs[id];
    }

    mErrorMessages.push_back(name + " & " + altname + " option already exists.");
    return nullptr;
  }

  //========================//
  //   get argument value   //
  //===============================================================================================//
  template <typename T>
  T get_value(const string &name, size_t id = 0) const
  {
    T out;

    auto search = mIndices.find(name);
    if (search != mIndices.end())
    {
      size_t index = search->second;
      size_t nVal = mValues[index].value.size();
      if (nVal != 0 && id < nVal)
        get_value<T>(index, id, out);
    }

    return out;
  }

  template <typename T>
  vector<T> get_values(const string &name) const
  {
    vector<T> out;
    get_values(name, out);

    return out;
  }

  template <typename T>
  void get_values(const string &name, vector<T> &out) const
  {
    out.clear(); //making sure the vector is empty

    auto search = mIndices.find(name);
    if (search != mIndices.end())
    {
      size_t index = search->second;
      size_t nVal = mValues[index].value.size();
      out.reserve(nVal);
      for (size_t i = 0; i < nVal; ++i)
      {
        T temp;
        get_value<T>(index, i, temp);
        out.push_back(temp);
      }
    }
  }

  //=====================================//
  //   check if an argument was parsed   //
  //===============================================================================================//

  bool found(const string &name)
  {
    auto search = mIndices.find(name);
    if (search == mIndices.end())
      return false;

    return mValues[search->second].found;
  }

  //========================//
  //    helper functions    //
  //===============================================================================================//
private:
  bool is_valid_name(const string &name)
  {
    if (name.size() <= 1 || name[0] != '-')
      return false;
    if (name.size() <= 2)
      return bool(isalpha(name[1]));
    else
      return bool(isalpha(name[(name[1] == '-') ? 2 : 1]));
  }

  template <typename T>
  inline void get_value(size_t argIndex, size_t valueIndex, T &out) const
  {
    istringstream ss(mValues[argIndex].value.at(valueIndex));
    ss >> out;
  }

  //========================//
  //    class attributes    //
  //===============================================================================================//
private:
  string mBinName;
  string mProgramName;
  string mDescription;

  unordered_map<string, size_t> mIndices;
  vector<Argument> mArgs;
  vector<Value> mValues;

  vector<string> mErrorMessages;
};
