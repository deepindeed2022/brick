#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define DEST_PORT 1500      //目标地址端口号
#define DEST_IP "127.0.0.1" /*目标地址IP，这里设为本机*/
#define MAX_DATA 1024        //接收到的数据最大程度

void help(char* argv[]) {
    fprintf(stderr, "%s [127.0.0.1] [port:1500] [command]\n", argv[0]);
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        help(argv);
        exit(1);
    }
    char* dest_ip = argv[1];
    int dest_port = atoi(argv[2]);
    char* command = argv[3];
    int sockfd, new_fd;           /*cocket句柄和接受到连接后的句柄 */
    struct sockaddr_in dest_addr; /*目标地址信息*/
    char buf[MAX_DATA];           //储存接收数据

    sockfd = socket(AF_INET, SOCK_STREAM, 0); /*建立socket*/
    if (sockfd == -1) {
        printf("socket failed:%d", errno);
    }

    //参数意义见上面服务器端
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DEST_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    bzero(&(dest_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {                                       //连接方法，传入句柄，目标地址和大小
        printf("connect failed:%d", errno); //失败时可以打印errno
    } else {
        fprintf(stderr, "send to:%s\n", command);
        send(sockfd, command, strlen(command) + 1, 0);

        // 将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。
        while(recv(sockfd, buf, MAX_DATA, 0) > 0) {
            if(buf[0] == 0x7F && buf[1] == 0xEE) break;
            char* data = buf+2;
            fprintf(stderr, "from server:%s", data);
            fprintf(stderr, ">> check num: %#x %#x\n", buf[1], buf[0]);
        }
    }
    close(sockfd); //关闭socket
    return 0;
}