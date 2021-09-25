cmake_minimum_required(VERSION 3.2)

# 设置C++编译 选项
#set(CXX_FLAGS
#        -g
#        # -DVALGRIND
#        # -DCHECK_PTHREAD_RETURN_VALUE
#        # -D_FILE_OFFSET_BITS=64
#        #-Wall
#        # -Wextra
#        -fpermissive
#        # -Werror
#        # -Wconversion
#        # -Wno-unused-parameter
#        # -Wold-style-cast
#        # -Woverloaded-virtual
#        # -Wpointer-arith
#        # -Wshadow
#        # -Wwrite-strings
#        # -march=native
#        )

## 设置变量和宏定义
#add_definitions("-std=c++11 -O0 -g -Wall")
#

# 设置头文件搜索路径  ${PROJECT_SOURCE_DIR} 代表的事当前CMakeLists.txt所在的文件夹
include_directories("${PROJECT_SOURCE_DIR}/inc/")# basic

# API 设计模块设计

add_executable(select_sort sort/select_sort.cpp)
add_executable(bubble_sort sort/bubble_sort.cpp)


add_executable(array_list
        array_list/main.cpp
        array_list/array_list.cpp
        array_list/linear_list.cpp)
target_include_directories(array_list
        PUBLIC array_list)


