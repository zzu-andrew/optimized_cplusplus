//
// Created by andrew on 2021/9/8.
//

#ifndef OPTIMIZED_CPLUSPLUS_MODULEB_H
#define OPTIMIZED_CPLUSPLUS_MODULEB_H

#include <string>
// define a class call ModuleA class
class ModuleB {
public:
    // a type for the function signature that is accepted as a callback
    typedef void (*CallbackType)(void *obj, const std::string &name, void *data);

    ModuleB();

    // set the callback function. only one callback can br register
    void SetCallback(void *obj, CallbackType cb, void *data);

    // call the callback function
    void DoAction();

private:
    void *mObject{};
    CallbackType mCallback{};
    void *mClosure{};
};


#endif //OPTIMIZED_CPLUSPLUS_MODULEB_H
