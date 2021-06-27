cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)

add_executable(test main.cpp)

# basic

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


