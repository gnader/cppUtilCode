project(CPPUtilCode)

cmake_minimum_required(VERSION 3.2.0)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# COMPILER OPTIONS ################################################################################
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

## EXTERNAL LIBS ###############################################################
find_package(Eigen REQUIRED)
include_directories(${EIGEN_INCLUDE_DIRS})

# FILES ###########################################################################################
include_directories(${PROJECT_SOURCE_DIR}/src)

# EXAMPLES ########################################################################################
add_executable(args_example examples/argmgr_example.cpp)
add_executable(timer_example examples/timer_example.cpp)