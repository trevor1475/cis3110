//Trevor Livingston - 0833394
//https://www.cs.mtu.edu/~shene/NSF-3/e-Book/MUTEX/TM-example-left-right.html
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

void *philosopher(void *arg);

pthread_mutex_t chopsticks[1000];
int attendingPhilosophers;
int eatCount;
int started;

struct Philo {
	int index;
};

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Invalid arguements (argv[1] = #philosophers, argv[2] = #eats)\n");
		exit(1);
	}
	else
	{
		attendingPhilosophers = atoi(argv[1]);
		eatCount = atoi(argv[2]) * attendingPhilosophers;
		started = 0;
	}

	pthread_t philoThreads[attendingPhilosophers];
	struct Philo philoArray[attendingPhilosophers];
	int i = 0;

	for(i = 0; i < attendingPhilosophers; i++)
	{
		if (pthread_mutex_init(&chopsticks[i],0) != 0)
		{
			printf("Mutex initializing error\n");
			exit(1);
		}
		philoArray[i].index = i;
	}

	for(i = 0; i < attendingPhilosophers; i++)
	{
		if(pthread_create(&philoThreads[i],NULL, philosopher, &philoArray[i]))
		{
			printf("Thread failure\n");
			exit(1);
		}
	}

	for(i = 0; i < attendingPhilosophers; i++)
	{
		if(pthread_join(philoThreads[i], NULL))
		{
			printf("Thread failure\n");
			exit(1);
		}
	}

	for(i = 0; i < attendingPhilosophers; i++)
	{
		pthread_mutex_destroy(&chopsticks[i]);
	}
	return 0;
}

void *philosopher(void *arg)
{
	struct Philo *p = (struct Philo*)arg;
	int index = p->index;
	int i = 0;

	while(eatCount != 0)
	{
		pthread_mutex_lock(&chopsticks[index]);
		pthread_mutex_lock(&chopsticks[(index + 1) % attendingPhilosophers]);

		if(started == 0)
		{
			printf("Philosopher %d is thinking\n", index + 1);
			started = 1;
		}

		printf("Philosopher %d is eating\n", index + 1);
		eatCount--;

		for(i = 0; i < attendingPhilosophers; i++)
		{
			if(i != index)
			{
				printf("Philosopher %d is thinking\n", i + 1);
			}
		}
		printf("Philosopher %d is thinking\n", index + 1);

		pthread_mutex_unlock(&chopsticks[index]);
		pthread_mutex_unlock(&chopsticks[(index + 1) % attendingPhilosophers]);
		sleep(1);
	}
	return 0;
}