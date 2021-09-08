//
// Created by andrew on 2021/9/7.
//

#include "modulea.h"

void ModuleA::StaticMethod(const std::string &name, void *data) {
    int closure = *static_cast<int *>(data);
    std::cout << "In static callback: '" << name << "', closure = " << closure << std::endl;
}
