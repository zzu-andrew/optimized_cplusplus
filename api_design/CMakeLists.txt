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

add_executable(shared_pointer qualities/shared_pointer/main.cpp qualities/shared_pointer/myobject.cpp)
target_include_directories(shared_pointer
        PUBLIC qualities/shared_pointer)
target_link_libraries(shared_pointer
        PUBLIC -pthread)

add_executable(callback_static qualities/callback_static/main.cpp
            qualities/callback_static/modulea.cpp
            qualities/callback_static/moduleb.cpp)
target_include_directories(callback_static
        PUBLIC qualities/callback_static)
target_link_libraries(callback_static
        PUBLIC -pthread)

add_executable(callback_instance qualities/callback_instance/main.cpp
        qualities/callback_instance/modulea.cpp
        qualities/callback_instance/moduleb.cpp)
target_include_directories(callback_instance
        PUBLIC qualities/callback_instance)
target_link_libraries(callback_instance
        PUBLIC -pthread)


