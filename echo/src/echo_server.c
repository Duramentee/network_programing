#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

// 8080 是一个常见的 HTTP 替代端口, 通常在开发和测试 Web 服务时使用
// 8080 被广泛用于当地开发服务器, 特别是在 80 端口 (HTTP 的标准端口) 被其他的服务占用时
#define PORT 8080
// BUFFER_SIZE 宏定义, 表示缓冲区大小
#define BUFFER_SIZE 1024

void handle_error(const char *msg, int server_fd) {
    perror(msg);
    if (server_fd >= 0) {
        close(server_fd);
    }
    // 宏 EXIT_FAILURE 为非零值,unix中用于表示错误返回
    exit(EXIT_FAILURE);
}

int main() {
    // server_fd 是服务器的套接字文件描述符, 用于标识服务器的 Socket
    // 后续的操作 (诸如绑定,监听,接受连接)都将使用这个描述符.
    int server_fd = -1;
    // new_socket 用于接受新连接的套接字文件描述符,
    // 当有客户端连接时, 该变量将保存新建立的链接的 Socket 描述符
    int new_socket = -1;
    // address 是一个结构体, 用于存储与网络相关的地址信息, 包括地址组, IP地址, 端口号等.
    // 该结构体在绑定和接受链接时将被使用
    struct sockaddr_in address = {0};
    // buffer 是一个字符数组, 用于存储接收到的数据, 初始化为 0 确保缓冲区在使用之前是清空的。
    char buffer[BUFFER_SIZE] = {0};

    // 为服务器创建一个 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        handle_error("socket failed", -1);
    }

    // 绑定
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY 表示服务器可以接受来自任何网络接口的连接请求
    address.sin_port = htons(PORT);  // sin_port 用网络字节序表示,要做一次表示格式的转换

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        handle_error("bind failed", server_fd);
    }

    // 监听
    if (listen(server_fd, 3) < 0) {
        handle_error("listen failed", server_fd);
    }

    printf("Server listening on port %d\n", PORT);

    // 接受连接
    socklen_t addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket < 0) {
        handle_error("accept failed", server_fd);
    }

    // 打印具体来自哪个监听描述符成功接受了连接
    printf(
        "Connection accepted from %s:%d\n",
        inet_ntoa(address.sin_addr),  // 打印IP地址
        ntohs(address.sin_port)  // 打印端口号
    );

    while (1) {
        // valread 用于存储从套接字读取的字节数，后续会用来判断读取的数据是否正常。
        // read函数调用, 从已连接描述符读数据到 buffer, 最多读 BUFFER_SIZE(1024) 字节
        ssize_t valread = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (valread < 0) {
            perror("read failed");
            break;
        } else if (valread == 0) {
            printf("Client disconnected\n");
            break;  // 客户端关闭连接
        }

        buffer[valread] = '\0';
        printf("Received: %s", buffer);

        if (send(new_socket, buffer, valread, 0) < 0) {
            perror("send failed");
            break;
        }
    }

    printf("Connection closed\n");
    close(new_socket);
    close(server_fd);

    return 0;
}