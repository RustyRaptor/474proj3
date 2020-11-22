#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

 #define rope_stall	7000

sem_t rope;
sem_t right_mutex;
sem_t left_mutex;
sem_t deadlock_protection;
sem_t counter;

typedef struct thread_val{
    int tid; //id thread
}thread_val;

int left = 0;
int right = 0;

int leftCount = 0;
int rightCount = 0;
int cross_time;

sem_t a,b;

typedef enum {None,toLeft, toRight} crossing;
crossing leftDirection = None;

void *left_to_right(){
    thread_val *val = (thread_val *);
    semwait(&left_mutex);

    if((leftDirection == toLeft || leftDirection == None) && left < 4 &&
		(right + left < 10 || rightCount == 0) && leftCount == 0)
	{
		printf("Going Left \n");
		leftDirection = toLeft;
		leftCount++;
		printf("Going Left Mutex.\n");
		semsignal(&left_mutex);
	}

    else
	{
		printf(" Going Left is waiting.\n");
		leftCount++;
		printf(" Going Left Mutex.\n");
		semsignal(&left_mutex);
		semwait(&a);
		printf("Going left waiting, now im signaled.\n");
		leftCount--;
		left++;
		leftDirection = toLeft;

		// *** Baboons check to see if anyone is waiting and then signal the other.
		if(leftCount > 0 && left < 4 && (right + left < 10 || toRight == 0))
		{
			printf("Going Left.\n");
			printf(" Signal thread behind me.\n");
			semsignal(&rope);
		}
		else
		{
			printf(" Going left.\n");
			printf(" Going Signaling Mutex.\n");
			semsignal(&left_mutex);
		}
	}

    printf(" Going left is crossing the rope.\n");
	// Time to cross rope
	stall(rope_stall);

	printf(" wait for Mutex.\n");
	semwait(&left_mutex);
	printf("Passed Mutex.\n");
	rightCount++;
	leftCount--;

	if(leftCount != 0 && (left + right < 10 || rightCount == 0))
	{
		printf("Going signaling.\n");
		semsignal(&a);
	}
	else if (left == 0 && rightCount != 0 &&
		(leftCount == 0 || left + right >= 10))
	{
		printf(" Going left changing direction to Going Right.\n");
		leftDirection = toRight;
		right = 0;
		printf("Going Left signaling a waiting GoingRight.\n");
		semsignal(&b);
	}
	else if(left == 0 && right == 0 && leftCount == 0)
	{
		printf("Thread id: %d CrossToA setting direction to none.\n");
		leftDirection = None;
		right = 0;
		printf("Going left Signaling Mutex.\n");
		semsignal(&left_mutex);
	}
	else
	{
		printf("Going Left Signaling Mutex.\n");
		semsignal(&left_mutex);
	}

	fflush(stdout);
	pthread_exit(NULL);
}

void *right_to_left(){

}

int main(){ 
    
}