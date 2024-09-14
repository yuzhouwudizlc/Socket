#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");
/*
    创建套接字
*/
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888); //host to network short
/*
    设置服务器地址
*/
    errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr)) == -1,"socker bind error");
/*
    使用 bind 函数将套接字绑定到指定的地址和端口。
*/
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
/*
    使用 listen 函数开始监听传入的连接请求，SOMAXCONN 是系统允许的最大队列长度。
*/
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr,sizeof(clnt_addr));

  
    int clnt_sockfd = accept (sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
     errif(clnt_sockfd == -1, "socket accept error");
     
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
/*
    创建 sockaddr_in 结构体 clnt_addr 来存储客户端地址信息。
    使用 accept 函数接受客户端的连接请求，返回一个新的套接字 clnt_sockfd 用于与客户端通信。
*/
    while(true){
        char buf[1024];
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd,buf,sizeof(buf));
        if(read_bytes > 0){
            printf("message from client fd %d :%s\n",clnt_sockfd,buf);
            write(clnt_sockfd,buf,sizeof(buf));
        }else if(read_bytes == 0){
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
        }else if(read_bytes == -1){
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
        
    }
    close(sockfd);
    return 0;


}