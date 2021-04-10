cmake_minimum_required(VERSION 3.2.0)

set(GLUTILS_SRC
    buffer.h      buffer.cpp
    program.h     program.cpp
    shader.h      shader.cpp)

add_library(glutils ${GLUTILS_SRC})