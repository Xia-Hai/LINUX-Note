#ifndef HTTPCONN_H
#define HTTPCONN_H

#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>


class http_conn {
public:
    static int m_epollfd; // 所有的socket上的事件都被注册到同一个epoll上
    static int m_user_count; // 统计用户的数量
    static const int READ_BUFFER_SIZE = 2048; // 读缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024; // 写缓冲区的大小

    // HTTP请求方法，这里只支持GET
    enum METHOD {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT};

    // 解析客户端请求时，主状态机的状态
    enum CHECK_STATE {CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT};

    // 从状态机的三种状态，也就是行的读取状态
    enum LINE_STATUS {LINE_OK = 0, LINE_BAD, LINE_OPEN};

    // 服务器处理HTTP请求的可能结果，报文解析的结果
    enum HTTP_CODE {NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, 
                FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION};
    

    http_conn() {};
    ~http_conn() {};

    void process(); // 处理客户端的请求

    void init(int sockfd, const sockaddr_in &addr); // 初始化新接收的连接
    void close_conn(); // 关闭连接

    bool read(); // 非阻塞读
    bool write(); // 非阻塞写

    
    

private:
    int m_sockfd; // 该HTTP连接的socket
    sockaddr_in m_address; // 通信socket地址 
    char m_read_buf[READ_BUFFER_SIZE]; // 读缓冲区；
    int m_read_index; // 标识读缓冲区中已经读入客户端数据的最后一个字节的下一个位置

    int m_checked_index; // 当前分析的字符在读缓冲区的位置
    int m_start_line; // 当前正在解析的行的起始位置
    
    char *m_url; // 目标文件名
    char *m_version; // 协议版本
    METHOD m_method; // 请求方法
    char *m_host; // 主机名
    bool m_linger; // HTTP是否保持连接

    CHECK_STATE m_check_state; // 主状态机当前的状态
    void init(); // 初始化连接其余的信息

    HTTP_CODE process_read(); // 解析HTTP请求
    HTTP_CODE parse_request_line(char *text); // 解析请求首行
    HTTP_CODE parse_request_header(char *text); // 解析头部
    HTTP_CODE parse_request_content(char *text); // 

    LINE_STATUS parse_line(); // 解析行

    char *get_line() { return m_read_buf + m_start_line;}
    HTTP_CODE do_request();
};



#endif
/* HTTPCONN_H */