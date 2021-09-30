#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
//#include <cstring>

using namespace std;

int main()
{
    pid_t pid = -1;
    //创建一个子进程
    pid = fork(); //此时的pid在子进程中返回0
    if (0 == pid)
    {
        //子进程
        while (1)
        {
            cout << "hello h: " << getpid() << ' ' << getppid() << endl;
            //exit(0);
            sleep(1);
        }
    }
    else if (pid > 0)
    {
        //父进程
        while (1)
        {
            cout << "hello x: " << getpid() << ' ' << pid << endl;
            sleep(1);
        }
    }
    else
    {
        perror("fork");
        return 0;
    }
    return 0;
}