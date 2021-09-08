//
// Created by andrew on 2021/9/7.
//

#include "modulea.h"
#include "moduleb.h"
#include <thread>

/*
 * c++中调用静态回调函数，下一节介绍c++中调用非静态成员函数
 * */

int main(int argc, char*argv[]) {

    std::cout << "start callback module demo." << std::endl;
    // C++中调用静态成员函数比较简单，不需要绑定类对象就可以直接调用
    ModuleB b;
    int closure = 32;

    // add a callback for ModuleA to ModuleB
    b.SetCallback(ModuleA::StaticMethod, &closure);
    // invoke callback function
    b.DoAction();

    std::cout << "exit main ......" << std::endl;

    return 0;
}


