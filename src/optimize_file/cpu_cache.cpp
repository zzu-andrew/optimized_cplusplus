//
// Created by andrew on 2021/9/20.
//
#include <iostream>
#include <vector>
#include <sched.h>
#include <ctime>
#include <fstream>

using namespace std;

#define CACHE_LINE_SIZE 64

struct Node {
    Node *next;
    char paddings[CACHE_LINE_SIZE - sizeof(void *)];
};

// 内存扫描，使用链表的方式每次调用指针去踩内存
#define N1(node) node=node->next;
#define N2(node) N1(node);N1(node);
#define N4(node) N2(node);N2(node);
#define N8(node) N4(node);N4(node);
#define N16(node) N8(node);N8(node);
#define N32(node) N16(node);N16(node);
#define N64(node) N32(node);N32(node);
#define N128(node) N64(node);N64(node);
#define N256(node) N128(node);N128(node);
#define N512(node) N256(node);N256(node);
#define N1024(node) N512(node);N512(node);

const long long sec2non = 1000000000;

const Node *Test(int T, const Node *c, std::size_t size, ofstream & outFile) {

    // 提前定义，防止在时间打点的时候将变量申请的时间计算在内
    struct timespec start{};
    struct timespec end{};
    const Node* node = nullptr;
    //< start
    clock_gettime(CLOCK_MONOTONIC, &start);
    // 保证测试的次数，也就是踩内存的次数和T一样
    const int M = static_cast<int>(T / size);
    for (int i = 0; i < M; i ++) {
        node  = c;
        const int s = static_cast<int>(size / 64);
        for (int  j = 0;  j < s; ++ j) {
            // 踩缓存
            N64(node);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    //< end
    // 计算总耗时
    auto elapsed = (end.tv_sec - start.tv_sec) * sec2non + end.tv_nsec - start.tv_nsec;
    // 总耗时 除缓存命中次数，计算缓存命中耗时
    std::cout << "elapsed milliseconds per elements : " << elapsed / M << "ns" << std::endl;
    outFile << elapsed / M << endl;

    return node;
}

/*
 * @brief 系统缓存信息
 * L1d 缓存：                       128 KiB
 * L1i 缓存：                       128 KiB
 * L2 缓存：                        1 MiB
 * L3 缓存：                        8 MiB
 * */

//< rdmsr: version msr-tools-1.3 rdmsr 版本信息
//< rdmsr - tool for reading CPU machine specific registers (MSR)
int main(int argc, char*argv[]) {

    if (argc < 3) {
        std::cout << "argc : " << argc << std::endl;
        return -1;
    }

    // 清空CPU 参数，避免影响缓存命中
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    sched_setaffinity(0, sizeof(set), &set);
    // 16M，保证内存数量大于一二级缓存的量，才能保证系统会将缓存放到三级缓存里面
    // 如果一级 二级缓存够使用 系统将不会将内存放到三级缓存里面
    const int N = 1024 * 1024 * 16;   // * (64)
    // 使用vector搞一个环形buffer
    std::vector<Node> va;
    va.reserve(N);
    for (int i = 0; i < N - 1; i ++) {
        va[i].next = &(va[i + 1]);
    }
    va[N - 1].next = &va[0];

    const int M = 1000000;
    char *endPtr = nullptr;
    // 转换成10进制数
    const int beginKb = static_cast<int>(strtol(argv[1], &endPtr, 10));
    const int endKb = static_cast<int>(strtol(argv[2], &endPtr, 10));

    const Node* node = nullptr;
    // 耗时统计
    // 耗时统计
    ofstream outfile("./ou.csv", ios::trunc);
    for (int i = beginKb; i < endKb; i += 4) {
        std::cout << "test Kb : " << i << std::endl;
        std::cerr << i << '\t';
        node = Test(M, &va[0], i * 1024 / sizeof(Node), outfile);
    }

    outfile.close();

    std::cout << node << '\t' << &va[0] << std::endl;
    return 0;
}


