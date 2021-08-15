cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)

add_executable(test main.cpp)


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


#target_include_directories(foo
#        PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
#        )

add_executable(const_func basic/const_func.cpp)
add_executable(condition_var basic/condition_var.cpp)
target_link_libraries(condition_var
        PUBLIC -pthread)

add_executable(make_a_tree basic/make_a_tree.cpp)


# 添加库文件
add_library(timer timer/timer_utils.cpp)
# so ensure we add that to the header search path
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(timer
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/timer)

add_executable(benchmark_example basic/benchmark_example.cpp)
target_link_libraries(benchmark_example
        PUBLIC benchmark
        PUBLIC -pthread)

add_executable(remove_ctrl optimize_file/remove_ctrl.cpp)
target_link_libraries(remove_ctrl
        PUBLIC benchmark
        PUBLIC -pthread)

add_executable(optimize_hotfunc optimize_file/optimized_hotfunc.cpp)
target_link_libraries(optimize_hotfunc
        PUBLIC benchmark
        PUBLIC -pthread)

add_executable(effective_basic effective_C++/effective_basic.cpp)
add_executable(auto_ptr_guard_data effective_C++/auto_ptr_guard_data.cpp)
add_executable(template_recursive effective_C++/template_recursive.cpp)
add_executable(operate_new_failed effective_C++/operate_new_failed.cpp)
add_executable(virtual_param effective_C++/virtual_param.cpp)
add_executable(write_file basic/write_file.cpp)

add_executable(function basic/function.cpp)
add_executable(return_value_optimization basic/return_value_optimization.cpp)

add_executable(concurrent_atomic_release_consume concurrent_programming/release_consume.cpp)
target_link_libraries(concurrent_atomic_release_consume
        PUBLIC -pthread)


add_executable(sequentially_consistent concurrent_programming/sequentially_consistent.cpp)
target_link_libraries(sequentially_consistent
        PUBLIC -pthread)


