/**
 * @file pthread.barrier.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PTHREAD_BARRIER_SIZE 4

pthread_barrier_t barrier;

void err_exit(const char *err_msg)
{
    printf("error: %s\n", err_msg);
    exit(1);
}

void *thread_fun(void *arg)
{
    int result;
    char *thr_name = (char *)arg;

    printf("线程%s工作完成...\n", thr_name);

    result = pthread_barrier_wait(&barrier);
    if (result == PTHREAD_BARRIER_SERIAL_THREAD)
        printf("线程%s, wait后第一个返回\n", thr_name);
    else if (result == 0)
        printf("线程%s, wait后返回为0\n", thr_name);

    return NULL;
}

int main(void)
{
    pthread_t tid_1, tid_2, tid_3;

    pthread_barrier_init(&barrier, NULL, PTHREAD_BARRIER_SIZE);

    if (pthread_create(&tid_1, NULL, thread_fun, "1") != 0)
        err_exit("pthread_create()");

    if (pthread_create(&tid_2, NULL, thread_fun, "2") != 0)
        err_exit("pthread_create()");

    if (pthread_create(&tid_3, NULL, thread_fun, "3") != 0)
        err_exit("pthread_create()");

    pthread_barrier_wait(&barrier);
    printf("所有线程工作已完成...\n");

    sleep(1);
    return 0;
}
