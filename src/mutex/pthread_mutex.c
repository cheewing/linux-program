/**
 * @file pthread_mutex.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mtx;
pthread_mutexattr_t mtx_attr;
int money;

void err_exit(const char *err_msg)
{
    printf("error:%s\n", err_msg);
    exit(1);
}

void *thread_fun(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mtx);
        printf("子线程进入临界区查看money\n");
        if (money == 0)
        {
            money += 200;
            printf("子线程: money = %d\n", money);
        }
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid;

    if (pthread_mutexattr_init(&mtx_attr) == -1)
        err_exit("pthread_mutexatttr_init()");

    if (pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL) == -1)
        err_exit("pthread_mutexattr_settype()");

    if (pthread_mutex_init(&mtx, &mtx_attr) == -1)
        err_exit("pthread_mutexattr_init()");

    if (pthread_create(&tid, NULL, thread_fun, NULL) == -1)
        err_exit("pthread_create()");

    money = 1000;
    while (1)
    {
        pthread_mutex_lock(&mtx);
        if (money > 0)
        {
            money -= 100;
            printf("主线程: money = %d\n", money);
        }
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    return 0;
}
