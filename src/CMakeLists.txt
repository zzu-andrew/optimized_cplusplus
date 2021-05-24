cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)

add_executable(test main.cpp)

add_executable(const_func basic/const_func.cpp)

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
