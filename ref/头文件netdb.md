`netdb.h` 是一个与网络编程相关的头文件，包含了处理网络地址、主机名和服务名转换的函数和结构定义。这个头文件主要用于用于 Unix 和类 Unix 系统，以便进行网络通信。

### 常用内容

以下是 `netdb.h` 中的一些常见定义和函数：

1. **结构体定义**

	- `struct hostent`：

		- 用于存储有关主机的信息，如主机名、别名和 IP 地址。

		```c
		struct hostent {
		    char *h_name;       // 官方主机名
		    char **h_aliases;   // 主机的别名列表
		    int h_addrtype;     // 地址类型（例如 AF_INET 或 AF_INET6）
		    int h_length;       // 地址长度
		    char **h_addr_list; // IP 地址列表（h_addr_list[0] 为第一个地址）
		};
		```

	- `struct servent`：

		- 用于存储服务的信息，如服务名、端口号和协议。

		```c
		struct servent {
		    char *s_name;       // 服务名
		    char **s_aliases;   // 服务的别名列表
		    int s_port;         // 端口号
		    char *s_proto;      // 协议名称（如 "tcp" 或 "udp"）
		};
		```

2. **函数原型**

	- `struct hostent *gethostbyname(const char *name);`
		- 根据主机名获取 `hostent` 结构体信息。
	- `struct hostent *gethostbyaddr(const char *addr, int len, int type);`
		- 根据 IP 地址获取 `hostent` 结构体信息。
	- `struct servent *getservbyname(const char *name, const char *proto);`
		- 根据服务名和协议获取 `servent` 结构体信息。
	- `struct servent *getservbyport(int port, const char *proto);`
		- 根据端口号和协议获取 `servent` 结构体信息。

3. **错误处理**

	- `int h_errno;`：用于指示 `gethostbyname` 和 `gethostbyaddr` 等函数的错误类型。

### 示例

以下是一个使用 `netdb.h` 的示例代码，演示如何通过主机名获取 IP 地址：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct hostent *he;
    struct in_addr addr;

    // 根据主机名获取信息
    he = gethostbyname("www.example.com");
    if (he == NULL) {
        herror("gethostbyname");
        return 1;
    }

    // 将地址转换为可读字符串
    memcpy(&addr, he->h_addr, sizeof(struct in_addr));
    printf("IP Address: %s\n", inet_ntoa(addr));

    return 0;
}
```

### 总结

`netdb.h` 头文件为网络编程提供了必要的结构体和函数，用于处理主机名、服务名和地址之间的转换。这在进行网络通信时非常重要，允许开发者查询和解析主机及服务相关的信息。随着网络编程技术的发展，现代应用程序可能会使用更高级的 API（如 `getaddrinfo()`）来替代某些 `netdb.h` 中的传统函数，但 `netdb.h` 仍然在许多程序中被广泛使用。