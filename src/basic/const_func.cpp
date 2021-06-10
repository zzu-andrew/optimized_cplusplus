//
// Created by andrew on 2021/5/23.
//
#include <iostream>
#include <array>

using namespace std;
/*
 * 1. const成员函数不能修改成员变量
 * 2. const对象不能引用非const成员函数
 * */

class WatchData {
public:
    WatchData() = default;
    ~WatchData() = default;

    virtual int GetData() const  {
        return m_index;
    }
    // 函数后面的const是修饰所有的成员变量在该函数中为const
    /*void SetData(int &&idx) const {  // error: assignment of member ‘WatchData::m_index’ in read-only object
        m_index = idx;
    }*/

    virtual void SetData(int &&idx) {
        m_index = idx;
    }

private:
    int m_index{};
};

class StandData : public WatchData {
public:
    StandData() = default;
    ~StandData() = default;
    int GetData() const override {
        return m_index;
    }
    // 函数后面的const是修饰所有的成员变量在该函数中为const
    /*void SetData(int &&idx) const {  // error: assignment of member ‘WatchData::m_index’ in read-only object
        m_index = idx;
    }*/

    void SetData(int &&idx) override {
        m_index = idx;
    }

private:
    int m_index{};
};


void ArrayTest() {
    std::array<char, 100> address{};

    int i = 0;
    for (auto &it : address) {
        it = i;
        i ++;
    }

    std::array<char, 100> address2{};
    std::swap(address,address2);

}


int main(int argc, const char** argv) {

    WatchData watchData{};
    watchData.SetData(6);
    cout << watchData.GetData() << endl;

    // const 对象不能引用非const的成员函数
//    const WatchData *pWatchData = &watchData;
    //  pWatchData->SetData(4);  error: passing ‘const WatchData’ as ‘this’ argument discards qualifiers [-fpermissive]

    StandData standData{};
    standData.WatchData::SetData(43);
    cout << standData.WatchData::GetData() << endl;

    ArrayTest();

    cout << "test" << endl;
    return 0;
}
