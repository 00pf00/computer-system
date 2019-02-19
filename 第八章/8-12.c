
#include <sys/param.h>
#include <cygwin/signal.h>
#include "codes/csapp.h"

volatile sig_atomic_t pid = 0;
void sigchld_handler(int s);
void sigint_handler(int s);
/*sigsuspend会打破临界区的阻塞信号，阻塞当前信号*/
int main(int argc, char **argv)
{
    sigset_t mask, prev,new;

    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);
    Sigemptyset(&prev);
    Sigaddset(&prev,SIGINT);

    printf("main = %d\n",getpid());
    while (1) {
        Sigprocmask(SIG_BLOCK, &mask, &new); /* Block SIGCHLD */
//        if (Fork() == 0) /* Child */
//        {
//            printf("child = %d\n",getpid());
//            pause();
//        }

            //exit(0);

        /* Wait for SIGCHLD to be received */
//        pid = 0;
//        while (!pid)
        Sigsuspend(&prev);
        pid = pid +1;
        printf("main pid = %d\n",pid);
        Sigprocmask(SIG_SETMASK, &new, NULL);

        /* Do some work after receiving SIGCHLD */
        printf(".\n");
    }
    exit(0);
}
void sigchld_handler(int s)
{
//    int olderrno = errno;
//    pid = Waitpid(-1, NULL, 0);
//    errno = olderrno;
//       printf("handler sigchld = %d\n",s);
     pid = pid +1;
     printf("sigchld pid = %d\n",pid);
}
void sigint_handler(int s)
{
    //printf("handler sigint = %d\n",s);
    pid = pid +1;
    printf("sigint pid = %d\n",pid);
}
