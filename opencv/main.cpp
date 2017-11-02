
#include <iostream>
#include <string>
#include <vector>

#include "test.hpp"

using namespace std;

int main(int argc, char ** argv)
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " {test_number} {argv}\n";
        return 1;
    }

    vector<string> arg_vec;
    for (int i = 0; i < argc; ++i) {
        arg_vec.push_back(argv[i]);
    }

    int test_number = 0;
    try {
        test_number = stoi(arg_vec[1]);
    } catch (...) {
        cerr << "Test number exception.\n";
        return 1;
    }

    cout << "Call test number: " << test_number << endl;

    switch (test_number) {
    case  1: return test01(arg_vec);
    case  2: return test02(arg_vec);
    case  3: return test03(arg_vec);
    default:
        cout << "Undefined test number.\n";
    }

    return 0;
}

