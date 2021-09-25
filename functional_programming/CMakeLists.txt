cmake_minimum_required(VERSION 3.2)

## 设置变量和宏定义
#add_definitions("-std=c++11 -O0 -g -Wall")
#

# 设置头文件搜索路径  ${PROJECT_SOURCE_DIR} 代表的事当前CMakeLists.txt所在的文件夹
include_directories("${PROJECT_SOURCE_DIR}/inc/")# basic

add_executable(count_line count_line/count_file_line.cpp)

add_executable(average_score average_score/average_score.cpp)
target_include_directories(average_score
        PUBLIC inc)

add_executable(transform_filter transform_filter/transform_filter.cpp)
target_include_directories(transform_filter
        PUBLIC inc)