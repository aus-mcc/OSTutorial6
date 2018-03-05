#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
int moving_sum[5] = {0};

typedef struct{
    int index;
    int number;
} n;
void *factorial(void *numb);
int calculate(int x);

void *factorial(void *numb){
    n r = *(n*) numb;
    int factorial = calculate(r.number);

    if(r.index == 0){
        factorial = calculate(r.number);
    }
    else{
        sem_wait(&sem);
        while(moving_sum[r.index -1] == 0){
            sem_post(&sem);
            sem_wait(&sem);
        }
        moving_sum[r.index] = moving_sum[r.index-1] + factorial;
        sem_post(&sem);
    }
    return NULL;
}

int calculate(int x){
    if(x <= 1){
        return 1;
    }
    else{
        return x*calculate(x-1);
    }
}

int main(void)
{
    int nums[5];
    n numbers[5];
    pthread_t thrs[5];

    sem_init(&sem, 0,1);

    for(int i=0; i<5; i++){
        printf("Please enter a number: ");
        scanf("%d", &nums[i]);
        numbers[i].index = i;
        numbers[i].number = nums[i];
    }

    for(int i=0;i<5;i++){
        pthread_create(&thrs[i], 0, factorial, (void *)&numbers[i]);
    }

    for(int i=0; i<5; i++){
        pthread_join(thrs[i], NULL);
    }

    for(int i=0; i<5; i++){
        printf("Index: %d, Value: %d\n", i, moving_sum[i]);
    }
}