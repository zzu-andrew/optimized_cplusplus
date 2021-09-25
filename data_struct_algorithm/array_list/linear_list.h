//
// Created by andrew on 2021/9/25.
//

#ifndef OPTIMIZED_CPLUSPLUS_LINEAR_LIST_H
#define OPTIMIZED_CPLUSPLUS_LINEAR_LIST_H

#include <iostream>
#include <string>

using namespace std;

// 定义一个抽象类，用于指定实现一个arrayList都需要实现哪些函数接口

template<class T>
class LinearList {
public:
    virtual ~LinearList() = default;;

    virtual bool Empty() const = 0;

    // return true iff list is empty
    virtual int Size() const = 0;

    // return number of elements in list
    virtual T &Get(int theIndex) const = 0;

    // return element whose index is theIndex
    virtual int IndexOf(const T &theElement) const = 0;

    // return index of first occurence of theElement
    virtual void Erase(int theIndex) = 0;

    // remove the element whose index is theIndex
    virtual void Insert(int theIndex, const T &theElement) = 0;

    // insert theElement so that its index is theIndex
    virtual void Output(ostream &out) const = 0;

};

struct MyException : public exception
{
public:
    explicit MyException(std::string str) {
        m_exception = std::move(str);
    }

    const char * what () const noexcept override
    {
        return m_exception.c_str();
    }

private:
    std::string m_exception;
};

#endif //OPTIMIZED_CPLUSPLUS_LINEAR_LIST_H
