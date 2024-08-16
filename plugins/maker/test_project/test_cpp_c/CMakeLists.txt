cmake_minimum_required(VERSION 3.14)

project(test_cpp_c LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(test_cpp_c
  main.cpp
  filec.c
  filec.h
  ../make.js
  ../somelib/make.js
)
