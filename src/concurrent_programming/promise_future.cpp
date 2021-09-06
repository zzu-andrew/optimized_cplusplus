//
// Created by Achou.Wang on 2021/8/31.
//
#include <iostream>
#include <future>
#include <string>
#include <chrono>

/*
 * std::promise 多线程之间共享数据
 * */

// 使用promise设置数据
void WorkThread1(std::promise<std::string> &p) {
    // 为了使效果更加明显
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 传入数据
    std::string name("promise set.");
    std::cout << "promise set : " << name << std::endl;
    /* Stores the value into the shared state without making the state ready immediately.
     * The state is made ready when the current thread exits, after all variables
     * with thread-local storage duration have been destroyed.*/
    // 只要在线程退出，当前线程中的局部变量一类的都清理完全之后才会设置值，通常用于管理线程中获取线程的状态
//    p.set_value_at_thread_exit()
    p.set_value(name);
    std::this_thread::sleep_for(std::chrono::seconds(6));
}

// 使用future获取数据
void WorkThread2(std::future<std::string> &fu) {
    // 是future 的 get接口获取数据，会一直阻塞，直到有数据到来
    std::cout << "future get a string data." << std::endl;
    auto name = fu.get();
    std::cout << "future get name : " << name << std::endl;
}


int main()
{
    // 先声明promise
    std::promise<std::string> p1;
    // 要从promise里获取出future
    std::future<std::string> fu = p1.get_future();

    //    创建线程t1
    std::thread t1(WorkThread1, std::ref(p1));
    //    创建线程t2
    std::thread t2(WorkThread2, std::ref(fu));

    //    阻塞等待线程结束
    t2.join();
    t1.join();

    return 0;
}