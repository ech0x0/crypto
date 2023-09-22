cmake_minimum_required(VERSION 3.24)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(crypto VERSION 1.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(crypto ${SRC_FILES})

if(MSVC)
  target_compile_options(crypto PRIVATE /W4)
else()
  target_compile_options(crypto PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(crypto gmp)