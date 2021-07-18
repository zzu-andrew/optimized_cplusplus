//
// Created by andrew on 2021/7/4.
//
#include <iostream>


class Shape {
public:
    virtual ~Shape() = default;
    enum ShapeColor {Red, Green, Blue};
    virtual void draw(ShapeColor color = Red) const = 0;
};

class Rectangle : public Shape{
public:
    virtual void draw(ShapeColor color = Green) const;
};

void Rectangle::draw(ShapeColor color) const {

    if (Red == color) {
        std::cout << "Red" << std::endl;
    } else if (Green == color) {
        std::cout << "Green" << std::endl;
    } else if (Blue == color) {
        std::cout << "Blue" << std::endl;
    } else {
        std::cout << "invalid color" << std::endl;
    }
}




int main(int argc, char* argv[]) {
    // 直接定义子类对象
    Rectangle rectangle;
    rectangle.draw();
    // 通过父类指针指向子类对象
    Shape * pShape = new Rectangle;
    pShape->draw();

    delete pShape;
    return 0;
}

