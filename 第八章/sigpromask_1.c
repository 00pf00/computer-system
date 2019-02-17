#include <unistd.h>
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <signal.h>
#include "codes/csapp.h"

void handler(int sig);
/*sigaction()设置捕捉的信号类型，只捕捉设置的信号类型
sigprocmask(SIG_BLOCK) 设置信号阻塞期间，对发送的多个捕捉的信号只保留一个
*/

int main()
{
    sigset_t new,old;
    struct sigaction act;
    pid_t pid = getpid();
    printf("%d\n",pid);
    act.sa_handler = handler;//信号处理函数handler
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT,&act,0);//准备捕捉SIGINT信号
    sigemptyset(&new);
    sigemptyset(&old);
    sigaddset(&new, SIGINT);
    sigprocmask(SIG_BLOCK, &new, &old);  //将SIGINT信号阻塞，同时保存当前信号集
    printf("Blocked\n");
    sigprocmask(SIG_SETMASK, &old, NULL);  //取消阻塞
    pause();  //让进程暂停直到信号出现
    printf("end\n");
    return 0;
}
void handler(int sig)
{
    printf("SIGINT sig = %d\n",sig);
}
