#include <sys/param.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "codes/csapp.h"
#include "codes/sbuf.h"
#define NTHREADS  4
#define SBUFSIZE  16
static sem_t mutex;
static int byte_cnt;
sbuf_t sbuf;
void *thread(void *vargp);
void echo_cnt(int connfd);
static void init_echo_cnt(void);
int main(int argc,char **argv)
{
  int i,listenfd,connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); //line:conc:pre:initsbuf
    for (i = 0; i < NTHREADS; i++)  /* Create worker threads */ //line:conc:pre:begincreate
        Pthread_create(&tid, NULL, thread, NULL);               //line:conc:pre:endcreate

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* 将连接添加到连接池中*/
    }
}
void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf); /* 将连接从连接池中去打出来*/ //line:conc:pre:removeconnfd
        echo_cnt(connfd);                /* Service client */
        Close(connfd);
    }
}
void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    Pthread_once(&once, init_echo_cnt);
    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        P(&mutex);
        byte_cnt += n; //line:conc:pre:cntaccess1
        printf("server received %d (%d total) bytes on fd %d\n",
               n, byte_cnt, connfd); //line:conc:pre:cntaccess2
        V(&mutex);
        Rio_writen(connfd, buf, n);
    }
}
static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}
