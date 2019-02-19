#include <setjmp.h>
#include "codes/csapp.h"

jmp_buf buf;
void handler(int sig);
int main()
{
    printf("main_pid = %d\n",getpid());
    if(!sigsetjmp(buf,1)){
        Signal(SIGINT,handler);
        Sio_puts("starting\n");
    }else{
        Sio_puts("restarting\n");
    }

    while (1){
        Sleep(1);
        Sio_puts("processing...\n");
    }
}

void handler(int sig)
{
    siglongjmp(buf,1);
}
