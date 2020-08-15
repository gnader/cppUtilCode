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

# LIB #############################################################################################
# set(CPPUITLS_SRC_FILES)

set(CPPUTILS_HEADER_FILES
    src/args.h
    src/sampling.h
    src/timer.h)


# EXAMPLES ########################################################################################
add_executable(args_example examples/args_example.cpp)
add_executable(distribution_example examples/distribution_example.cpp)
add_executable(sampling_example examples/sampling_example.cpp)