project(CPPUtilCode)

cmake_minimum_required(VERSION 3.2.0)

# COMPILER OPTIONS ################################################################################
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# FILES ###########################################################################################
include_directories(${PROJECT_SOURCE_DIR}/src)

# LIB #############################################################################################
# set(CPPUITLS_SRC_FILES)

set(CPPUTILS_HEADER_FILES
    src/args.h)

# add_library(cpputils ${FUSION_SRC_FILES})

# EXAMPLES ########################################################################################
add_executable(args_example examples/args_example.cpp)