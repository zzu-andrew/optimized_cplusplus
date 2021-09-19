//
// Created by andrew on 2021/9/8.
//
#include <iostream>


/*
 * 1. C++中如何使用基类指针，调用只有在子类中才存在的成员函数
 * 2. 还能通过这种方式，绕过C++的保护机制，直接在类的外部调用私有成员函数
 * */

class Widget {
public:
    void Locate() const {
        std::cout << "widget locate" << m_widget << std::endl;
    }

private:
    int m_widget{};
};

// 放到Widget的域里面
typedef void (Widget::*LOCATE_FUNC)();

// is-a
class Derive : public Widget {
public:
    void ShowDerive() const {
        std::cout << "Derive show" << derive << std::endl;
    }

    LOCATE_FUNC GetLocateFunction() {
        // C++中获取函数指针的时候需要，使用取地址符
        return (LOCATE_FUNC)&Derive::DeriveLocate;
    }

private:
    void DeriveLocate() const {
        std::cout << "Derive locate  " << derive << std::endl;
    }

    int32_t derive = 3;
};


int main(int argc, char* argv[]) {

    // 定义一个子类的对象
    Derive derive;
    LOCATE_FUNC lpLocateFunc = derive.GetLocateFunction();

    // 1. 我们使用一个子类对象， 去调用子类对象的一个成员函数
    // C++成员函数是和对象绑定的
    (derive.*lpLocateFunc)();

    // 定义一个 Widget类型的一整指针，这个指针指向derive
    Widget *lpWidget = &derive;
    // 通过lpWidget这个指针调用DeriveLocate成员函数
    // 绕过了C++保护机制
    (lpWidget->*lpLocateFunc)();

    return 0;
}

