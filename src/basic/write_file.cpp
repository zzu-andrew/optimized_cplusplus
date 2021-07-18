//
// Created by andrew on 2021/7/7.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


struct timespec timeSnap[30000];


const unsigned long second2nsec = 1000000000;

int main(int argc, char* argv[]) {

    ofstream outfile("./ou.txt", ios::trunc);

    for (auto & i : timeSnap) {
        clock_gettime(CLOCK_MONOTONIC, &i);
    }

    for (auto & t : timeSnap) {

        outfile << t.tv_sec * second2nsec + t.tv_nsec;
        outfile << endl;
    }


    outfile.close();
    return 0;
}


