//
// Created by andrew on 2021/5/31.
//
#include <benchmark/benchmark.h>
#include <string>
#include <cstring>
#include <bitset>

/*
 *
 * 这段代码对字符串中的每个字符都会判断循环条件 i < strlen(s) 是否成立 1。调用
strlen() 的开销是昂贵的，遍历参数字符串对它的字符计数使得这个算法的开销从 O(n)
变为了 O(n2)。这是一个在库函数中隐藏了循环（请参见 3.5.2 节）的典型例子
 * */

static void find_blank(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";

        for (size_t i = 0; i < strlen(s); ++i)
            if (s[i] == ' ')
                s[i] = '*';
    }
}
BENCHMARK(find_blank);

static void find_blank_init_length(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";

        for (size_t i = 0, len = strlen(s); i < len; ++i)
            if (s[i] == ' ')
                s[i] = '*';
    }
}

BENCHMARK(find_blank_init_length);

static void find_blank_do_while(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";
        size_t i = 0, len = strlen(s);
        do {
            if (s[i] == ' ')
                s[i] = '*';
            ++ i;
        }while (i < len);
    }
}

BENCHMARK(find_blank_do_while);


void switch_test(int idx) {
    switch(idx)
    {
        case 1:
            break;
        case 100000000:
            break;

        default:
            break;
    }
}

static void switch_bench_mark_test(benchmark::State& state) {

    for (auto _: state) {
        switch_test(1);
        switch_test(100000000);
    }
}

BENCHMARK(switch_bench_mark_test);

void switch_test_op(int idx) {
    switch(idx)
    {
        case 1:
            break;
        case 2:
            break;

        default:
            break;
    }

}

static void switch_bench_mark_test_op(benchmark::State& state) {

    for (auto _: state) {
        switch_test(1);
        switch_test(2);
    }
}

BENCHMARK(switch_bench_mark_test_op);


BENCHMARK_MAIN();