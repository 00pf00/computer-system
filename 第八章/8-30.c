
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int sig); /*SIGINT handler*/
int main() {

    pid_t pid;
    pid = getpid();
    printf("%d \n",pid);
    /*IInstall the SIGINT handler*/
    if(signal(10,sigint_handler) == SIG_ERR)
        perror("signal error");

    pause();
    return 0;
}

void sigint_handler(int sig)
{
    printf("Caught SIGINT");
    exit(0);
}
