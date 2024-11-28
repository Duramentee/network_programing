#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handleError(const char *msg, int sock) {
    // 先打印错误信息
    perror("msg");
    // 如果文件描述符已经存在那就关闭
    if (sock > 0) {
        close(sock);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip_address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 创建 socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        handleError("socket failed", -1);
    }

    // 设置服务器地址结构
    struct sockaddr_in address = {0};  // 初始化结构体
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // 转换 IPv4 地址从文本格式到二进制格式
    if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0) {
        handleError("inet_pton failed", sock);
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        handleError("connect failed", sock);
    }

    printf("Connected to server\n");

    // 发送消息
    const char *hello = "Hello from client";
    if (send(sock, hello, strlen(hello), 0) < 0) {
        handleError("send failed", sock);
    }
    printf("Message sent to server: %s\n", hello);

    // 读取响应
    char buffer[BUFFER_SIZE] = {0}; // 留出空间给字符串结束符
    ssize_t valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread < 0) {
        handleError("read failed", sock);
    }

    buffer[valread] = '\0'; // 确保字符串结束
    printf("Message received from server: %s\n", buffer);

    // 关闭 socket
    close(sock);
    return EXIT_SUCCESS;
}