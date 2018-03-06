#define _XOPEN_SOURCE 700 // required for barriers to work
#define NUMBERS 10

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>


int buffer[5] = {0};
int numbers[NUMBERS];
sem_t sem;

void *producer(int numbers[]){
    int buf_used = 0; // Counter to hold how many spots in the buffer are used
    int n=0; // counter for loop
    while(n<NUMBERS){ // while counter is less than amount of numbers
        sleep(1); // delay for random time
        if(buf_used<=5){ // if buffer is not full
            sem_wait(&sem); // lock thread
            if(buffer[buf_used]==0){ // if current element is 0 aka unused
                printf("Produced %d\n", numbers[n]); // print that it produced 
                buffer[buf_used] = numbers[n]; // fill the spot in the buffer
                n++; // incremement
            }
            else{ // if it is used, skip it
            buf_used++;
            }
            sem_post(&sem); // unlock thread
        }
        else{ // if buffer is full, reset counter
            buf_used = 0;
        }
        
    }
    return 0;
}
void *consumer(){
    int buf_used = 0; // counter to hold how many values buffer holds
    int n=0; // loop counter
    while(n<NUMBERS){ // while n is less than total numbers
        sleep(1); // delay for random time
        if(buf_used<=5){ // if buffer is not full
            sem_wait(&sem); // lock access to buffer
            if(buffer[buf_used]!=0){ // if spot in buffer is used
                printf("Consumed %d\n", buffer[buf_used]); // print that it is consumed
                buffer[buf_used] = 0; // set element back to zero
                n++; // increment
            }
            else{ // if it is not used yet
                buf_used++; //incremenet
            }
            sem_post(&sem); // unlock access to buffer
        }
        else{
            buf_used = 0; // if it is full reset counter
        }
        
    }
}
int main(void)
{
    pthread_t thrs[2]; //array for threads
    sem_init(&sem, 0,1); // initialize semaphore
    printf("please enter 10 numbers: \n"); // prompt for user input
    
    for(int i = 0; i < NUMBERS; i++){ // while less than total numbers
        scanf("%d", &numbers[i]); // each input is entered into the numbers array
    }

    for(int i =0; i<2; i++){ // create threads
        if(i==0){ // thread one will be producer
            pthread_create(&thrs[i],0,producer, (int *) numbers);
        }
        else{ // thread two will be consumer
            pthread_create(&thrs[i], 0, consumer,(void *) NULL);
        }
    }

    sem_destroy(&sem); // release semaphores

    for(int i=0; i<2; i++){ // rejoin threads so accessed resources can be used
        pthread_join(thrs[i], NULL);
    }

    for(int i=0; i<5; i++){ // print out all values in buffer
        printf("%d\n", buffer[i]);
    }
}