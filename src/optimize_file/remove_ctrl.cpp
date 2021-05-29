//
// Created by andrew on 2021/5/24.
//


#include <benchmark/benchmark.h>
#include <string>
#include <cstring>


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

std::string remove_ctrl_reserve(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_reserve(benchmark::State& state)
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
        remove_ctrl_reserve(str);
    }
}
BENCHMARK(bench_remove_ctrl_reserve);


std::string remove_ctrl_ref_args(std::string &s) {
    std::string result;
    result.reserve(s.length());
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_ref_args(benchmark::State& state)
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
        remove_ctrl_ref_args(str);
    }
}
BENCHMARK(bench_remove_ctrl_ref_args);


std::string remove_ctrl_ref_args_it(std::string &s) {
    std::string result;
    result.reserve(s.length());
    for (auto &it:s) {
        if(it >= 0x20)
            result += it;
    }
    return result;
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_ref_args_it(benchmark::State& state)
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
        remove_ctrl_ref_args_it(str);
    }
}
BENCHMARK(bench_remove_ctrl_ref_args_it);

void remove_ctrl_ref_result_it(std::string &result, std::string const &s) {
    result.clear();
    result.reserve(s.length());
    for (auto &it:s) {
        if (it >= 0x20)
            result += it;
    }
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_ref_result_it(benchmark::State& state)
{
    std::string result{};
    for (auto _: state) {
        std::string str("test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++");
        remove_ctrl_ref_result_it(result, str);
    }
}
BENCHMARK(bench_remove_ctrl_ref_result_it);

void remove_ctrl_cstrings(char* destp, char const* srcp, size_t size) {
    for (size_t i=0; i<size; ++i) {
        if (srcp[i] >= 0x20)
            *destp++ = srcp[i];
    }
    *destp = 0;
}

// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_ctrl_cstrings(benchmark::State& state)
{
    char des[300];

    for (auto _: state) {
        const char *src = "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++";
        remove_ctrl_cstrings(des, src, strlen(src));
    }
}
BENCHMARK(bench_remove_ctrl_cstrings);

void remove_ctrl_block_append(std::string &result, const std::string &s) {
    result.clear();
    result.reserve(s.length());
    for (size_t b=0,i=b; b < s.length(); b = i+1) {
        for (i=b; i<s.length(); ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b, i-b);
    }
}
// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_block_append(benchmark::State& state)
{
    std::string result{};
    for (auto _: state) {
        std::string str("test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++"
                        "test string just for optimize C++");
        remove_ctrl_block_append(result, str);
    }
}
BENCHMARK(bench_remove_block_append);


void remove_ctrl_erase(std::string &s) {
    for (size_t i = 0; i < s.length();)
    if (s[i] < 0x20)
        s.erase(i,1);
    else ++i;
}

// 使用operator[]读取元素，依次存入1-6的平方
static void bench_remove_erase(benchmark::State& state)
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
        remove_ctrl_erase(str);
    }
}
BENCHMARK(bench_remove_erase);

BENCHMARK_MAIN();