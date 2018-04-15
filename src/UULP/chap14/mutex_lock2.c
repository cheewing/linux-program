#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

main()
{
    pthread_t t1;
    void *print_msg(void *);

    pthread_create(&t1, NULL, print_msg, NULL);
    pthread_join(t1, NULL);
    pthread_mutex_lock(&lock);
    printf("after main mutex lock\n");
    pthread_mutex_unlock(&lock);
}

void *print_msg(void *m)
{
    pthread_mutex_lock(&lock);
    printf("print_msg\n");
//    pthread_mutex_unlock(&lock);
}
