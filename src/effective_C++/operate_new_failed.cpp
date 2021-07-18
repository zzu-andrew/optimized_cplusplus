//
// Created by andrew on 2021/6/27.
//
#include <iostream>

/*
 * 通过set_new_handler设置标准库中 new函数失败调用的函数
 * 方便调试标准函数调用失败之后的过程
 * */


/*
 * 一旦出现申请内存失败，new会调用new_handler 并期望该函数能够提供能够
 * 申请出对应内存的方法，如果不能还是会一直调用，停止的方法就是在回调函数中将
 * 对应的new_handler设置成nullptr
 * */

void new_failed( ) {
    std::cout << " new failed " << std::endl;
    //  及时的将  new_handler 只有这样才会停止 new_handler的调用
    std::set_new_handler(nullptr);
}

struct new_data{
    double res[10000000];
    double res1[10000000];
    double res2[10000000];
    double res3[10000000];
    double res4[10000000];
    double res5[10000000];
    double res6[10000000];
    double res7[10000000];
    double res8[10000000];
    double res9[10000000];
    double res0[10000000];
};

int main(int argc, char *argv[]) {

    std::new_handler oldHandler = std::set_new_handler(new_failed);

    // 申请失败会抛出 bad_alloc或派生自bad_alloc的异常，并且这个异常不会被operator捕捉，因此会被传播到内存的索求处
    new_data *pInt = new new_data[1000000000];

    // 这样使用也要保证 new_data的构造函数不抛出任何异常
    new_data *pNoThrowInt = new(std::nothrow) new_data[1000000000];

    std::set_new_handler(oldHandler);
    delete[] pNoThrowInt;

    delete[] pInt;


    return 0;
}
