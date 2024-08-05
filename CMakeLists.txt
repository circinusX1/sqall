cmake_minimum_required(VERSION 3.14)

project(sq_all LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include_directories (${CMAKE_CURRENT_SOURCE_DIR}/simplesquirrel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/simplesquirrel/include/simplesquirrel
    ${CMAKE_CURRENT_SOURCE_DIR}/simplesquirrel/libs/squirrel/include
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sq_all
    main.cpp
    simplesquirrel/source/args.cpp simplesquirrel/source/array.cpp
    simplesquirrel/source/function.cpp
    simplesquirrel/source/enum.cpp
    simplesquirrel/source/object.cpp
    simplesquirrel/source/class.cpp
    simplesquirrel/source/vm.cpp
    simplesquirrel/source/table.cpp
    simplesquirrel/source/instance.cpp
    simplesquirrel/source/script.cpp

    simplesquirrel/source/sq_module.cpp
    bin/example_functions.nut
    bin/system.js
    simplesquirrel/include/simplesquirrel/sq_module.hpp
    simplesquirrel/include/simplesquirrel/object.hpp
    simplesquirrel/include/simplesquirrel/args.hpp
    simplesquirrel/include/simplesquirrel/class.hpp
    simplesquirrel/include/simplesquirrel/type.hpp
    simplesquirrel/include/simplesquirrel/enum.hpp
    simplesquirrel/include/simplesquirrel/script.hpp
    simplesquirrel/include/simplesquirrel/allocators.hpp
    simplesquirrel/include/simplesquirrel/instance.hpp
    simplesquirrel/include/simplesquirrel/table.hpp
    simplesquirrel/include/simplesquirrel/function.hpp
    simplesquirrel/include/simplesquirrel/exceptions.hpp
    simplesquirrel/include/simplesquirrel/binding.hpp
    simplesquirrel/include/simplesquirrel/array.hpp
    simplesquirrel/include/simplesquirrel/vm.hpp
    simplesquirrel/include/simplesquirrel/simplesquirrel.hpp
)
set(BIN_NAME sq_all)
target_link_libraries(sq_all -L${CMAKE_CURRENT_SOURCE_DIR}/simplesquirrel/libs
    -lsquirrel_static
    -lsqstdlib_static)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_BINARY_DIR}/${BIN_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${BIN_NAME}
)


