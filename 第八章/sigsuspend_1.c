#include <unistd.h>
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <signal.h>
#include "codes/csapp.h"

int counter = 0;
void handler(int sig);
/*主要解决取消阻塞和pause之间SIGINT信号到达之后，程序不会借宿而是程序继续等待SIGINT
信号到来程序才会结束*/
int main()
{
    sigset_t new,old,wait; //三个信号集
    struct sigaction act;
    pid_t pid = getpid();
    printf("%d\n",pid);
    act.sa_handler = handler;//信号处理函数handler
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGQUIT,&act,0);
    sigaction(SIGUSR1,&act,0);
    sigaction(SIGINT,&act,0);//可以捕捉以下三个信号：SIGINT/SIGQUIT/SIGUSR1
    sigemptyset(&new);
    sigaddset(&new, SIGINT);//SIGINT信号加入到new信号集中
    sigemptyset(&wait);
    sigaddset(&wait,SIGUSR1);//SIGUSR1信号加入wait
    sigprocmask(SIG_BLOCK, &new, &old);  //将SIGINT信号阻塞，同时保存当前信号集
    counter = counter+1;
    for (int i = 0; i < 15; ++i)
        sleep(1);
    printf("main_after counter = %d\n",counter);
    if(sigsuspend(&wait) != -1)      //程序在此处挂起;用wait信号集替换new信号集.即：过来SIGUSR1信号阻塞掉,程序继续挂起;过来其他信号,例如SIGINT,则会唤醒程序.
        printf("sigsuspend error\n"); // 执行sigsuspend的原子操作.注意:如果“sigaddset(&wait, SIGUSR1);”这句没有,则此处不会阻塞任何信号，即过来任何信号均会唤醒程序.
    printf("After sigsuspend\n");

    sigprocmask(SIG_SETMASK, &old, NULL);  //取消阻塞
    kill(pid,SIGINT);
    printf("end\n");
    return 0;
}
void handler(int sig)
{
    printf("now counter = %d\n",counter);
    counter = counter +1;
    printf("handler counter = %d\n",counter);
//    printf("sig = %d\n",sig);
//    if(sig == SIGINT)
//        printf("SIGINT sig %d\n",sig);
//    else if(sig == SIGQUIT)
//        printf("SIGQUIT sig %d\n",sig);
//    else
//        printf("SIGUSR1 sig %d\n",sig);
}
