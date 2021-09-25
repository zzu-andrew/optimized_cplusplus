//
// Created by andrew on 2021/9/25.
//
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <ostream>

using namespace std;

template<typename T>
void Bubble(T a[], int n) {
    for (int i = 0; i < n - 1; i ++) {
        if (a[i] > a[i + 1]) swap(a[i], a[i + 1]);
    }
}
template<typename T>
void BubbleSort(T a[], int n) {
    for (int i = n; i > 1; i --) {
        Bubble(a, i);
    }
}

int main(int argc, char *argv[]) {

    int a[10] = {10,7,8,9,4, 2, 3, 6, 5,1};

    // output the elements
    cout << "a[0:9] = ";
    copy(a, a+10, ostream_iterator<int>(cout, " "));
    cout << endl;

    // sort
    BubbleSort(a,10);

    // output the sorted sequence
    cout << "After the sort, a[0:9] = ";
    copy(a, a+10, ostream_iterator<int>(cout, " "));
    cout << endl;
    return 0;
}
