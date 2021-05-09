cmake_minimum_required(VERSION 3.2)

project(optimized_cplusplus)


add_executable(gtest_demo gtest_demo.cpp)
target_link_libraries(gtest_demo
        PUBLIC gtest
        PUBLIC pthread)
add_executable(gtest_sample gtest_sample.cpp)
# 要是没有主函数，使用的时候需要加上gtest_main 库
target_link_libraries(gtest_sample
        PUBLIC gtest
        PUBLIC pthread
        PUBLIC gtest_main)
add_executable(gtest_testcase gtest_testcase.cpp)
# 要是没有主函数，使用的时候需要加上gtest_main 库
target_link_libraries(gtest_testcase
        PUBLIC gtest
        PUBLIC pthread
        PUBLIC gtest_main)
add_executable(gtest_global gtest_global.cpp)
# 要是没有主函数，使用的时候需要加上gtest_main 库
target_link_libraries(gtest_global
        PUBLIC gtest
        PUBLIC pthread)

