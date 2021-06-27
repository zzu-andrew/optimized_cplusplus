//
// Created by andrew on 2021/6/13.
//
#include <iostream>

class WidgetOperator {
public:
    WidgetOperator() = default;;
    ~WidgetOperator() = default;;
    WidgetOperator(const WidgetOperator & wo) {
        std::cout << "call WidgetOperator ctor" << std::endl;
    }

    WidgetOperator& operator=(const WidgetOperator& wo) {
        std::cout << "call WidgetOperator operator= " << std::endl;
        return *this;
    }
};

void WidgetOperatorTest() {
    std::cout << "W1" << std::endl;
    WidgetOperator W1;  // 调用无参构造函数
    std::cout << "W2(W1)" << std::endl;
    WidgetOperator W2(W1); // 调用copy构造函数
    std::cout << "W1 = W2" << std::endl;
    W1 = W2;  // 调用 operator=函数
    std::cout << "WidgetOperator W3 = W1" << std::endl;
    WidgetOperator W3 = W1; // 调用copy构造函数
}

int main(int argc, char* argv[]) {

    WidgetOperatorTest();



    return 0;
}

