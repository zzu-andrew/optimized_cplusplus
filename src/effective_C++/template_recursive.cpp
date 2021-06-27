//
// Created by andrew on 2021/6/26.
//

#include <iostream>

/*
 * 使用模板元编程实现编译期间的递归调用
 * 可用于头文件的计算校验值的计算
 *
 * */

// 模板递归实现编译期间进行阶乘
template<unsigned n>
class Factorial { // 一般情况下: Factorial 的值
public:
    enum {value = n * Factorial<n - 1>::value};
};

// 特殊情况进行特化
template<>
class Factorial<0> {
public:
    enum {value = 1};   // 特殊情况下 Factorial<0>的值是1
};


void new_failed( ) {
    std::cout << " new failed " << std::endl;
}


int main(int argc, char *argv[]) {

    std::cout << Factorial<3>::value << std::endl;  // 6
    std::cout << Factorial<5>::value << std::endl;  // 120

    return 0;
}
