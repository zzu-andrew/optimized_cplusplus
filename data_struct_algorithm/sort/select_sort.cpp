//
// Created by andrew on 2021/9/20.
//
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <exception>
#include <iterator>
#include <ostream>

using namespace std;




template<typename T>
int indexOfMax(T a[], int n)
{
    // Locate the largest element in a[0:n-1].
    if(n <= 0)
        exit(-1);
    int indexOfMax = 0;
    for(int i = 1; i < n; i++)
    {
        if(a[indexOfMax] < a[i])
            indexOfMax = i;
    }

    return indexOfMax;
}

template<typename T>
void SelectionSort(T a[], int32_t n) {
    // 对数组[0 : n-1]个元素进行排序
    for (int32_t size = n; size > 1; size --) {
        // 找出最大值的坐标
        int32_t j = indexOfMax(a, size);
        // 将最大值放到目前size允许的最后
        swap(a[j], a[size - 1]);
    }

}


int main(int argc, char *argv[]) {

    int a[10] = {3, 2, 4, 1, 6, 9, 8, 7, 5, 0};

    // output the elements
    cout << "a[0:9] = ";
    copy(a, a+10, ostream_iterator<int>(cout, " "));
    cout << endl;

    // sort
    SelectionSort(a,10);

    // output the sorted sequence
    cout << "After the sort, a[0:9] = ";
    copy(a, a+10, ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}
