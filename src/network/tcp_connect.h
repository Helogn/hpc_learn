#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class TcpSocket
{
    /*
    为了理解这个类的需求，做如下分析：
    作为一个类需要类的构造函数 TcpSocket
    为了建立连接，需要输入目标主机的ip和端口，从而定义私有变量ip和port
    需要主动建立连接的过程，定义类函数connect
    建立连接后需要存储该连接变量 this.status
    */

    /*
    TCP协议特性：
    Socket基本流程：
    服务器端：socket() → bind() → listen() → accept() → read()/write() → close()
    客户端：socket() → connect() → read()/write() → close()
    struct sockaddr_in {
    short            sin_family;   // AF_INET
    unsigned short   sin_port;     // 端口号(网络字节序)
    struct in_addr   sin_addr;     // IP地址
    char             sin_zero[8]; // 填充
    };
    */
    public:
    TcpSocket();
    TcpSocket(std::string ip, int port);
    bool connect();
    private:
    struct sockaddr_in address;
};
