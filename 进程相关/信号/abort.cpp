#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

// abort
int main() {
    int i = 0;
    while (1) {
        cout << "do something here" << endl;
        if (4 == i) {
            abort();
        }
        i++;
        sleep(1);
    }
    return 0;
}