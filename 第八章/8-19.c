#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/errno.h>

#define N 2

int main() {
    int status,i;
    pid_t pid[N],retpid;
    /*Parent create N children*/
    for (i = 0 ; i < N ; i++){
        if((pid[i] = fork()) == 0){
            /*child*/
            _exit(100+i);
        }
    }
        /*Parent reaps N children in order*/
        i = 0;
        while((retpid = waitpid(pid[i++],&status,0)) > 0){
            if(WIFEXITED(status)){
                printf("child %d terminated normally with exit status =%d\n",retpid,WEXITSTATUS(status));//WEXITSTATUS()判断进程是否正常退出
            }else{
                printf("child %d terminated abnormally\n",retpid);
            }
        }
   /*The only normal termination is if there are more children*/
   if(errno != ECHILD){
       perror("waitpid error");
   }
   _exit(0);
}
