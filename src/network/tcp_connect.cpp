#include "tcp_connect.h"
#include<iostream>
#include<sys/socket.h>
#include<cstring>

TcpSocket::TcpSocket()
{
    address.sin_port = 80;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    
}

TcpSocket::TcpSocket(std::string ip, int port)
{
    memset(&address, 0, sizeof(address)); // 清零结构体

    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    // if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
    //     std::cout<<"ip transfer error"<<std::endl;
    // }
}

bool TcpSocket::connect()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if (listen(server_fd, 3) < 0) {
        // 处理错误：监听失败
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    std::cout << buffer << std::endl;
    send(new_socket, "Hello from server", 17, 0);
    return true;
}
