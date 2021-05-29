//
// Created by andrew on 2021/5/29.
//
#include <iostream>


using namespace std;


int main(int argc, char* argv[]) {

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




    return 0;
}
