//
// Created by andrew on 2021/9/8.
//

#include "moduleb.h"


void ModuleB::SetCallback(void *obj, CallbackType cb, void *data) {
    mObject = obj;
    mCallback = cb;
    mClosure = data;
}

void ModuleB::DoAction() {
    if (mCallback) {
        (*mCallback)(mObject, "Hello World!", mClosure);
    }
}

ModuleB::ModuleB() : mObject(nullptr), mCallback(nullptr), mClosure(nullptr) {

}
