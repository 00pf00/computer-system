#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/errno.h>

#define N 2

int main() {
    int status,i;
    pid_t pid;
    /*Parent create N children*/
    for (i = 0 ; i < N ; i++){
        if((pid = fork()) == 0){
            /*child*/
            _exit(100+i);
        }
    }
        /*Parent reaps N children in no particular order*/
        while((pid = waitpid(-1,&status,0)) > 0){
            if(WIFEXITED(status)){
                printf("child %d terminated normally whit exit status =%d\n",pid,WEXITSTATUS(status));
            }else{
                printf("child %d terminated abnormally\n",pid);
            }
        }
   /*The only normal termination is if there are more children*/
   if(errno != ECHILD){
       perror("waitpid error");
   }
   _exit(0);
}
