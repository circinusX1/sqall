cmake_minimum_required(VERSION 3.14)

project(sq_maker LANGUAGES CXX)

include_directories(../../simplesquirrel/include
                    ../../simplesquirrel/source/
                    ../../simplesquirrel/libs/squirrel/include
                    ../../)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_EXE_LINKER_FLAGS -rdynamic)

set(BIN_NAME sq_maker)
set(LIB_FILENAME "lib${BIN_NAME}.so")


add_library(sq_maker SHARED
  maker_global.h
  maker.cpp
  maker.h
  varss.h varss.cpp
  utils.h utils.cpp
  ../../bin/make.js
)

## copy to bin
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_BINARY_DIR}/${LIB_FILENAME}
    ../../bin/${LIB_FILENAME}
)

