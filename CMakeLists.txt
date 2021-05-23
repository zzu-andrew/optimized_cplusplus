cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)

set(CMAKE_CXX_FLAGS "-std=c++11")


add_definitions("-std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1")

set(CXX_FLAGS
    #-g
    # -DVALGRIND
    # -DCHECK_PTHREAD_RETURN_VALUE
    # -D_FILE_OFFSET_BITS=64
     -Wall
    # -Wextra
    -fpermissive
    # -Werror
    # -Wconversion
    # -Wno-unused-parameter
    # -Wold-style-cast
    # -Woverloaded-virtual
    # -Wpointer-arith
    # -Wshadow
    # -Wwrite-strings
    # -march=native
)

message(STATUS "==========" ${PROJECT_SOURCE_DIR})

include_directories("${PROJECT_SOURCE_DIR}/inc/")

add_subdirectory(src/)
add_subdirectory(gtest/)