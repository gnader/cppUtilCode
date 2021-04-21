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

#ifndef ARGMGR_H
#define ARGMGR_H

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

/* 
 * TODO : 
 *   ////1. report parsing issues
 *   2. serialize to yaml file
 *   3. read from yaml file
 */

class Argument
{
public:
  Argument()
      : mName(""), mHelpMsg(""), mIsRequired(false), mIsFound(false), mExpectednValue(-1)
  {
  }

  Argument(const std::string &name, const std::string &helpMsg = "", int nValue = -1, bool isRequired = false)
      : mName(std::move(name)), mHelpMsg(std::move(helpMsg)), mIsRequired(isRequired), mIsFound(false), mExpectednValue(nValue)
  {
    if (mExpectednValue == 0)
      mIsRequired = false; // argument is a flag
  }

  Argument(const std::string &name, const std::vector<std::string> &value, const std::string &helpMsg = "", bool isRequired = false)
      : mName(std::move(name)), mHelpMsg(std::move(helpMsg)), mIsRequired(isRequired), mIsFound(false)
  {
    mExpectednValue = value.size();
    mValue.reserve(mExpectednValue);
    for (const auto &v : value)
      mValue.emplace_back(v);

    if (mExpectednValue == 0)
      mIsRequired = false; // argument is a flag
  }

  virtual ~Argument() {}

  inline const std::string &name() const
  {
    return mName;
  }

  inline const std::string &help_message() const
  {
    return mHelpMsg;
  }

  inline bool is_required() const
  {
    return mIsRequired;
  }

  inline bool is_found() const
  {
    return mIsFound;
  }

  inline int expected_nvalue() const
  {
    return mExpectednValue;
  }

  inline int nvalue() const
  {
    return mValue.size();
  }

  inline const std::vector<std::string> &value() const
  {
    return mValue;
  }

  inline const std::string &value(int i) const
  {
    return mValue.at(i);
  }

  inline Argument &help_message(const std::string &helpmsg)
  {
    mHelpMsg = helpmsg;
    return *this;
  }

  inline Argument &is_required(bool isRequired)
  {
    mIsRequired = isRequired;
    return *this;
  }

  inline Argument &is_found(bool isFound)
  {
    mIsFound = isFound;
    return *this;
  }

  inline Argument &nvalue(int nvalue)
  {
    mExpectednValue = nvalue;
    return *this;
  }

  inline Argument &value(const std::vector<std::string> &value)
  {
    if (value.size() < mValue.size())
    {
      for (int i = 0; i < value.size(); ++i)
        mValue[i] = value[i];
    }
    else
    {
      mValue.clear();
      mValue.reserve(value.size());
      for (const auto &v : value)
        mValue.emplace_back(v);
    }

    return *this;
  }

  std::string usage_text() const
  {
    std::stringstream ss;
    ss << std::left << std::setw(10) << name() << " | ";

    if (expected_nvalue() < 0)
      ss << std::left << std::setw(10) << "undefined"
         << " ";
    else if (expected_nvalue() == 0)
      ss << std::left << std::setw(10) << "flag"
         << " ";
    else
      ss << std::left << std::setw(10) << expected_nvalue() << " ";

    if (is_required())
      ss << std::left << std::setw(10) << "Yes"
         << " ";
    else
      ss << std::left << std::setw(10) << "No"
         << " ";

    ss << help_message();

    return ss.str();
  }

  std::string content_text() const
  {
    std::stringstream ss;
    ss << std::left << std::setw(10) << name() << " | ";

    if (is_found())
      ss << std::left << std::setw(10) << "Yes"
         << " ";
    else
      ss << std::left << std::setw(10) << "No"
         << " ";

    if (nvalue() == 0)
      ss << "No Data"
         << " ";
    else
    {
      for (auto s : mValue)
        ss << s << " ";
    }

    return ss.str();
  }

private:
  // Argument info
  std::string mName;
  std::string mHelpMsg;

  // Argument config
  bool mIsRequired;
  bool mIsFound;
  int mExpectednValue;

  // Argument value
  std::vector<std::string> mValue;
};

class ArgumentManager
{
public:
  enum ErrorType
  {
    INVALID_ARG = 0,
    MISSING_ARG = 1,
    MISSING_PARAM = 2,
    INVALID_TOKEN = 3,
    OTHER = 4
  };

  struct ParsingError
  {
    ErrorType type;
    std::string key;

    ParsingError(ErrorType _type = OTHER, const std::string &_key = "-")
        : type(_type), key(_key)
    {
    }
  };

public:
  ArgumentManager(const std::string &progName,
                  const std::string &progVersion,
                  const std::string &progAuthor,
                  const std::string &progDescription)
      : mProgName(progName), mProgVersion(progVersion), mProgAuthor(progAuthor), mProgDescription(progDescription), mPosArgs(Argument("positional", "", -1))
  {
    add("--help", 0, false, "outputs the programs usage");
  }

  virtual ~ArgumentManager() {}

  //setup positional arguments
  void positional(int nValue = -1, bool isRequired = false, const std::string &helpMsg = "")
  {
    mPosArgs.nvalue(nValue).is_required(false).help_message(helpMsg);
  }

  void positional(const std::vector<std::string> &value, bool isRequired = false, const std::string &helpMsg = "")
  {
    mPosArgs.value(value).is_required(false).help_message(helpMsg);
  }

  //setup named arguments
  void add(const std::string &name, int nValue = -1, bool isRequired = false, const std::string &helpMsg = "")
  {
    bool valid_name = is_valid_name(name);
    bool not_exist = (mArgMap.find(name) == mArgMap.end());

    if (not_exist && valid_name)
    {
      mArgs.push_back(Argument(name, helpMsg, nValue, isRequired));
      mArgMap[name] = mArgs.size() - 1;
    }
  }

  void add(const std::string &name, const std::vector<std::string> &value, bool isRequired = false, const std::string &helpMsg = "")
  {
    bool valid_name = is_valid_name(name);
    bool not_exist = (mArgMap.find(name) == mArgMap.end());

    if (not_exist && valid_name)
    {
      mArgs.push_back(Argument(name, value, helpMsg, isRequired));
      mArgMap[name] = mArgs.size() - 1;
    }
  }

  //accessing argument state
  bool found(const std::string &name) const
  {
    auto loc = mArgMap.find(name);
    return loc != mArgMap.end();
  }

  int number_of_param(const std::string &name) const
  {
    int n = -1;

    auto loc = mArgMap.find(name);
    bool exist = loc != mArgMap.end();
    if (exist)
      n = mArgs[loc->second].nvalue();

    return n;
  }

  //accessing argument value
  template <typename T>
  T get_as(const std::string &name, size_t n = 0) const
  {
    T out;

    auto loc = mArgMap.find(name);
    bool exist = loc != mArgMap.end();
    if (exist)
    {
      bool valid_param = n < mArgs[loc->second].nvalue();
      if (valid_param)
        cast<T>(mArgs[loc->second].value(n), out);
    }

    return out;
  }

  template <typename T>
  T get_as(size_t n = 0) const
  {
    T out;

    bool valid_param = n < mPosArgs.nvalue();
    if (valid_param)
      cast<T>(mPosArgs.value(n), out);

    return out;
  }

  //parse from CLI
  int parse(int argc, char **argv)
  {
    std::vector<std::string> tokens(argv, argv + argc);

    mErrorList.clear();

    //get binary name
    size_t pos = tokens[0].find_last_of("/\\");
    mBinaryName = tokens[0].substr(pos + 1);

    auto read_params = [this, &tokens](size_t i, int num, std::vector<std::string> &params) {
      bool undefined = num < 0;
      params.clear();

      size_t j = i;
      while (params.size() < num || undefined)
      {
        if (j >= tokens.size()) // end of tokens vector
          break;

        bool has_valid_name = this->is_valid_name(tokens[j]);
        if (has_valid_name) // current token is an option
          break;

        params.push_back(tokens[j]);
        j += 1;
      }

      return j;
    };

    //process tokens
    size_t i = 1;
    bool read_successfull;
    std::vector<std::string> params;

    //positional arguments;
    i = read_params(i, mPosArgs.expected_nvalue(), params);
    read_successfull = (params.size() > 0);
    if (read_successfull)
    {
      mPosArgs.value(params);
      mPosArgs.is_found(true);
    }

    // named arguments
    while (i < tokens.size())
    {
      auto loc = mArgMap.find(tokens[i]);

      bool valid = is_valid_name(tokens[i]);
      bool exist = loc != mArgMap.end();

      if (exist)
      {
        Argument &current = mArgs[loc->second];
        i = read_params(i + 1, current.expected_nvalue(), params);
        read_successfull = params.size() > 0;
        if (read_successfull)
          current.value(params);
        current.is_found(true);
      }
      else // just ignore
      {
        if (valid)
          mErrorList.push_back(ParsingError(INVALID_ARG, tokens[i]));
        else
          mErrorList.push_back(ParsingError(INVALID_TOKEN, tokens[i]));
        i += 1;
      }
    }

    int nerr = check_parsed_args();

    return nerr;
  }

  //output
  void output_info(std::ostream &stream = std::cout) const
  {
    bool has_name = mProgName.size() != 0;
    bool has_version = mProgVersion.size() != 0;
    bool has_author = mProgAuthor.size() != 0;
    bool has_description = mProgDescription.size() != 0;

    if (has_name)
      stream << mProgName;
    if (has_version)
      stream << " v" << mProgVersion;
    if (has_author)
      stream << " by " << mProgAuthor;

    if (has_name || has_author || has_author)
    {
      stream << std::endl;
      size_t n = mProgName.size() + mProgVersion.size() + mProgAuthor.size() + 6;
      for (int i = 0; i < n; ++i)
        stream << '=';
      stream << std::endl;
    }

    if (has_description)
      stream << mProgDescription << std::endl;
    stream << std::endl;
  }

  void output_usage(std::ostream &stream = std::cout) const
  {
    bool has_posArgs = mPosArgs.expected_nvalue() != 0;

    stream << "usage : " << mBinaryName;

    if (has_posArgs)
      stream << " <positional parameters>";
    stream << " [options] <params>" << std::endl;

    stream << "List of arguments:" << std::endl;
    stream << std::left << std::setw(10) << "name"
           << " | ";
    stream << std::left << std::setw(10) << "num param"
           << " ";
    stream << std::left << std::setw(10) << "required"
           << " ";
    stream << std::left << std::setw(10) << "desciption"
           << " " << std::endl;

    for (int i = 0; i < 11; ++i)
      stream << "-";
    stream << "|";
    for (int i = 0; i < 33; ++i)
      stream
          << "-";
    stream << std::endl;

    if (has_posArgs)
      stream << mPosArgs.usage_text() << std::endl;
    for (auto arg : mArgs)
      stream << arg.usage_text() << std::endl;
    stream << std::endl;
  }

  void output_content(std::ostream &stream = std::cout) const
  {
    stream << "argument value:" << std::endl;
    stream << std::left << std::setw(10) << "name"
           << " | ";
    stream << std::left << std::setw(10) << "is found"
           << " ";
    stream << std::left << std::setw(10) << "value"
           << " " << std::endl;

    for (int i = 0; i < 11; ++i)
      stream << "-";
    stream << "|";
    for (int i = 0; i < 22; ++i)
      stream << "-";
    stream << std::endl;

    bool has_posArgs = mPosArgs.expected_nvalue() != 0;

    if (has_posArgs)
      stream << mPosArgs.content_text() << std::endl;
    for (auto arg : mArgs)
      stream << arg.content_text() << std::endl;
    stream << std::endl;
  }

  void output_errors(std::ostream &stream = std::cout) const
  {
    if (mErrorList.size() > 0)
    {
      stream << "parsing errors:" << std::endl;

      for (auto err : mErrorList)
      {
        auto loc = mArgMap.find(err.key);

        switch (err.type)
        {
        case INVALID_ARG:
          stream << "  [WARNING] arg \"" << err.key << "\" does not exist. argument is ignored !!" << std::endl;
          break;
        case INVALID_TOKEN:
          stream << "  [WARNING] token \"" << err.key << "\" is not a valid key name. token is ignored !!" << std::endl;
          break;
        case MISSING_ARG:
          stream << "  [ERROR] required arg  \"" << err.key << "\" was not found !!" << std::endl;
          break;
        case MISSING_PARAM:
          stream << "  [ERROR] arg  \"" << err.key << "\" requires " << mArgs[loc->second].expected_nvalue() << " parameters;"
                 << " only " << mArgs[loc->second].nvalue() << " were parsed !!" << std::endl;
          break;
        default:
          break;
        }
      }

      stream << std::endl;
    }
  }

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
  inline void cast(const std::string &value, T &out) const
  {
    std::istringstream ss(value);
    ss >> out;
  }

  int check_parsed_args()
  {
    int n = 0;

    if (mPosArgs.is_required() && !mPosArgs.is_found())
    {
      mErrorList.push_back(ParsingError(MISSING_ARG, mPosArgs.name()));
      n += 1;
    }
    else if (mPosArgs.expected_nvalue() > mPosArgs.nvalue())
    {
      mErrorList.push_back(ParsingError(MISSING_PARAM, mPosArgs.name()));
      n += 1;
    }

    for (auto arg : mArgs)
    {
      if (arg.is_required() && !arg.is_found())
      {
        mErrorList.push_back(ParsingError(MISSING_ARG, arg.name()));
        n += 1;
      }
      else if (arg.expected_nvalue() > arg.nvalue())
      {
        mErrorList.push_back(ParsingError(MISSING_PARAM, arg.name()));
        n += 1;
      }
    }

    return n;
  }

private:
  // Program Info
  std::string mBinaryName;
  std::string mProgName;
  std::string mProgVersion;
  std::string mProgAuthor;
  std::string mProgDescription;

  // Program Arguments
  Argument mPosArgs;           //positional arguments
  std::vector<Argument> mArgs; //name arguments

  // Map to arguments
  std::unordered_map<std::string, size_t> mArgMap;

  // parsing errors
  std::vector<ParsingError> mErrorList;
};

#endif //ARGMGR_H