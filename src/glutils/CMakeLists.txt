cmake_minimum_required(VERSION 3.2.0)

set(GLUTILS_SRC
    program.cpp
    shader.cpp)

add_library(glutils ${GLUTILS_SRC})