//
// Created by andrew on 2021/9/19.
//

#include <iostream>

using namespace std;


// declare the external linkage elements of myobject.cpp
extern void FreeFunction();
extern int INTERNAL_CONSTANT;
extern std::string Filename;

int main(int argc, char *argv[]) {

    // 不通过公有API调用私有函数和修改全局变量
    // call a function hidden within the myobject.cpp module
    FreeFunction();

    // access a constant defined within myobject.cpp
    std::cout << "Internal constant = " << INTERNAL_CONSTANT << std::endl;

    // change global state within myobject.cpp
    Filename = "different.txt";



    return 0;
}

