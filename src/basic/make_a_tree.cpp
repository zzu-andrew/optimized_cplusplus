//
// Created by andrew on 2021/5/29.
//
#include <iostream>
#include <list>
#include <future>
#include <mutex>
#include <thread>

#include <shared_mutex>

using namespace std;





void packaged_task() {

    std::async()


}





int main(int argc, char* argv[]) {

    // 看着图就可以手上写一个二叉树
    struct treenode {
        char const* name;
        treenode* left;
        treenode* right;
    } tree[] = {
            { "D", &tree[1], &tree[2] },
            { "B", &tree[3], &tree[4] },
            { "F", &tree[5], nullptr },
            { "A", nullptr, nullptr },
            { "C", nullptr, nullptr },
            { "E", nullptr, nullptr },
    };

    cout << tree->name << tree->left->name << endl;

    std::string name;
    name.reserve(200);
    name.shrink_to_fit();




    return 0;
}
