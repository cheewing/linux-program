/**
 * @file pthread_cond.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mtx;
pthread_mutexattr_t mtx_attr;
int money;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void err_exit(const char *err_msg)
{
    printf("error: %s\n", err_msg);
    exit(1);
}

void *thread_fun(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mtx);

        if (money > 0)
        {
            printf("子线程坐等money等于0...\n");
            pthread_cond_wait(&cond, &mtx);
        }
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
    money = 1000;

    if (pthread_mutexattr_init(&mtx_attr) == -1)
        err_exit("phtread_mutexattr_init()");

    if (pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL) == -1)
        err_exit("pthread_mutexattr_settype()");

    if (pthread_mutex_init(&mtx, &mtx_attr) == -1)
        err_exit("pthread_mutex_init()");

    if (pthread_create(&tid, NULL, thread_fun, NULL) == -1)
        err_exit("pthread_create()");

    while (1)
    {
        pthread_mutex_lock(&mtx);
        if (money > 0)
        {
            money -= 100;
            printf("主线程进入临界区查看money = %d\n", money);
        }
        pthread_mutex_unlock(&mtx);

        if (money == 0)
        {
            printf("通知子线程\n");
            pthread_cond_signal(&cond);
        }
        sleep(1);
    }

    return 0;
}
