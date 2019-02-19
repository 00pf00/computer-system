
#include <sys/param.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "codes/csapp.h"
#define N 2
void *thread(void *vargp);
char **ptr;
int main()
{
    int i;
    pthread_t  tid;
    char *msgs[N] ={
            "Hello from foo",
            "Hello from bar"
    };
    ptr = msgs;
    for (int i = 0; i < N; i++) {
        pthread_create(&tid,NULL,thread,(void *)i);
    }
}
void *thread(void *vargp)
{
    int myid = (int)vargp;
    static int cnt = 0;
    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt); //line:conc:sharing:stack
    return NULL;
}
