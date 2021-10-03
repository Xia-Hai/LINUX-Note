#include <iostream>
#include <string>
#include <ctime>
#include <string.h>


using namespace std;

const int SIZE = 32;
int main() {
    time_t t = -1;
    struct tm *pT = NULL;
    char file_name[SIZE];
    // 获取时间 秒
    t = time(NULL);
    if (t == -1) {
        perror("time");
        exit(1);
    }
    cout << "t: " << t << endl;
    pT = localtime(&t);
    if (pT == nullptr) {
        cout << "localtime failed..." << endl;
        exit(1);
    }
    cout << "year: " << pT->tm_year + 1900 << endl;
    cout << "month: " << pT->tm_mon + 1 << endl;
    cout << "day: " << pT->tm_mday << endl;
    cout << "hour: " << pT->tm_hour << endl;
    cout << "min: " << pT->tm_min << endl;
    cout << "sec: " << pT->tm_sec << endl;

    memset(file_name, 0, SIZE);
    sprintf(file_name, "%s %d%d%d%d%d%d.log", "touch", pT->tm_year + 1900, pT->tm_mon + 1, pT->tm_mday,
                pT->tm_hour, pT->tm_min, pT->tm_sec);
    cout << "file_name: " << file_name << endl;

    return 0;
}