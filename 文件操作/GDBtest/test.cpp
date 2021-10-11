#include <iostream>
using namespace std;

void fun() {
    for (int i = 0; i < 10; i++) {
        cout << i << endl;
    }
}

int main(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < 20; i++) {
        cout << i << ' ';
    }
    cout << endl;
    fun();
    cout << "GDBtest" << endl;
    return 0;
}