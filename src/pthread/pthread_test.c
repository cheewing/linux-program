#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int money;
pthread_mutex_t mutex;
pthread_mutexattr_t mutex_attr;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void err_exit(const char *err_msg)
{
	printf("error: %s\n", err_msg);
	exit(1);
}

void *pthread_func(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);

		if (money > 0)
		{
			printf("子线程坐等money等于0...\n");
			pthread_cond_wait(&cond, &mutex);
		}

		printf("子线程进入临界区查看money\n");

		if (money == 0)
		{
			money += 200;
			printf("子线程：money=%d\n", money);
		}

		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	return NULL;
}

int main()
{
	pthread_t thread;
	money = 1000;

	if (pthread_mutexattr_init(&mutex_attr) != 0)
		err_exit("pthread_mutexattr_init");

	if (pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL) != 0)
		err_exit("pthread_mutexattr_settype");

	// 初始化mutex
	if (pthread_mutex_init(&mutex, &mutex_attr) != 0)
		err_exit("pthread_mutex_init");

	if (pthread_create(&thread, NULL, pthread_func, NULL) != 0)
		err_exit("pthread_create");

	
	while (1) 
	{
		pthread_mutex_lock(&mutex);

		if (money > 0)
		{
			money -= 100;
			printf("主线程进入临界区查看money=%d\n", money);
		}

		pthread_mutex_unlock(&mutex);

		if (money == 0)
		{
			// 通知子线程
			printf("通知子线程\n");
			pthread_cond_signal(&cond);
		}
		sleep(1);
	}

	exit(1);
}