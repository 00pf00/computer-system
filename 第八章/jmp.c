#include <setjmp.h>
#include <stdio.h>

jmp_buf buf;
void banana();
int main()
{
    if(setjmp(buf)){
        printf("back in main \n");
    }else{
        printf("first time through\n");
        banana();
    }
}
void banana()
{
    printf("in banana()\n");
    longjmp(buf,1);//回到buf所记录的位置，并返回1，同时buf的内容被销毁
    /* the code can't be processed*/
    printf("youo'll never see this, because i longjmp'd");
}
