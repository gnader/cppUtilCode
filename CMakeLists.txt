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
  add_executable(_argmgr examples/argmgr.cpp)
endif()

if(TIMER_EXAMPLE)
  add_executable(_timer examples/timer.cpp)
endif()

if(GLUTILS_EXAMPLE)
  #glfw
  find_package(GLFW REQUIRED)
  if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    add_definitions(-DGLFW_INCLUDE_NONE)
    message("glfw:")
    message("${GLFW_INCLUDE_DIR}")
    message("${GLFW_LIBRARIES}")
  endif()
  # glbinding
  find_package(GLBINDING REQUIRED)
  if(GLBINDING_FOUND)
    include_directories(${GLBINDING_INCLUDE_DIR})
    message("glbinding:")
    message("${GLBINDING_INCLUDE_DIR}")
    message("${GLBINDING_LIBRARIES}")
  endif()

    # gluitls
  add_subdirectory("src/glutils")
    
  add_executable(_glutils examples/glutils.cpp)
  target_link_libraries(_glutils glutils ${GLBINDING_LIBRARIES} ${GLFW_LIBRARIES})
endif()
