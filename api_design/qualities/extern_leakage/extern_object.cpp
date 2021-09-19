//
// Created by andrew on 2021/9/19.
//

#include "extern_object.h"


using namespace std;

//namespace {
    // an integer constant with external linkage
    int INTERNAL_CONSTANT = 42;

// a string constant with external linkage
    std::string Filename = "file.txt";

// a non-static function with external linkage
    void FreeFunction()
    {
        cout << "Free function called" << endl;
    }
//}



// API
int SetFilename(std::string s) {
    Filename = std::move(s);


    return 0;
}