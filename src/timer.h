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

#ifndef __TIMER_H__
#define __TIMER_H__

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

//===============================================================================================//
//                                       CLASS DEFINITION                                        //
//===============================================================================================//

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

//===============================================================================================//
//                                            MACROS                                             //
//===============================================================================================//

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