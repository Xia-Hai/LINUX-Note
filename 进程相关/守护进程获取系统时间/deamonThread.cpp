#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <sys/stat.h>

using namespace std;
// 创建守护进程

const int SIZE = 128;
int main()
{
    pid_t pid = -1;
    int ret = -1;
    time_t t = -1;
    struct tm *pT = nullptr;
    char file_name[SIZE];
    // 1. 创建子进程，父进程退出
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid > 0)
    {
        exit(0); // 父进程退出
    }
    // 2. 创建新的会话

    pid = setsid(); // 子进程脱离控制终端
    if (pid == -1)
    {
        perror("setsid");
        exit(1);
    }
    // 3. 设置当前工作目录

    ret = chdir("/"); // 改变到根目录,防止可以卸载的文件系统
    if (ret == -1)
    {
        perror("chdir");
        exit(1);
    }
    // 4. 设置权限掩码

    umask(0); // 没有屏蔽任何权限
    // 5. 关闭文件描述符

    close(STDIN_FILENO); // fd 0 1 2
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    // 6. 执行核心任务
    // 每隔一秒钟输出当前时间到 /tmp/txt.log 文件中

    while (1)
    {
        t = time(NULL);
        pT = localtime(&t);
        if (pT == nullptr)
        {
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
        sprintf(file_name, "%s%d%d%d%d%d%d.log", "touch /home/xiahai/log", pT->tm_year + 1900, pT->tm_mon + 1, pT->tm_mday,
                pT->tm_hour, pT->tm_min, pT->tm_sec);
        //cout << "file_name: " << file_name << endl;
        system(file_name);
        sleep(5);
    }

    return 0;
}