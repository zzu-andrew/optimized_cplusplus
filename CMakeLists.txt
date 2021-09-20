cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)

set(CMAKE_CXX_FLAGS "-std=c++11")

# O2 for lantency test
#add_definitions("-std=c++11 -O0 -g -pg -Wall")
add_definitions("-std=c++11 -O2 -g -Wall")
#set(CXX_FLAGS
#    #-g
#    # -DVALGRIND
#    # -DCHECK_PTHREAD_RETURN_VALUE
#    # -D_FILE_OFFSET_BITS=64
#     -Wall
#    # -Wextra
#    -fpermissive
#    # -Werror
#    # -Wconversion
#    # -Wno-unused-parameter
#    # -Wold-style-cast
#    # -Woverloaded-virtual
#    # -Wpointer-arith
#    # -Wshadow
#    # -Wwrite-strings
#    # -march=native
#)

message(STATUS "==========" ${PROJECT_SOURCE_DIR})

include_directories("${PROJECT_SOURCE_DIR}/inc/")

add_subdirectory(src)
add_subdirectory(gtest)
add_subdirectory(api_design)