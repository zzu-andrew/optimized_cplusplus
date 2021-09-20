//
// Created by andrew on 2021/9/19.
//
#include <iostream>
#include "my_shared_object.h"


using namespace std;

class APIInitMgr
{
public:
    APIInitMgr()
    {
        cout << "APIInitMgr Initialized." << endl;
    }

    ~APIInitMgr()
    {
        cout << "APIInitMgr Destroyed." << endl;
    }
};

static APIInitMgr foo = APIInitMgr();

// 自动调用的函数
 __attribute__((constructor)) void APIInitialize() {
     // cout 是一个对象，执行结束之后会执行析枸函数，constructor标记的函数中不能执行析枸函数
//    cout << "api initialized." << endl;
    // 这这里不要有析枸
//     APIInitMgr mgr;
    // 这个函数中只能调用不是对象的方法
    printf("init\n");
}

// 自动调用的函数
__attribute__((destructor))  void APICleanUp() {
    cout << "api clean up." << endl;
}

void DoSomething(const std::string &name) {
    cout << "name is : " << name << endl;
}