#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

main()
{
    int errno = pthread_mutex_unlock(&lock);
    printf("errno: %d\n", errno);
}
