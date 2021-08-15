//
// Created by andrew on 2021/7/11.
//
#include <iostream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <map>
#include <random>


/*
 *  因为继承了binary_function 实现之后默认就有了
 *  template<typename _Tp>
    struct less : public binary_function<_Tp, _Tp, bool>
    {
      _GLIBCXX14_CONSTEXPR
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x < __y; }
    };
    template<typename _Arg1, typename _Arg2, typename _Result>
    struct binary_function
    {
      /// @c first_argument_type is the type of the first argument
      typedef _Arg1 	first_argument_type;

      /// @c second_argument_type is the type of the second argument
      typedef _Arg2 	second_argument_type;

      /// @c result_type is the return type
      typedef _Result 	result_type;
    };
 *
 * */


void sort_demo() {

    int array[] = {1,2,3,4,5,9,7,8};

    std::sort(array, array + (sizeof(array))/sizeof(int), std::less<int>());


    std::sort(array, array + 5, std::greater<int>());

    for (auto &it:array) {
        std::cout << std::setw(4)  << it;
    }
    std::cout << std::endl;
}


void vector_demo() {
    std::vector<int> myVec;

    myVec.reserve(100);
    for (auto i = 0; i < 100; i ++) {
        myVec.push_back(i);
    }
  

}


// stack

//queue


//map

//multimap

// unordered_map

// set
//multiset
//unordered_set


class MapOp : public std::binary_function<std::pair<int, int>, std::pair<int, int>, bool>{
public:
    bool operator() (std::map<int, int>::iterator pair, int level) {

        return pair->second < level;
    }
};


// adapter
void adapter_demo() {

    std::random_device rd;
    std::mt19937 mt(rd());

    std::map<int, int> myMap;
    for (int i = 0;  i < 1000; ++ i) {
        myMap.insert(std::make_pair(i, mt()));
    }

    for (auto &it:myMap) {
        std::cout << std::setw(5) << it.first << std::setw(13)  << it.second << std::endl;
    }


    auto count= myMap.count(900185717);
    std::cout << count << std::endl;

//    int dataCount = std::count_if(myMap.begin(), myMap.end(), std::bind(MapOp(), 3000, std::placeholders::_1));

}


int main(int argc, char* argv[]) {


    sort_demo();
    vector_demo();
    adapter_demo();


    return 0;
}
