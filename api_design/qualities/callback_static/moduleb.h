//
// Created by andrew on 2021/9/7.
//

#ifndef OPTIMIZED_CPLUSPLUS_MODULEB_H
#define OPTIMIZED_CPLUSPLUS_MODULEB_H
#include <iostream>

/*
 * moduleB 调用moduleA
 * */

class ModuleB {
public:
    // define function type
    typedef void (*CallbackType)(const std::string &name, void *data);

    ModuleB();

    //    set callback function, only one callback function can be registered
    void SetCallback(CallbackType cb, void *data);
    // call callback function
    void DoAction();


private:
    CallbackType m_callback;
    void *m_closure;
};


#endif //OPTIMIZED_CPLUSPLUS_MODULEB_H
