/* args.h - v1.1
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
  * @Brief
  * A simple class that manages CLI arguments.
  */

//TODO
//  - Better management of errors (warnings vs. errors)
//  - Improve handeling of default values
//  - Improve class interface
//  - Add support for positional arguments

#pragma once

#include <functional>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class ArgumentManager
{
  //======================//
  //    Internal Types    //
  //===============================================================================================//
private:
  struct Info
  {
    std::string name;
    std::string altname;
    std::string help;

    bool optional;
    bool quiet;

    Info(const std::string &_name = "",
         const std::string &_altname = "",
         const std::string &_help = "",
         bool _optional = true, bool _quiet = false)
        : name(_name), altname(_altname), help(_help), optional(_optional), quiet(_quiet)
    {
    }
  };

  struct Validator
  {
    typedef std::function<bool(const std::string &)> FunctionType;
    FunctionType fn;
    std::string message;

    Validator(
        const FunctionType &_fn = [](const auto &in) { return true; },
        const std::string &_message = "")
        : fn(_fn), message(_message)
    {
    }
  };

public:
  class Argument
  {
    friend class ArgumentManager;

  public:
    inline Argument &optional(bool _optional)
    {
      mInfo.optional = _optional;
      return *this;
    }

    inline Argument &help(const std::string &_help)
    {
      mInfo.help = _help;
      return *this;
    }

    inline Argument &validator(const std::function<bool(const std::string &)> &fn, const std::string &message = "")
    {
      mValidator.fn = fn;
      mValidator.message = message;
      return *this;
    }

    virtual ~Argument()
    {
    }

  private:
    Argument()
        : mInfo("", "", "", true, false)
    {
    }

    Argument(const std::string &name, bool optional = false, const std::string &help = "")
        : mInfo(name, "", help, optional, false)
    {
    }

    Argument(const std::string &name, const std::string &altname, bool optional = false, const std::string &help = "")
        : mInfo(name, altname, help, optional, false)
    {
    }

    // Argument atribute getters
    inline const std::string &name() const
    {
      return mInfo.name;
    }

    inline const std::string &altname() const
    {
      return mInfo.altname;
    }

    inline const std::string &help() const
    {
      return mInfo.help;
    }

    inline bool optional() const
    {
      return mInfo.optional;
    }

    inline bool quiet() const
    {
      return mInfo.quiet;
    }

    inline bool is_valid(std::string &in) const
    {
      return mValidator.fn(in);
    }

    inline bool is_valid(std::vector<std::string> &in) const
    {
      bool status = true;
      for (const auto &s : in)
        status = status && mValidator.fn(s);
      return status;
    }

    inline const std::string &message() const
    {
      return mValidator.message;
    }

    // Checks if attributes was set
    inline bool has_altname() const
    {
      return mInfo.altname.size() > 0;
    }

    inline bool has_help() const
    {
      return mInfo.help.size() > 0;
    }

    inline bool has_message()
    {
      return mValidator.message.size();
    }

    // generates usage text
    std::string to_string() const
    {
      std::string s = "* ";

      s += name();
      if (has_altname())
        s += ", " + altname();
      if (has_help())
        s += has_altname() ? "\t\t" : "\t";
      s += help();
      s += "\n";

      return s;
    }

  private:
    Info mInfo;
    Validator mValidator;
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

    Value(int nval, const std::string &init)
        : found(false)
    {
      value.reserve(nval);
      for (size_t i = 0; i < nval; ++i)
        value.emplace_back(init);
    }

    Value(const std::vector<std::string> &init)
        : value(init)
    {
    }

    inline size_t nval() const
    {
      return value.size();
    }

  private:
    bool found;
    std::vector<std::string> value;
  };
  //===============================================================================================//
public:
  ArgumentManager(const std::string &programName = "", const std::string &description = "")
      : mBinName(""), mProgramName(programName), mDescription(description)
  {
    add("-h", "--help", 0, true, "outputs the program's usage");
  }

  virtual ~ArgumentManager() {}

  //======================//
  //      parse cli       //
  //===============================================================================================//
  size_t parse(int argc, char **argv)
  {
    std::vector<std::string> tokens(argv, argv + argc);

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
      bool status = true;
      for (size_t j = 0; j < mValues[index].value.size(); ++j)
      {
        size_t current = i + j + 1;
        if (current >= tokens.size() || is_valid_name(tokens[current]))
          mErrorMessages.push_back(tokens[i] + " has less values than expected");
        else
        {
          mValues[index].value.at(j) = tokens[current];
          if (!mArgs[index].is_valid(tokens[current]))
            mErrorMessages.push_back(tokens[current] +
                                     " does not satify the following condition: " +
                                     mArgs[index].message());
        }
      }

      i += mValues[index].value.size() + 1;
    }

    return mErrorMessages.size();
  }

  //======================//
  //   prog information   //
  //===============================================================================================//
  std::string usage() const
  {
    std::string usage = "";

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
    for (const auto &arg : mArgs)
      if (!arg.optional())
        usage += " " + arg.to_string();
    usage += "Optional options:\n";
    for (const auto &arg : mArgs)
      if (arg.optional())
        usage += " " + arg.to_string();

    return usage;
  }

  std::string error_messages() const
  {
    std::string msg = "";
    int i = 0;
    for (const auto &s : mErrorMessages)
      msg += (" " + std::to_string(++i) + ".  " + s + "\n");
    return msg;
  }

  //======================//
  //     add Argument     //
  //===============================================================================================//
  Argument &add(const std::string &name, size_t num = 1, bool optional = false, const std::string &help = "")
  {
    std::vector<std::string> init;
    init.reserve(num);
    for (size_t i = 0; i < num; ++i)
      init.emplace_back("0");
    return add(name, init, optional, help);
  }

  Argument &add(const std::string &name, const std::vector<std::string> &init, bool optional = false, const std::string &help = "")
  {
    if (is_valid_name(name))
    {
      auto search = mIndices.find(name);
      if (search == mIndices.end())
      {
        size_t id = mArgs.size();
        mIndices[name] = id;
        mArgs.push_back(Argument(name, optional, help));
        mValues.push_back(init);

        return mArgs[id];
      }
      else
        mErrorMessages.push_back(name + " option already exists.");
    }
    else
      mErrorMessages.push_back(name + " is not a valid option name, options must start with - or -- followed by a letter");

    return mArgs.back();
  }

  Argument &add(const std::string &name, const std::string &altname, size_t num = 1, bool optional = false, const std::string &help = "")
  {
    std::vector<std::string> init;
    init.reserve(num);
    for (size_t i = 0; i < num; ++i)
      init.emplace_back("0");
    return add(name, altname, init, optional, help);
  }

  Argument &add(const std::string &name, const std::string &altname, const std::vector<std::string> &init, bool optional = false, const std::string &help = "")
  {
    if (is_valid_name(name) && is_valid_name(altname))
    {
      auto search1 = mIndices.find(name);
      auto search2 = mIndices.find(altname);
      if (search1 == mIndices.end() && search2 == mIndices.end())
      {
        size_t id = mArgs.size();
        mIndices[name] = id;
        mIndices[altname] = id;
        mArgs.push_back(Argument(name, altname, optional, help));
        mValues.push_back(init);
      }
      else
        mErrorMessages.push_back(name + " & " + altname + " option already exists.");
    }
    else
      mErrorMessages.push_back(name + " is not a valid option name, options must start with - or -- followed by a letter.");

    return mArgs.back();
  }

  //========================//
  //   get argument value   //
  //===============================================================================================//
  template <typename T>
  T get_value(const std::string &name, size_t id = 0) const
  {
    T out;

    auto search = mIndices.find(name);
    if (search != mIndices.end())
    {
      size_t index = search->second;
      size_t nVal = mValues[index].value.size();
      if (nVal != 0 && id < nVal)
        cast_value<T>(index, id, out);
    }

    return out;
  }

  template <typename T>
  std::vector<T> get_values(const std::string &name) const
  {
    std::vector<T> out;
    get_values(name, out);

    return out;
  }

  template <typename T>
  void get_values(const std::string &name, std::vector<T> &out) const
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
        cast_value<T>(index, i, temp);
        out.push_back(temp);
      }
    }
  }

  //=====================================//
  //   check if an argument was parsed   //
  //===============================================================================================//

  bool found(const std::string &name)
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
  inline bool is_valid_name(const std::string &name)
  {
    if (name.size() <= 1 || name[0] != '-')
      return false;
    if (name.size() <= 2)
      return bool(isalpha(name[1]));
    else
      return bool(isalpha(name[(name[1] == '-') ? 2 : 1]));
  }

  template <typename T>
  inline void cast_value(size_t argIndex, size_t valueIndex, T &out) const
  {
    std::istringstream ss(mValues[argIndex].value.at(valueIndex));
    ss >> out;
  }

  //========================//
  //    class attributes    //
  //===============================================================================================//
private:
  std::string mBinName;
  std::string mProgramName;
  std::string mDescription;

  std::unordered_map<std::string, size_t> mIndices;
  std::vector<Argument> mArgs;
  std::vector<Value> mValues;

  std::vector<std::string> mErrorMessages;
};
