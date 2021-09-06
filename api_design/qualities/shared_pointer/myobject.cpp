//
// Created by andrew on 2021/9/6.
//
#include <iostream>
#include "myobject.h"

MyObject::~MyObject() {
    std::cout << "........ destroyed object " << m_id << std::endl;
}

MyObjectPtr MyObject::CreateInstance(int id) {
    return MyObjectPtr(new MyObject(id));
}

MyObject::MyObject(int id) : m_id(id) {
    std::cout << "...... create object " << m_id << std::endl;
}

