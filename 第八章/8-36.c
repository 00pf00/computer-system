
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include "codes/csapp.h"

void handler1(int sig);
int main()
{
    int i, n;
    pid_t pid;
    char buf[MAXBUF];

    pid = getpid();
    printf("self %d\n",pid);
    if(signal(SIGCHLD,handler1) == SIG_ERR)
        unix_error("signal error");
    /*Parent create children*/
    for (int i = 0; i < 3; ++i) {
        if (Fork() == 0){
            printf("Hello from child %d \n",(int)getpid());
            exit(0);
        }
    }
    /*Parent waits from terminal input and then processes it*/
    if((n=read(STDERR_FILENO,buf, sizeof(buf))) < 0)
        unix_error("read");
    printf("Parent processing input \n");
    while (1)
        ;
    exit(0);
}

void handler1(int sig)
{
    int olderrno = errno;
    if((waitpid(-1,NULL,0)) < 0)
        sio_error("waitpid error");
    Sio_puts("Handler reaped child \n");
    Sleep(1);
    errno = olderrno;

}
