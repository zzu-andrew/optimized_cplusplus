//
// Created by andrew on 2021/9/7.
//

#ifndef OPTIMIZED_CPLUSPLUS_MODULEA_H
#define OPTIMIZED_CPLUSPLUS_MODULEA_H

#include <iostream>
#include <string>

/*
 * a callback class with a static member function
 * */
class ModuleA {
public:
    // The static function callback
    static void StaticMethod(const std::string &name, void *data);
};


#endif //OPTIMIZED_CPLUSPLUS_MODULEA_H
