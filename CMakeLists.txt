project(CPPUtilCode)

cmake_minimum_required(VERSION 3.2.0)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# OPTIONS #########################################################################################
option(ARGMGR_EXAMPLE "compile argmgr example" ON)
option(TIMER_EXAMPLE "compile timer example" ON)
option(GLUTILS_EXAMPLE "compile glutils examples" ON)

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
  add_executable(argmgr examples/argmgr_example.cpp)
endif()

if(TIMER_EXAMPLE)
  add_executable(timer examples/timer_example.cpp)
endif()

if(GLUTILS_EXAMPLE)
  # glbinding
  find_package(glbinding)
  if(GLBINDING_FOUND)
    include_directories(${GLBINDING_INCLUDE_DIR})
    set(EXTLIBS ${EXTLIBS} ${GLBINDING_LIBRARIES})
    message("${GLBINDING_INCLUDE_DIR}")
    message("${GLBINDING_LIBRARIES}")
  endif()
  # gluitls
  add_subdirectory("src/glutils")

  
endif()
