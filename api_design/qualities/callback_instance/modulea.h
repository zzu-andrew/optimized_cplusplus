//
// Created by andrew on 2021/9/8.
//

#ifndef OPTIMIZED_CPLUSPLUS_MODULEA_H
#define OPTIMIZED_CPLUSPLUS_MODULEA_H

#include <string>


/*
 * 回调成员函数
 * */
class ModuleA {
public:
    // a static function that lets you call a member function
    static void StaticWrapper(void *obj, const std::string &name, void *data);

    /// the member function callback
    void InstanceMethod(const std::string &name, void *data);

};


#endif //OPTIMIZED_CPLUSPLUS_MODULEA_H
