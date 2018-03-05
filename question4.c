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
    int buf_used = 0;
    int n=0;
    while(n<NUMBERS){
        sleep(1);
        if(buf_used<=5){
            sem_wait(&sem);
            if(buffer[buf_used]==0){
                printf("Produced %d\n", numbers[n]);
                buffer[buf_used] = numbers[n];
                n++;
            }
            else{
            buf_used++;
            }
            sem_post(&sem);
        }
        else{
            buf_used = 0;
        }
        
    }
    return 0;
}
void *consumer(){
    int buf_used = 0;
    int n=0;
    while(n<NUMBERS){
        sleep(1);
        if(buf_used<=5){
            sem_wait(&sem);
            if(buffer[buf_used]!=0){
                printf("Consumed %d\n", buffer[buf_used]);
                buffer[buf_used] = 0;
                n++;
            }
            else{
                buf_used++;
            }
            sem_post(&sem);
        }
        else{
            buf_used = 0;
        }
        
    }
}
int main(void)
{
    pthread_t thrs[2];
    sem_init(&sem, 0,1);
    printf("please enter 10 numbers: \n");
    
    for(int i = 0; i < NUMBERS; i++){
        scanf("%d", &numbers[i]);
    }

    for(int i =0; i<2; i++){
        if(i==0){
            pthread_create(&thrs[i],0,producer, (int *) numbers);
        }
        else{
            pthread_create(&thrs[i], 0, consumer,(void *) NULL);
        }
    }

    sem_destroy(&sem);

    for(int i=0; i<2; i++){
        pthread_join(thrs[i], NULL);
    }

    for(int i=0; i<5; i++){
        printf("%d\n", buffer[i]);
    }
}