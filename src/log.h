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

#ifndef __LOG_H__
#define __LOG_H__

#include <stdarg.h>
#include <stdio.h>

#if defined _WIN32
#include <windows.h>
#endif

#define PRINT_ERROR(...) Log::write(stderr, Log::LVL_ERROR, Log::TXT_DEFAULT, Log::TXT_RED, __VA_ARGS__)
#define PRINT_ERROR_ORIGIN(...)                                                                          \
  Log::write(stderr, Log::LVL_ERROR, Log::TXT_BRIGHT, Log::TXT_RED, "[%s, ln %d] ", __FILE__, __LINE__); \
  Log::write(stderr, Log::LVL_ERROR, Log::TXT_DEFAULT, Log::TXT_RED, __VA_ARGS__)

#define PRINT_WARNING(...) Log::write(stderr, Log::LVL_WARNING, Log::TXT_DEFAULT, Log::TXT_YELLOW, __VA_ARGS__)
#define PRINT_WARNING_ORIGIN(...)                                                                          \
  Log::write(stderr, Log::LVL_WARNING, Log::TXT_BRIGHT, Log::TXT_RED, "[%s, ln %d] ", __FILE__, __LINE__); \
  Log::write(stderr, Log::LVL_WARNING, Log::TXT_DEFAULT, Log::TXT_RED, __VA_ARGS__)

#define PRINT_DEBUG(...) Log::write(stdout, Log::LVL_DEBUG, Log::TXT_BRIGHT, Log::TXT_WHITE, __VA_ARGS__)
#define PRINT_DEBUG_COLORED(COLOR, ...) Log::write(stdout, Log::LVL_DEBUG, Log::TXT_BRIGHT, COLOR, __VA_ARGS__)

#define PRINT(...) Log::write(stdout, Log::LVL_INFO_1, Log::TXT_DEFAULT, Log::TXT_WHITE, __VA_ARGS__)
#define PRINT_COLORED(COLOR, ...) Log::write(stdout, Log::LVL_INFO_1, Log::TXT_DEFAULT, COLOR, __VA_ARGS__)

#define PRINT_ALWAYS(...) Log::write(stdout, Log::LVL_MUTE, Log::TXT_DEFAULT, Log::TXT_WHITE, __VA_ARGS__)
#define PRINT_ALWAYS_COLORED(COLOR, ...) Log::write(stdout, Log::LVL_MUTE, Log::TXT_DEFAULT, COLOR, __VA_ARGS__)

#define PRINT_DETAIL(...) Log::write(stdout, Log::LVL_DETAIL, Log::TXT_DEFAULT, Log::TXT_WHITE, __VA_ARGS__)
#define PRINT_DETAIL_COLORED(COLOR, ...) Log::write(stdout, Log::LVL_DETAIL, Log::TXT_DEFAULT, COLOR, __VA_ARGS__)

#define PRINT_VERBOSE(VERBOSE, ...) Log::write(stdout, VERBOSE, Log::TXT_DEFAULT, Log::TXT_WHITE, __VA_ARGS__)
#define PRINT_VERBOSE_COLORED(VERBOSE, COLOR, ...) Log::write(stdout, VERBOSE, Log::TXT_DEFAULT, COLOR, __VA_ARGS__)

class Log
{
public:
  enum EFFECT
  {
    TXT_DEFAULT = 0,
    TXT_BRIGHT = 1,
    TXT_UNDERLINE = 2
  };

  enum COLOR
  {
    TXT_BLACK = 0,
    TXT_RED = 1,
    TXT_GREEN = 2,
    TXT_BLUE = 3,
    TXT_YELLOW = 4,
    TXT_MAGENTA = 5,
    TXT_GYAN = 6,
    TXT_WHITE = 7
  };

  enum VERBOSE
  {
    LVL_MUTE = 0,
    LVL_ERROR = 1,
    LVL_WARNING = 2,
    LVL_DEBUG = 3,
    LVL_INFO_1 = 4,
    LVL_INFO_2 = 5,
    LVL_INFO_3 = 6,
    LVL_DETAIL = 7
  };

public:
  static void write(FILE *stream, const char *msg, ...)
  {
    va_list arg;

    va_start(arg, msg);
    vfprintf(stream, msg, arg);
    fflush(stream);
    va_end(arg);
  }

  static void write(FILE *stream, VERBOSE lvl, EFFECT e, COLOR c, const char *msg, ...)
  {
    if (mVerboseCheck && mVerboseLvl < lvl)
      return;

    set_text_style(stream, e, c);

    va_list arg;

    va_start(arg, msg);
    vfprintf(stream, msg, arg);
    fflush(stream);
    va_end(arg);

    reset_text_style(stream);
  }

  static void enable_verbose_check() { mVerboseCheck = true; }

  static void disable_verbose_check() { mVerboseCheck = false; }

  static void set_verbose_level(VERBOSE lvl) { mVerboseLvl = lvl; }

  static void set_text_style(FILE *stream, EFFECT e, COLOR c)
  {
#if defined _WIN32
    HANDLE h = GetStdHandle((stream == stdout) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    SetConsoleTextAttribute(h, mEffects[e] | mColors[c]);
#endif
  }

  static void reset_text_style(FILE *stream)
  {
    set_text_style(stream, TXT_DEFAULT, TXT_WHITE);
  }

protected:
  inline static bool mVerboseCheck = true;
  inline static VERBOSE mVerboseLvl = LVL_INFO_1;

#if defined _WIN32
  inline static WORD mEffects[3] = {0, FOREGROUND_INTENSITY, COMMON_LVB_UNDERSCORE};
  inline static WORD mColors[8] = {0, FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE};
#endif
};

#endif //__LOG_H__