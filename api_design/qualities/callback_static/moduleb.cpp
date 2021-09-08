//
// Created by andrew on 2021/9/7.
//

#include "moduleb.h"

ModuleB::ModuleB() : m_callback(nullptr), m_closure(nullptr) {

}

void ModuleB::SetCallback(ModuleB::CallbackType cb, void *data) {
    m_callback = cb;
    m_closure = data;
}

void ModuleB::DoAction() {
    if (m_callback) {
        (*m_callback)("Hello world!", m_closure);
    }
}
