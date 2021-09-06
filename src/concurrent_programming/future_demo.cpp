//
// Created by andrew on 2021/8/30.
//
#include <future>
#include <iostream>
#include <thread>

int find_the_answer_to_ltuae() {
    return 3;
}

void do_other_stuff() {
    std::this_thread::sleep_for(std::chrono::seconds(6));
}

int main(int argc, char *argv[])
{
    // 新启用一个线程 find_the_answer_to_ltuae
    std::future<int> the_answer=std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout<<"The answer is "<<the_answer.get()<<std::endl;
    //
    return 0;
}
