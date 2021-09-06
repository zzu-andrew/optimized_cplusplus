//
// Created by andrew on 2021/9/6.
//

#ifndef OPTIMIZED_CPLUSPLUS_MYOBJECT_H
#define OPTIMIZED_CPLUSPLUS_MYOBJECT_H


#include <boost/shared_ptr.hpp>

// a reference counter pointer to MyObject
typedef boost::shared_ptr<class MyObject> MyObjectPtr;

class MyObject {
public:
    // instance reference counter of MyObject
    static MyObjectPtr CreateInstance(int id);
    ~MyObject();

private:
    explicit MyObject(int id);
    int m_id;
};

#endif //OPTIMIZED_CPLUSPLUS_MYOBJECT_H
