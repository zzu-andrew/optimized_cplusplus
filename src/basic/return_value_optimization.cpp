//
// Created by andrew on 2021/7/17.
//

#include <iostream>

using namespace std;

class X {

public:
    X() {
        cout << "Construct X " << endl;
    }

    ~X() {
        cout << "Destruct X" << endl;
    }
};

X ReturnValueOptimization() {
    X x;

    return x;
}

int main(int argc, char* argv[]) {

    X retValue = ReturnValueOptimization();

    return 0;
}


