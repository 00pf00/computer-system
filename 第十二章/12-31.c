#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "codes/csapp.h"

#define MAXTHREADS 32

void *sum_mutex(void *vargp);
long gsum = 0;
long nelems_per_thread;
sem_t mutex;
/*1L << log_nelems 得到的值为2的log_nelems次幂*/
int main(int argc,char **argv)
{
    long i, nelems, log_nelems, nthreads, myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];
    if (argc != 3) {
        printf("Usage: %s <nthreads> <log_nelems>\n", argv[0]);
        exit(0);
    }
    nthreads = atoi(argv[1]);
    log_nelems = atoi(argv[2]);
    nelems = (1L << log_nelems);
    /* $end psummutex */
    /* Check input arguments */
    if  ((nelems % nthreads) != 0 || (log_nelems > 31)) {
        printf("Error: invalid nelems\n");
        exit(0);
    }
    /* $begin psummutex */
    nelems_per_thread = nelems / nthreads;
    sem_init(&mutex, 0, 1);

    /* Create peer threads and wait for them to finish */
    for (i = 0; i < nthreads; i++) {                  //line:conc:psummutex:createloop1
        myid[i] = i;                                  //line:conc:psummutex:createloop2
        Pthread_create(&tid[i], NULL, sum_mutex, &myid[i]); //line:conc:psummutex:createloop3
    }                                                 //line:conc:psummutex:createloop4
    for (i = 0; i < nthreads; i++)                    //line:conc:psummutex:waitloop1
        Pthread_join(tid[i], NULL);                   //line:conc:psummutex:waitloop2

    /* Check final answer */
    if (gsum != (nelems * (nelems-1))/2)     //line:conc:psummutex:check1
        printf("Error: result=%ld\n", gsum); //line:conc:psummutex:check2
    printf("gsum = %d\n",gsum);
    exit(0);
}
void *sum_mutex(void *vargp)
{
    long myid = *((long *)vargp);          /* Extract the thread ID */ //line:conc:psummutex:extractid
    long start = myid * nelems_per_thread; /* Start element index */ //line:conc:psummutex:getstart
    long end = start + nelems_per_thread;  /* End element index */ //line:conc:psummutex:getend
    long i;

    for (i = start; i < end; i++) {        //line:conc:psummutex:threadsumloop1
        P(&mutex);                     //line:conc:psummutex:lock
        gsum += i;                     //line:conc:psummutex:threadsumloop2
        V(&mutex);                     //line:conc:psummutex:unlock
    }	                               //line:conc:psummutex:threadsumloop3
    return NULL;
}
