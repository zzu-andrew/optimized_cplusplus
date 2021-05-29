//
// Created by andrew on 2021/5/29.
//
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

using namespace std;


class ConditionVariable {
public:
    ~ConditionVariable() = default;

    ConditionVariable()  = default;

    const queue<int> &getDataQueue() const {
        return data_queue;
    }

    void setDataQueue(const queue<int> &dataQueue) {
        data_queue = dataQueue;
    }

    void insertDataQueue(int insertData) {
        data_queue.push(insertData);
    }

    void dataPrepare(int data) {


        std::unique_lock<std::mutex> lk(m_mut);
        data_cont.wait(lk, [this]{return !data_queue.empty();});
    }

private:
    // 定义互斥锁
    mutable std::mutex m_mut;
    std::queue<int> data_queue;
    std::condition_variable data_cont;

};

// std::adopt_lock, std::defer_lock
std::mutex foo,bar;

void task_a () {
    std::lock (foo,bar);         // simultaneous lock (prevents deadlock)
    // adopt_lock 获取一把已经加锁的锁
    std::unique_lock<std::mutex> lck1 (foo,std::adopt_lock);
    std::unique_lock<std::mutex> lck2 (bar,std::adopt_lock);
    std::cout << "task a\n";
    // (unlocked automatically on destruction of lck1 and lck2)
}

void task_b () {
    // foo.lock(); bar.lock(); // replaced by:
    std::unique_lock<std::mutex> lck1, lck2;
    // defer_lock 构造 unique_lock 但是先不进行加锁
    lck1 = std::unique_lock<std::mutex>(bar,std::defer_lock);
    lck2 = std::unique_lock<std::mutex>(foo,std::defer_lock);
    std::lock (lck1,lck2);       // simultaneous lock (prevents deadlock)
    std::cout << "task b\n";
    // (unlocked automatically on destruction of lck1 and lck2)
}


class MoveData{
public:
    int data{};
    std::string name{};
    std::string address{};
    void show() const {
        std::cout << data << name << address << std::endl;
    }
};

void move_test1(MoveData &data) {

    MoveData moveData;
    moveData.data = 9;
    moveData.name = "sting";
    moveData.address = "127.0.0.1";
    data = std::move(moveData);
}

void move_test(MoveData &data) {
    move_test1(data);
}


int main(int argc, char *argv[]) {

    std::thread th1 (task_a);
    std::thread th2 (task_b);
    th1.join();
    th2.join();

    MoveData pData;
    move_test(pData);
    pData.show();



    return 0;
}




