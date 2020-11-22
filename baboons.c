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

sem_t rope;
sem_t right_mutex;
sem_t left_mutex;
sem_t deadlock_protection;
sem_t counter;

int left = 0;
int right = 0;
int cross_time;

void *left_to_right(){

}

void *right_to_left(){

}

int main(){ 
    
}