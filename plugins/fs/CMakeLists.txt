cmake_minimum_required(VERSION 3.14)

project(sq_system LANGUAGES CXX)

include_directories(../../simplesquirrel/include
                    ../../simplesquirrel/source/
                    ../../simplesquirrel/libs/squirrel/include
                    ../../)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_EXE_LINKER_FLAGS -rdynamic)

set(BIN_NAME sq_system)
set(LIB_FILENAME "lib${BIN_NAME}.so")


add_library(sq_system SHARED
  fs_global.h
  fs.cpp
  fs.h
)

## copy to bin
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_BINARY_DIR}/${LIB_FILENAME}
    ${PROJECT_SOURCE_DIR}/../../bin/${LIB_FILENAME}
)

