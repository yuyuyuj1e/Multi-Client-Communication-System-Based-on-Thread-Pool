# 一、实现功能
1. 封装了用于 ```TCP``` 通信的 ```C++``` 类
    - ```TcpSocket```: 用于通信的 ```socket```
    - ```TcpServer```: 用于服务器部署，绑定服务器 ```IP``` 及监听端口，等待客户端连接
2. 在监听函数中给定端口后，无需用户操作，```TcpServer``` 内部自动绑定端口，并进行监听；在客户端连接时，返回一个用于通信的套接字类 ```TcpSocket```
3. 多种信息发送方式
    - ```C``` 风格: ```int sendMessage(const char*, size_t)```
    - ```C++``` 风格: ```int sendMessage(std::string)```
4. 统一的信息接收方式，无论对方是用 ```C``` 风格方式发送，还是 ```C++``` 风格方式发送，统一返回 ```string``` 字符串
5. 类内部自动解决 TCP "粘包"问题，用户无需进行相关设置
6. 实时反映双方连接状态

---
# 二、遇到的问题及解决方案
## 问题一: 如何解决 ```TCP``` "粘包"问题
> **问题详述**: ```TCP``` 是面向流的的传输协议，发送端和接收端每次接收/发送数据的间隔/速率可能有所不同。接收端接收的数据可能是由一条数据构成或由多条数据组合而成。如果时多条数据组合而成就没有办法将其分开，这样就出现了"粘包"问题。

> **解决方案**: 在数据前添加包头，包头内容就是本次发送数据的长度，接收端每次先接收包头内容，了解到本次需要接受的数据长度后，再去接收指定长度的数据。

## 问题二: 如何理解大端存储和小端存储
> **问题详述**: 网络字节序都是大端存储(低高高低)的，而主机字节序都是小端存储(低低高高)的。所以每次将数据发送的时候都需要将小端存储的数据转换成大端存储的，而每次接收数据时都需要将小端存储的转换成大端存储的。

> **解决方案**: 
> - 网络字节序转主机字节序：
>   - ```uint16_t ntohs(uint16_t)```
>   - ```uint32_t ntohl(uint32_t);```
>   - ```const char *inet_ntop(int, const void*, char*, socklen_t);```
>   - ```char* inet_ntoa(struct in_addr);  // 只能处理 ipv4```
> - 主机字节序转网络字节序:
>   - ```uint16_t htons(uint16_t);```
>   - ```uint32_t htonl(uint32_t);```
>   - ```int inet_pton(int, const char*, void*); ```
>   - ```in_addr_t inet_addr(const char*);  // 只能处理 ipv4```

## 问题三: ```C``` 语言字符串结束符 ```'\0'```
> **问题详述**: 由于 ```socket``` 函数是通过 ```C``` 风格来发送/接收数据(```char*```)，所以如果不添加字符串结束符就可能会出现乱码的情况。

> **解决方案**: 在接收数据的时候，除了为数据本身申请空间还需要为字符串结束符 ```'\0'``` 申请空间，并将最后一个位置赋值为 字符串结束符 ```'\0'```。

---
# 三、改进
## 1. 没有完美的将通信模块划分开
> **情形描述**: 通信模块只包含发送数据和读取数据，但是本次并没有实现单独的客户端类，而是将客户端类与通信模块融合，也就是通信类中其实也包含了客户端类中的连接服务器的功能。

## 2. 只拥有 ```TCP``` 通信功能
> **情形描述**: ```socket``` 其实包含了其他的通信方法，如进行 ```UDP``` 通信，也就是在创建套接字的时候，使用 ````SOCK_DGRAM```` 指定使用报式传输协议。
