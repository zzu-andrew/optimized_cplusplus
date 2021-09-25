//
// Created by andrew on 2021/9/25.
//

#include "array_list.h"





template<typename T>
ArrayList<T>::ArrayList(int32_t initialCapacity) {
    //  构造函数
    if (initialCapacity < 1) {
        cout << "Init capacity = " << initialCapacity << "Must be > 0" << endl;
        return;
    }
    m_arrayLength = initialCapacity;
    m_element = new T[m_arrayLength];
    m_listSize = 0;
}

template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T> &arrayList) {
    // 复制构造函数
    m_arrayLength = arrayList.m_arrayLength;
    m_listSize = arrayList.m_listSize;
    // 重新申请内存，每个元素的内存都要保证唯一
    m_element = new T[m_arrayLength];
    // 将老元素里面的内容复制到新对象里面
    copy(arrayList.m_element, arrayList.m_element + m_listSize, m_element);
}

template<typename T>
void ArrayList<T>::CheckIndex(int32_t index) {
    if (index < 0 || index >= m_listSize) {
        throw MyException("Check exception failed.");
    }
}

template<typename T>
T &ArrayList<T>::Get(int index) const {
    try {
        CheckIndex(index);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return m_element[index];
}

template<typename T>
int ArrayList<T>::indexOf(const T &theElement) const {
    // 查找元素 find 函数会查找对应的元素，找到之后返回指向对应元素的迭代器，这里是指针，那么穿出来的值减去首地址之后就是元素的索引信息
    int index = static_cast<int32_t>(find(m_element, m_element + m_listSize, theElement) - m_element);

    // check index valid
    if (index == m_listSize) {
        return -1;
    }

    return 0;
}



