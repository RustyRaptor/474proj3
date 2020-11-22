#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

////Create semaphores////
sem_t rope; // The rope sempahore ensures only one side has the rope at a time
sem_t right_to_left_mutex; // The right_to_left_mutex ensures that the queue
        // stays in order while crossing right to left
sem_t left_to_right_mutex; // The left_to_right_mutex ensures that the queue
        // stays in order while crossing left to right
sem_t mutex; // The mutex semaphore prevents deadlock
sem_t counter; // Ensure no more than 3 baboons are on the rope at any time

int left = 0;
int right = 0;
int cross_time; // Time to wait while baboons cross

void *left_to_right(void *baboonnum ) {
        //int *amountptr = (int*) time;
        int *num = (int*) baboonnum;
        //int amount = *amountptr;
        int numonrope;
        sem_wait( &mutex );
        sem_wait( &left_to_right_mutex );
        left++;
        if ( left == 1 ) {
                sem_wait( &rope );
                //printf( "Baboon number %d is waiting to cross from left to right\n", num);
        }
        printf( "Baboon number %d is waiting to cross from left to right\n", num);
        sem_post( &left_to_right_mutex );
        sem_post( &mutex );
        sem_wait( &counter );
        sem_getvalue( &counter, &numonrope );
        printf( "Baboon#%d has begun crossing, there are %d baboons on the rope going from left to right\n\n",
                num, 3 - numonrope );
        sleep( cross_time );
        sem_getvalue( &counter, &numonrope );
        printf( "Baboon#%d just finished crossing left to right, there are %d on the "
                "rope\n\n",
                num, 2 - numonrope);
        sem_post( &counter );
        sem_wait( &left_to_right_mutex );
        left--;
        if ( left == 0 )
                sem_post( &rope );
        sem_post( &left_to_right_mutex );
        pthread_exit( NULL );
}

void *right_to_left( void *baboonnum ) {
        //int *amountptr = (int*) time;
        int *num = (int*) baboonnum;
        //int amount = *amountptr;
        int numonrope;
        sem_wait( &mutex );
        sem_wait( &right_to_left_mutex );
        right++;
        //This ensures that the rope is on this side
        if ( right == 1 ) {
                sem_wait( &rope );
                //printf( "Baboon number %d is waiting to cross from right to left\n\n", num );
        }
        printf( "Baboon number %d is waiting to cross from right to left\n\n", num );
        sem_post( &right_to_left_mutex );
        sem_post( &mutex );
        sem_wait( &counter );
        sem_getvalue( &counter, &numonrope );
        printf( "Baboon#%d has begun crossing, there are %d baboons on the rope going from right to left\n\n",
                num, 3 - numonrope );
        sleep( cross_time );
        sem_getvalue( &counter, &numonrope );
        printf( "Baboon#%d just finished crossing right to left, there are %d on the "
                "rope\n\n",
                num, 2 - numonrope);
        sem_post( &counter );
        sem_wait( &right_to_left_mutex );
        right--;
        if ( right == 0 )
                sem_post( &rope );
        sem_post( &right_to_left_mutex );
        pthread_exit( NULL );
}

int main( int argc, char **argv ) {
        FILE *file; // File to read from
        int *timetocross = ( int * )malloc(
                sizeof( int ) ); // Used to set time taken for baboons to cross
        char baboons[100]; // Used to hold queue of baboons
        int babooncount = 0;
        int lcount = 0; // Used to determine how many baboons are going in each
                // direction in the queue
        int rcount = 0;
        char direction;
        // Check that arguments are provided
        if ( argc < 3 ) {
                printf( "Please enter a file name followed by an integer 1-10 which "
                        "represents the time taken to cross the rope\n" );
                free( timetocross );
                return -1;
        }
        file = fopen( argv[1], "r" ); // Open file to read order of baboons from
        int gettime = atoi( argv[2] );
        cross_time = gettime;
        *timetocross = gettime; // Set time to cross variable

        // Fill the queue of baboons based on the order provided from the file
        while ( ( fscanf( file, "%c", &direction ) != EOF ) ) {
                if ( direction == ',' )
                        continue;
                if ( direction == 'L' ) {
                        lcount = lcount + 1;
                        baboons[babooncount] = 'L';
                        babooncount = babooncount + 1;
                }
                if ( direction == 'R' ) {
                        rcount = rcount + 1;
                        baboons[babooncount] = 'R';
                        babooncount = babooncount + 1;
                }
        }
        // Print how many baboons are on each side to cross
        printf("\nThere are %d baboons waiting to cross from left to right"
           " and %d baboons waiting to cross from right to left\n\n",lcount, rcount);
        printf("The order of baboons is: ");
        for(int i = 0; i < babooncount; i++){
            printf("%c ", baboons[i]);
        }
        printf("\n\nIt will take each baboon %d seconds to cross\n\n", cross_time);
        // Initialize correct number of threads for each side
        pthread_t l_to_r[lcount];
        int left_thread_count = 0;
        pthread_t r_to_l[rcount];
        int right_thread_count = 0;
        // Initialize semaphores
        sem_init( &rope, 0, 1 );
        sem_init( &right_to_left_mutex, 0, 1 );
        sem_init( &left_to_right_mutex, 0, 1 );
        sem_init( &mutex, 0, 1 );
        sem_init( &counter, 0, 3 );
        // Create threads based on order of queue
        for ( int i = 0; i < babooncount; i++ ) {
                if ( baboons[i] == 'L' ) {
                        // printf("Left baboon\n");
                        pthread_create( &l_to_r[left_thread_count], NULL,
                                        left_to_right, (void *) i);
                        left_thread_count++;
                } else if ( baboons[i] == 'R' ) {
                        // printf("Right baboon\n");
                        pthread_create( &r_to_l[right_thread_count], NULL,
                                        right_to_left, (void *) i);
                        right_thread_count++;
                }
        }
        // Wait for threads to finish
        for ( int j = 0; j < left_thread_count; j++ ) {
                pthread_join( l_to_r[j], NULL );
        }
        for ( int j = 0; j < right_thread_count; j++ ) {
                pthread_join( r_to_l[j], NULL );
        }
        free( timetocross );
        // Destroy semaphores
        sem_destroy( &rope );
        sem_destroy( &right_to_left_mutex );
        sem_destroy( &left_to_right_mutex );
        sem_destroy( &mutex );
        sem_destroy( &counter );
        return 0;
}
