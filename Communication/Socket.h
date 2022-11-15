#ifndef TCP_SOCKET_H__
#define TCP_SOCKET_H__

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

class TcpSocket {
private:
    /* 私有成员变量 */
    int m_fd;  // 通信套接字
private:
    int readn();
public:
    TcpSocket();  // 默认构造函数，创建套接字
    ~TcpSocket();  // 关闭连接

    /* 接口 */
    int sendMsg(std::string);  // 发送信息
    std::string recvMsg();  // 接收信息
    void closeTcpSocket();  // 关闭套接字

};

TcpSocket::TcpSocket() 
    : m_fd(socket(AF_INET, SOCK_STREAM, 0)) 
{ }

TcpSocket::~TcpSocket() {
    this->closeTcpSocket();
}

void TcpSocket::closeTcpSocket() {
    // 如果连接没有关闭，断开连接
    if (this->m_fd > 0) {
        close(this->m_fd);
        std::cout << "套接字已关闭" << std::endl;
    }
}

#endif  // !TCP_SOCKET_H__