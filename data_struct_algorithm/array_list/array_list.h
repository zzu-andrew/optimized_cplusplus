//
// Created by andrew on 2021/9/25.
//

#ifndef OPTIMIZED_CPLUSPLUS_ARRAY_LIST_H
#define OPTIMIZED_CPLUSPLUS_ARRAY_LIST_H
#include "linear_list.h"
#include <iostream>

/*
 *   // 需要实现的内容，先粘贴到这里
 *
    // return element whose index is theIndex
    virtual int indexOf(const T &theElement) const = 0;

    // return index of first occurence of theElement
    virtual void erase(int theIndex) = 0;

    // remove the element whose index is theIndex
    virtual void insert(int theIndex, const T &theElement) = 0;

    // insert theElement so that its index is theIndex
    virtual void output(ostream &out) const = 0;
 *
 *
 * */


template<typename T>
class ArrayList : public LinearList<T> {
public:
    // 构造函数 析枸函数  赋值构造函数
    explicit ArrayList(int32_t initialCapacity = 10);
    ArrayList(const ArrayList<T>& arrayList);
    ~ArrayList() {
        // nullptr 也可进行delete 不会出问题
        delete [] m_element;
    }

    // ADT methods
    bool Empty() const override {
        // 只有list为空的返回true
        return 0 == m_listSize;
    }

    int Size() const override {
        return m_listSize;
    }

    T& Get(int index) const override;

    int indexOf(const T &theElement) const override;

protected:
    void CheckIndex(int32_t index);
    T* m_element; // 指向一个数组，数组里面存储列表的内容
    int32_t m_arrayLength{}; // 数组的长度
    int32_t m_listSize{}; // 列表的大小
};




#endif //OPTIMIZED_CPLUSPLUS_ARRAY_LIST_H
