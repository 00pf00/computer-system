
#include <sys/param.h>
#include "codes/csapp.h"


typedef struct pid_node
{
    int pid;
    struct pid_node * next;
} node;
node* list = NULL;
node* next = NULL;
void initjobs();
void addjob(int pid);
void deletejob(int pid);
void handler(int sig);
/*fork之后，子进程运行结束之后，SIGCHLD信号没有阻塞，
就会导致子程序无法被移除*/
int main(int argc,char **argv)
{
    int pid,main_pid;
    main_pid = getpid();
    printf("main_pid = %d\n",main_pid);
    sigset_t mask_all,prev_all;
    Sigfillset(&mask_all);
    Signal(SIGCHLD,handler);
    initjobs();/*Initialize the job list*/
    for (int i = 0; i < 10; ++i) {
        if((pid = Fork()) == 0){
            Execve("/bin/date",argv,NULL);
        }
        printf("child_pid = %d\n",pid);
        sleep(10);
        Sigprocmask(SIG_BLOCK,&mask_all,&prev_all);/*Parent process*/
        addjob(pid);/*Add the child to the job list*/
        Sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    if(list->next != NULL){
        node* p = list;
        int counter = 0;
        while(p->next != NULL){
            p = p->next;
            if(p->pid > 0){
                counter++;
                printf("没有移除的pid_%d = %d\n",counter,p->pid);
            }
        }
    }
    if(list->next != NULL){
        node* p = list;
        while(p->next != NULL){
          node* temp = p->next;
          free(p);
          p = temp;
        }
    }
    free(next);
    exit(0);
}
void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
}
void initjobs()
{
    list = malloc(sizeof(node));
}
void addjob(int pid)
{
    node* p = (node *)malloc(sizeof(node));
    p->pid = pid;
    if(next == NULL){
        list->next = p;
    }else{
        next->next = p;
    }
    next = p;
}
void deletejob(int pid)
{
    printf("要移除的pid= %d\n",pid);
    int flag = 0;
    if(next != NULL){
        node* p = list;
        while (p->next != NULL){
            if(p->next->pid == pid){
                if(p->next == next){
                    free(next);
                    p->next = NULL;
                    next = p;
                }else{
                   node* temp = p->next->next;
                   free(p->next);
                   p->next = temp;
                }
                flag = 1;
                break;
            }else{
                p = p->next;
            }
        }
    }
    if(flag != 1){
        printf("%d 没有添加到队列!!!\n",pid);
    }
    if(list->next != NULL){
        node* p = list;
        int counter = 0;
        while(p->next != NULL){
            p = p->next;
            if(p->pid > 0){
                counter++;
                printf("没有移除的pid_%d = %d\n",counter,p->pid);
            }
        }
    }
}
