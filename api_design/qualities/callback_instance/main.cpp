    //
    // Created by andrew on 2021/9/8.
    //
    #include <iostream>
    #include "modulea.h"
    #include "moduleb.h"

    using namespace std;


    /*
     * 另外一种方法可以使用bind将对象和函数绑定之后 function
     * */

    int main(int argc, char* argv[]) {

        ModuleB moduleB;
        ModuleA moduleA;

        int closure = 32;

        // add a callback fot ModuleA to MOduleB
        moduleB.SetCallback(&moduleA, ModuleA::StaticWrapper, &closure);

        // tell ModuleB to invoke all its callbacks
        moduleB.DoAction();

        return 0;
    }
