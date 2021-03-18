#ifndef TIMER_H
#define TIMER_H

/* timer.h - v1.01
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
 */

#include <chrono>

/**
 * @Brief
 * A simple timer class
 * 
 * example:
 * -------
 * Timer t;
 * t.start();
 * <SOME CODE HERE>
 * float time = t.stop();
 * std::cout << "runnig code took " << t << " ms" << std::endl ;
 * 
 * or user one of the macros
 * 
 * float dt;
 * COMPUTE_TIME(<FUNCTION HERE>, dt)
 * BENCH_TIME(<FUNCTION HERE>, 100, dt)
 */
class Timer
{
public:
  enum UNIT
  {
    MICROSECONDS = 1,
    MILLISECONDS = 1000,
    SECONDS = 1000000,
    MINUTES = 60000000
  };

protected:
  //======================//
  //    Internal Types    //
  //===============================================================================================//
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::duration<float, std::micro> Duration;
  typedef std::chrono::time_point<Clock> TP;

public:
  Timer()
      : mRunning(false)
  {
    reset();
  }

  virtual ~Timer() {}

  //======================//
  //  Main Functionality  //
  //===============================================================================================//
  /**
   * @Brief
   * resets the timer
  **/
  inline void reset()
  {
    mRunning = false;
    mCurrent = TP::min();
    mTotal = Duration::zero();
  }

  /**
   * @Brief
   * starts the timer
  **/
  inline void start()
  {
    mRunning = true;
    mCurrent = Clock::now();
  }

  /**
   * @Brief
   * updates the total time and pauses the timer
  **/
  inline void pause()
  {
    TP temp = Clock::now();
    mRunning = false;

    mTotal += (temp - mCurrent);
    mCurrent = temp;
  }

  /**
   * @Brief
   * returns the total time and resets the timer
  **/
  inline float stop(UNIT unit = MILLISECONDS)
  {
    pause();
    float t = total(unit);
    reset();

    return t;
  }

  /**
   * @Brief
   * computes returns the time from the last time point
   * updates the total time and time point if the timer is running
  **/
  inline float lap(UNIT unit = MILLISECONDS)
  {
    TP temp = Clock::now();
    Duration d = (temp - mCurrent);
    if (mRunning)
    {
      mTotal += d;
      mCurrent = temp;
    }

    return (d.count() / float(unit));
  }

  /**
   * @Brief
   * returns the total time in the input unit.
  **/
  inline float total(UNIT unit = MILLISECONDS)
  {
    return (mTotal.count() / float(unit));
  }

protected:
  bool mRunning;   // indicates whether the timer is running
  TP mCurrent;     // the last time point
  Duration mTotal; // the time from the start of the timer
};

#define COMPUTE_TIME(fun, dt) \
  {                           \
    Timer __timer__;          \
    __timer__.start();        \
    fun;                      \
    dt = __timer__.stop();    \
  }

#define BENCH_TIME(fun, n, avgt) \
  {                              \
    Timer __timer__;             \
    avgt = 0.;                   \
    for (int i = 0; i < n; ++i)  \
    {                            \
      __timer__.start();         \
      fun;                       \
      avgt += __timer__.stop();  \
    }                            \
    avgt = avgt / float(n);      \
  }

#endif //TIMER_H