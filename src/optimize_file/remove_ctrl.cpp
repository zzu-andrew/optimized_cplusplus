//
// Created by andrew on 2021/5/24.
//


#include <benchmark/benchmark.h>
#include <string>


std::string remove_ctrl(std::string s) {
    std::string result;
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result = result + s[i];
    }
    return result;
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_operator(benchmark::State& state)
{
    for (auto _: state) {
        std::string str("test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++");
        remove_ctrl(str);
    }
}
BENCHMARK(bench_remove_ctrl_operator);

std::string remove_ctrl_opt(std::string s) {
    std::string result;
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_operator_opt(benchmark::State& state)
{
    for (auto _: state) {
        std::string str("test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++");
        remove_ctrl(str);
    }
}
BENCHMARK(bench_remove_ctrl_operator_opt);


BENCHMARK_MAIN();