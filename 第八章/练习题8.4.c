#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/errno.h>


int main() {
    int status;
    pid_t pid;

    printf("Hello\n");
    pid = fork();
    printf("%d\n",!pid);
    if(pid != 0){
        if(waitpid(-1,&status,0) > 0){
            if(WIFEXITED(status) != 0){
                printf("parent =  %d\n",WEXITSTATUS(status));
            }
        }
    }
    printf("Bye\n");
    _exit(2);
}
