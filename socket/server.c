#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define PORT 1500 //端口号
#define BACKLOG 5 /*最大监听数*/
#define MAX_DATA 1024

void help(char* argv[]) {
    fprintf(stderr, "%s [port] [backlog]\n", argv[0]);
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        help(argv);
        exit(1);
    }
    int portnum = atoi(argv[1]);
    int backlog = argc > 3 ? atoi(argv[2]) : BACKLOG;
    int sockfd, new_fd;             // socket句柄和建立连接后的句柄
    struct sockaddr_in server_addr; // 本方地址信息结构体，下面有具体的属性赋值
    struct sockaddr_in their_addr;  // 对方地址信息
    int sin_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // 建立socket
    if (sockfd == -1)
    {
        printf("socket failed:%d", errno);
        return -1;
    }
    server_addr.sin_family = AF_INET;                // 该属性表示接收本机或其他机器传输
    server_addr.sin_port = htons(PORT);              // 端口号
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP，括号内容表示本机IP
    bzero(&(server_addr.sin_zero), 8);               // 将其他属性置0

    // 绑定地址结构体和socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        fprintf(stderr, "server bind error");
        return -1;
    }
    // 开启监听 ，第二个参数是最大监听数
    listen(sockfd, backlog);
    char buf[MAX_DATA];
    while (1) {
        // 在这里阻塞直到接收到消息，参数分别是socket句柄，接收到的地址信息以及大小
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); 
        if (new_fd == -1) {
            printf("receive failed");
        } else {
            size_t recv_count = recv(new_fd, buf, 1500, 0);
            if(recv_count > 0) {
                fprintf(stderr, "from client:%s\n", buf);
                //
                // TODO: parse the command and execute in os
                //
                FILE * fp;
                fp = popen(buf, "r");
                char line[1024];
                int len = 2;
                while(fgets(line, 1024, fp)) {
                    if(strlen(buf) + strlen(line) > 1024) {
                        fprintf(stderr, "send to: %s", &buf[2]);
                        buf[0] = 0x7F;
                        buf[1] = 0xEA;
                        memset(buf+len, 0, MAX_DATA - len);
                        //发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可）
                        if(send(new_fd, buf, MAX_DATA - 1, 0) != -1) {
                            len = 2;
                        }
                    }
                    strcpy(buf + len, line);
                    len += strlen(line);
                }
                buf[0] = 0x7F;
                buf[1] = 0xEE;
                //发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可）
                if(send(new_fd, buf, strlen(buf) + 1, 0) != -1) {
                    continue;
                }
            }
        }
    }
    return 0;
}