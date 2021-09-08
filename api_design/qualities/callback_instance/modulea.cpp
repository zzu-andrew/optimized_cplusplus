//
// Created by andrew on 2021/9/8.
//

#include <iostream>
#include "modulea.h"


void ModuleA::StaticWrapper(void *obj, const std::string &name, void *data) {
    // C++11的类型强制转换
    auto *self = static_cast<ModuleA *>(obj);
    self->InstanceMethod(name, data);
}

// 因为没有使用任何的类成员变量，所以Clion会提示定义成static 不用管
void ModuleA::InstanceMethod(const std::string &name, void *data) {
    int closure = *static_cast<int *>(data);
    std::cout << "In instance callback: '" << name << "' (closure = " << closure << ")" << std::endl;
}
