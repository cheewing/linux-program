/**
 * @file pthread_rwlock.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int global_num = 10;

void err_exit(const char *errmsg)
{
    printf("error: %s\n", errmsg);
    exit(1);
}

void *thread_read_lock(void *arg)
{
    char *pthr_name = (char *)arg;

    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("线程%s进入临界区, global_num = %d\n", pthr_name, global_num);
        sleep(1);
        printf("线程%s离开临界区...\n", pthr_name);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

void *thread_write_lock(void *arg)
{
    char *pthr_name = (char *)arg;

    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        printf("线程%s进入临界区, global_num = %d\n", pthr_name, global_num);
        sleep(1);
        printf("线程%s离开临界区...\n", pthr_name);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid_read_1, tid_read_2, tid_write_1, tid_write_2;

    if (pthread_create(&tid_read_1, NULL, thread_read_lock, "read1") != 0)
        err_exit("pthread_create()");

    if (pthread_create(&tid_read_2, NULL, thread_read_lock, "read2") != 0)
        err_exit("pthread_create()");

    if (pthread_create(&tid_write_1, NULL, thread_write_lock, "write1") != 0)
        err_exit("pthread_create()");

    if (pthread_create(&tid_write_2, NULL, thread_write_lock, "write2") != 0)
        err_exit("pthread_create()");

    if (pthread_join(tid_read_1, NULL) != 0)
        err_exit("pthread_join()");

    return 0;
}
