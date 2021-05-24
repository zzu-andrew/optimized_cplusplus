#include <iostream>
#include <mutex>
#include <string>

using namespace std;


class testString
{
private:

public:
    testString() = default;
    ~testString() = default;

    static void SetString(const string& str)
    {
        string bigStr;
        bigStr.append(str);
        cout << bigStr << endl;

    }

};


int main(int argc, const char** argv) {

    testString::SetString("using append");

    cout << "test" << endl;
    return 0;
}
