//
// Created by andrew on 2021/9/20.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>    // 数值计算库
#include "person.h"

using namespace std;

/*
 * 使用函数式编程实现平均值的求值
 * */

//Calculating the average score with std::accumulate.
double AverageScore(const std::vector<int> &scores) {
    /**
     * 1 迭代器 first 2 迭代器 last 3 求和的初始值
     */
    return static_cast<double>(std::accumulate(scores.cbegin(), scores.cend(), 0)) / scores.size();
}

// we are multiplying all scores
double ScoresProduct(const std::vector<int> &scores) {
    // std::multiplies<int>() 乘法， accumulate支持指定仿函数
    return std::accumulate(scores.cbegin(), scores.cend(), 1, std::multiplies<int>());
}


int main(int argc, char* argv[]) {

    std::cout << AverageScore({1, 2, 3, 4}) << std::endl;
    std::cout << ScoresProduct({1, 2, 3, 4}) << std::endl;

    return 0;
}
