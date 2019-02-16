
#include <unistd.h>
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <signal.h>
#include "codes/csapp.h"

volatile  long counter = 2;
void handler1(int sig);
int main()
{
    pid_t pid;
    sigset_t mask,prev_masker;

    printf("%ld",counter);
    fflush(stdout);
    signal(SIGUSR1,handler1);
    if ((pid = Fork())==0)
    {
        while (1){};
    }
    Kill(pid,SIGUSR1);
    Waitpid(-1,NULL,0);

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK,&mask,&prev_masker); /*Block sigs*/
    printf("%ld",counter++);
    Sigprocmask(SIG_SETMASK,&prev_masker,NULL);/*Restore sigs*/

    exit(0);
}

void handler1(int sig)
{
    sigset_t mask, prev_mask;

    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    Sio_putl(--counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    _exit(0);
}
