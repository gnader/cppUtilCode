project(CPPUtilCode)

cmake_minimum_required(VERSION 3.2.0)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# OPTIONS #########################################################################################
option(ARGMGR_EXAMPLE "compile argmgr example" ON)
option(TIMER_EXAMPLE "compile timer example" ON)

# COMPILER OPTIONS ################################################################################
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

# FILES ###########################################################################################
include_directories(${PROJECT_SOURCE_DIR}/src)

# EXAMPLES ########################################################################################
if(ARGMGR_EXAMPLE)
  add_executable(_argmgr examples/argmgr.cpp)
endif()

if(TIMER_EXAMPLE)
  add_executable(_timer examples/timer.cpp)
endif()