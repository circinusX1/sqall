cmake_minimum_required(VERSION 3.14)

project(somelib LANGUAGES C CXX)


add_library(somelib SHARED

  somelib.c
  somelib.h
)


target_compile_definitions(somelib PRIVATE SOMELIB_LIBRARY)
