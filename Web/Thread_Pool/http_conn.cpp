#include "http_conn.h"

int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;
// 设置文件描述符为非阻塞，为了一次性读出全部数据
void setnonblocking(int fd) {
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);    
}

// 添加文件描述符到 epoll 中
void addfd(int epollfd, int fd, bool one_shot) {
    epoll_event event;
    event.data.fd = fd;
    //event.events = EPOLLIN | EPOLLRDHUP;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

    if (one_shot) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

// 从 epoll 中删除
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 修改epoll中的文件描述符
void modfd(int epollfd, int fd, int ev) {

    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}


void http_conn::init(int sockfd, const sockaddr_in &addr) {
    m_sockfd = sockfd;
    m_address = addr;
    // 设置端口复用
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    // 添加到epoll对象中
    addfd(m_epollfd, m_sockfd, true);
    m_user_count++;
    init();
}

void http_conn::init() {
    m_check_state = CHECK_STATE_REQUESTLINE; // 初始化状态为解析请求首行
    m_checked_index = 0;
    m_start_line = 0;
    m_read_index = 0;
    m_url = 0;
    m_version = 0;
    m_method = GET;
    bzero(m_read_buf, READ_BUFFER_SIZE);
    m_host = 0;
    m_linger = false;
}

void http_conn::close_conn() {
    if (m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--; // 关闭一个连接，客户数量减 1
    }
}

bool http_conn::read() {
    // std::cout << "一次性读完数据" << std::endl;
    if (m_read_index >= READ_BUFFER_SIZE) {
        return false;
    }
    // 读取到的字节
    int bytes_read = 0;
    while (true) {
        bytes_read = recv(m_sockfd, m_read_buf + m_read_index, READ_BUFFER_SIZE - m_read_index, 0);
        if (bytes_read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 没有数据可读
                break;
            }
            return false;
        }
        else if (bytes_read == 0) {
            // 对方关闭连接
            return false;
        }
        m_read_index += bytes_read;
    }
    std::cout << "读取到了数据 " << m_read_buf << std::endl;

    return true;
}

bool http_conn::write() {
    std::cout << "一次性写完数据" << std::endl;
    
    return true;
}

http_conn::HTTP_CODE http_conn::do_request() {


}

// 主状态机，解析请求
http_conn::HTTP_CODE http_conn::process_read() {
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;

    char *text = nullptr;
    while ((m_check_state == CHECK_STATE_REQUESTLINE && line_status == LINE_OK) || (line_status = parse_line()) == LINE_OK) {
        // 解析到了一行完整的数据
        text = get_line();
        m_start_line = m_checked_index;
        std::cout << "got 1 http line : " << text << std::endl;

        switch (m_check_state)
        {
        case CHECK_STATE_REQUESTLINE:
            {
                ret = parse_request_line(text);
                if (ret == BAD_REQUEST) {
                    return BAD_REQUEST;
                }
            }
            break;
        case CHECK_STATE_HEADER:
            {
                ret = parse_request_header(text);
                if (ret == BAD_REQUEST) {
                    return BAD_REQUEST;
                }
                else if (ret == GET_REQUEST) {
                    return do_request();
                }
            }
            break;
        case CHECK_STATE_CONTENT:
            {
                ret = parse_request_content(text);
                if (ret == GET_REQUEST) {
                    return do_request();
                }
                line_status = LINE_OPEN;
            }
            break;
        default:
            {
                return INTERNAL_ERROR;
            }
            break;
        }
        
    }
    return NO_REQUEST;
} // 解析HTTP请求

http_conn::HTTP_CODE http_conn::parse_request_line(char *text) {

    m_url = strpbrk(text, " \t");
    *m_url++ = '\0';
    char *method = text;
    if (strcasecmp(method, "GET") == 0) {
        m_method = GET;
    }
    else {
        return BAD_REQUEST;
    }
    m_version = strpbrk(m_url, " \t");
    if (!m_version) {
        return BAD_REQUEST;
    }
    *m_version++ = '\0';
    if (strcasecmp(m_version, "HTTP/1.1") != 0) {
        return BAD_REQUEST;
    }
    if (strncasecmp(m_url, "http://", 7) == 0) {
        m_url += 7;
        m_url = strchr(m_url, '/');
    }
    if (!m_url || m_url[0] != '/') {
        return BAD_REQUEST;
    }
    m_check_state = CHECK_STATE_HEADER; // 改变主状态机的状态
    return NO_REQUEST;

} // 解析请求首行

http_conn::HTTP_CODE http_conn::parse_request_header(char *text) {
    
} // 解析头部

http_conn::HTTP_CODE http_conn::parse_request_content(char *text) {

} // 

http_conn::LINE_STATUS http_conn::parse_line() {
        
    char temp;
    for ( ; m_checked_index < m_read_index; ++m_checked_index) {
        temp = m_read_buf[m_checked_index];
        if (temp == '\r') {
            if ((m_checked_index + 1) == m_read_index) {
                return LINE_OPEN;
            }
            else if (m_read_buf[m_checked_index + 1] == '\n') {
                m_read_buf[m_checked_index++] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if (temp == '\n') {
            if (m_checked_index > 1 && m_read_buf[m_checked_index - 1] == '\r') {
                m_read_buf[m_checked_index - 1] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }    
    }
    return LINE_OPEN;

} // 解析行


void http_conn::process() {
    // 解析HTTP请求  生成响应
    HTTP_CODE read_ret = process_read();
    if (read_ret == NO_REQUEST) {
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }

    std::cout << "parse request, create response" << std::endl;


}