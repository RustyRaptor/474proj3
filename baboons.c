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

 typedef struct{
     int capacity;
     int direction;
     int num_baboons;
     int time;
     int iter;
 }sharedrope;

sharedrope *canyonrope;

void *left_to_right(){
    do{
        sem_wait(&left_mutex);
        sem_wait(&rope);
        canyonrope->direction = 1;
        if(canyonrope->num_baboons < canyonrope->capacity){
            canyonrope->num_baboons = canyonrope->num_baboons + 1;
            printf("Baboon %d is going right\n", canyonrope->num_baboons);
        }
        sleep(1);
        if(canyonrope->num_baboons > 0){
            printf("Baboon %d just got across on right\n", canyonrope->num_baboons);
            canyonrope->num_baboons = canyonrope->num_baboons - 1;
        }
        sem_post(&rope);
        sem_post(&right_mutex);

    }while(canyonrope->iter > 0);
}

void *right_to_left(){
    do{
        sem_wait(&right_mutex);
        sem_wait(&rope);
        canyonrope->direction = 0;
        if(canyonrope->num_baboons < canyonrope->capacity){
            canyonrope->num_baboons = canyonrope->num_baboons + 1;
            printf("Baboon %d is going left\n", canyonrope->num_baboons);
        }
        sleep(1);
        if(canyonrope->num_baboons > 0){
            printf("Baboon %d just got across on left\n", canyonrope->num_baboons);
            canyonrope->num_baboons = canyonrope->num_baboons - 1;
        }
        sem_post(&rope);
        sem_post(&left_mutex);
    }while(canyonrope->iter > 0);

}

int main(int argc, char **argv){ 

    FILE *file;
    int timetocross = 0;
    char direction;
    int lcount = 0;
    int rcount = 0;
    printf("ARGC: %d\n", argc);
    if(argc < 3){
        printf("Please enter a file name followed by an integer 1-10 which represents the time taken to cross the rope\n");
        return -1;
    }
    printf("ARGV1: %s\nARGV2: %s\n",argv[1], argv[2]);
    file = fopen(argv[1], "r");
    timetocross = atoi(argv[2]);    
    printf("Time to cross: %d\n", timetocross);
    
    while((direction = getc(file) != EOF)){
        printf("Direction = %c\n", direction);
        if(direction == ',') direction = getc(file);
        if(direction == 'L') lcount = lcount + 1;
        if(direction == 'R') rcount = rcount + 1;
    }

    printf("LCount : %d\nRCount : %d\n",lcount, rcount);

    // sem_init(&rope, 0, 1);
    // sem_init(&right_mutex, 0, 1);
    // sem_init(&left_mutex, 0 , 1);
    // sem_init(&deadlock_protection, 0, 1);
    // sem_init(&counter, 0 , 1);

    // //Set up variables needed for threads
    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    // pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    // pthread_t tid1[1];   
    // pthread_t tid2[1];     

    // //Create threads to run producer and consumer functions
    // pthread_create(&tid1[1], &attr, right_to_left, NULL);
    // pthread_create(&tid2[1], &attr, left_to_right, NULL);
    // //Wait for threads to finish
    // pthread_join(tid1[1], NULL);    
    // pthread_join(tid2[1], NULL);
    // //Destroy semaphores after use
    // sem_destroy(&rope);
    // sem_destroy(&right_mutex);
    // sem_destroy(&left_mutex);
    // sem_destroy(&deadlock_protection);
    // sem_destroy(&counter);
    return 0;
}