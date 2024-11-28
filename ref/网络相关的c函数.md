# unix网络编程

## 前要知识:

1.IP地址

```c
// 一个ip地址是一个 32 位的无符号整数
struct in_addr {
    uint32_t s_addr;
};
```











## 1.四个处理字节序的函数

```c
// 使用时需要包含头文件
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);

uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```

这四个函数用于在网络编程中处理字节序（endianness）问题。字节序是指多字节数据在内存中存储的顺序。在不同的计算机架构中，字节序可能有所不同：一般分为大端模式（Big-endian）和小端模式（Little-endian）。

h--host, n--network, l--long, s--short

仅提供了32位和16位的处理函数,而没有提供64位的处理函数

### 1.`htonl`

- 函数原型：`uint32_t htonl(uint32_t hostlong);`
- 功能：将一个32位的主机字长（host long）转换为网络字长（network long），即将数据转换为大端格式，以便在网络上传输。
- 返回值：转换后的32位无符号整数（`uint32_t`），其内容是将输入的主机字长（host long）从主机字节序转换为网络字节序（大端格式）。

### 2.`htons`

- 函数原型：`uint16_t htons(uint16_t hostshort);`
- 功能：将一个16位的主机字短（host short）转换为网络字短（network short），同样是将数据转换为大端格式。
- 返回值：转换后的16位无符号整数（`uint16_t`），其内容是将输入的主机字短（host short）从主机字节序转换为网络字节序（大端格式）。

### 3.`ntohl`

- 函数原型：`uint32_t ntohl(uint32_t netlong);`
- 功能：将一个32位的网络字长转换回主机字长，即将大端格式的数据转换为主机所用的字节顺序。
- 返回值：转换后的32位无符号整数（`uint32_t`），其内容是将输入的网络字长（net long）从网络字节序（大端格式）转换为主机字节序。

### 4.`ntohs`

- 函数原型：`uint16_t ntohs(uint16_t netshort);`
- 功能：将一个16位的网络字短转换回主机字短，也就是将大端格式的数据转换为主机所用的字节顺序。
- 返回值：转换后的16位无符号整数（`uint16_t`），其内容是将输入的网络字短（net short）从网络字节序（大端格式）转换为主机字节序。

这些函数在网络编程中非常常用，确保在不同平台之间传输数据时，数据格式保持一致，避免因字节序不同导致的数据错误。



## 2. 两个用于IP地址格式转换的函数

```c
// 使用时需要包含头文件
#include <arpa/inet.h>

int inet_pton(af, const char *src, void *dst);
const char *inet_ntop(af, const char *src, char *dst, socklen_t size);
```

`inet_pton` 和 `inet_ntop` 是用于IP地址格式转换的两个重要函数。

### 1.`inet_pton`

- 函数原型：`int inet_pton(int af, const char *src, void *dst);`
- 功能：这个函数用于将点分十进制表示的IPv4地址或者以冒号分隔的IPv6地址字符串（如"192.168.1.1"或"::1"）转换为网络字节序的二进制形式。
- 参数：
	- `af`：地址家族，通常是两个宏其一 `AF_INET`（IPv4）或 `AF_INET6`（IPv6）。
	- `src`：指向需要转换的IP地址字符串的指针。
	- `dst`：指向存储转换后结果的缓冲区指针，通常是一个`struct in_addr`或者`struct in6_addr`类型的结构体。
- 返回值：成功时返回1，失败时返回0，参数错误时返回-1。



### 2.`inet_ntop`

- 函数原型：`const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);`
- 功能：这个函数用于将网络字节序的二进制形式的IP地址转换为人类可读的字符串格式。
- 参数：
	- `af`：地址家族，通常是两个宏其一 `AF_INET` 或 `AF_INET6`。
	- `src`：指向需要转换的二进制IP地址的指针。
	- `dst`：指向存储转换后结果的缓冲区指针。
	- `size`：`dst`缓冲区的大小。
- 返回值：成功时返回指向结果字符串的指针，失败时返回NULL。

这两个函数在网络编程中非常常见，主要用于处理IP地址的表示和转换，方便与网络协议的交互。