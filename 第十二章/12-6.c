#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "codes/csapp.h"

void echo(int connfd);
void command(void);
/*同时对输入和网络客户端做出响应*/
int main(int argc,char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set,ready_set;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
    FD_ZERO(&read_set);              /* Clear read set */ //line:conc:select:clearreadset
    FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ //line:conc:select:addstdin
    FD_SET(listenfd, &read_set);     /* Add listenfd to read set */ //line:conc:select:addlistenfd
    while (1) {
        ready_set = read_set;
        Select(listenfd+1,&ready_set,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&ready_set))
            command();
        if(FD_ISSET(listenfd,&ready_set)){
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
            echo(connfd);/* Echo client input until EOF */
            close(connfd);
        }
    }
}
void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
        printf("server received %d bytes\n", (int)n);
        Rio_writen(connfd, buf, n);
    }
}
void command()
{
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
        exit(0); /* EOF */
    printf("%s", buf); /* Process the input command */
}
