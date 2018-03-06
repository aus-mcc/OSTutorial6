#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
int moving_sum[5] = {0};

typedef struct n{
    int index;
    int number;
} n;
/*
void *factorial(void *numb);
int calculate(int x);
*/

void *factorial(void *numb){
    n r = *(n *) numb; // create an instance of the struct inputted
    int factorial = calculate(r.number); // create factorial value

    if(r.index == 0){ // if value is the first entered, the total is that value
        factorial = calculate(r.number);
    }
    else{
        sem_wait(&sem); // lock access to moving_sum
        while(moving_sum[r.index -1] == 0){ 
            sem_post(&sem);
            sem_wait(&sem);
        }
        moving_sum[r.index] = moving_sum[r.index-1] + factorial; // update moving sum with new factorial value
        sem_post(&sem); // unlock access to moving_sum
    }
    return NULL;
}
// Function to calculate the factorial
int calculate(int x){
    if(x <= 1){ // if input is 1, return 1
        return 1;
    }
    else{ // else recursively solve until input is 1
        return x*calculate(x-1);
    }
}

int main(void)
{
    int nums[5];
    n numbers[5]; // array of structs
    pthread_t thrs[5]; // create 5 threads

    sem_init(&sem, 0,1); // initialize semaphore

    // Take user input
    for(int i=0; i<5; i++){
        printf("Please enter a number: "); // prompt
        scanf("%d", &nums[i]); // take input and put into nums array
        numbers[i].index = i; // create struct values from inputted data
        numbers[i].number = nums[i];
    }

    // Create 5 threads that execute the factorial function
    for(int i=0;i<5;i++){
        pthread_create(&thrs[i], 0, factorial, (void *)&numbers[i]);
    }

    // Rejoin all threads so variables are updated and ready to be used
    for(int i=0; i<5; i++){
        pthread_join(thrs[i], NULL);
    }
    // Print out each value from the moving sum array
    for(int i=0; i<5; i++){
        printf("Index: %d, Value: %d\n", i, moving_sum[i]);
    }
}