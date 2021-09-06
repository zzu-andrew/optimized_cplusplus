//
// Created by andrew on 2021/9/6.
//
#include <iostream>
#include <string>
#include "myobject.h"
#include <thread>

int main(int argc, char*argv[]) {

    std::cout << "start shared pointer demo." << std::endl;

    {
        // 共享指针，当指针不再指向的对象指针肯定在当前scope结束之前释放对象，但是具体释放的时间不确定
        MyObjectPtr ptr;
        // create a new instance of MyObject
        std::cout << "create instance of 1" << std::endl;
        ptr = MyObject::CreateInstance(1);
//        std::cout << "sleep for 3 seconds...." << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(3));
        // create a new instance of MyObject
        std::cout << "create instance of 2" << std::endl;
        ptr = MyObject::CreateInstance(2);

        std::cout << "create instance of 3" << std::endl;
        ptr = MyObject::CreateInstance(3);

        std::cout << "create instance of 4" << std::endl;
        ptr = MyObject::CreateInstance(4);

        std::cout << "create instance of 5" << std::endl;
        ptr = MyObject::CreateInstance(5);

        std::cout << "create instance of 6" << std::endl;
        ptr = MyObject::CreateInstance(6);

    }

    //    both instances get deleted when they go out of scope

    std::cout << "exit main ......" << std::endl;

    return 0;
}


/*
 * out
 * start shared pointer demo.
create instance of 1
...... create object 1
create instance of 2
...... create object 2
........ destroyed object 1
create instance of 3
...... create object 3
........ destroyed object 2
create instance of 4
...... create object 4
........ destroyed object 3
create instance of 5
...... create object 5
........ destroyed object 4
create instance of 6
...... create object 6
........ destroyed object 5
........ destroyed object 6
exit main ......

Process finished with exit code 0
 *
 * */