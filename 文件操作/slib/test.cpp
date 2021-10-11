#include <iostream>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"

using namespace std;

int main() {
    int x = 10;
    int y = 5;
    cout << "x + y = " << add(x, y) << endl;
    cout << "x - y = " << sub(x, y) << endl;
    cout << "x * y = " << mul(x, y) << endl;
    cout << "x / y = " << div1(x, y) << endl;
    cout << "static lib test" << endl;
    
    return 0;
}