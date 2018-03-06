#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int total_sum, status;
sem_t sem;

void* factorial_calc(int number){
    int factorial;
    for(int i=1;i<number;i++){
        factorial *= i;
    }
    sem_wait(&sem);
    total_sum += factorial;
    sem_post(&sem);
}

int main(void)
{
    FILE *fp;
    int NUMBERS = 5;
    
    int input[NUMBERS];
    printf("Please enter 5 numbers to write to file.\n");
    
    for(int i=0; i<NUMBERS; i++){
        scanf("%d", &input[i]);
    }

    fp = fopen("numbers.txt", "w");
    for(int b=0; b<NUMBERS; b++){
        fprintf(fp, "%d", input[NUMBERS]);
    }
    fclose(fp);

    pid_t pid1 = 0;

    pid1 = fork();

    if(pid1==-1){
        printf("Error creating process");
        return 0;
    }

    sem_init(&sem, 0, 1);

    if(pid1==0){

        int numEntered[NUMBERS];

        FILE *fp = fopen("numbers.txt", "r");

        for(int a = 0; a<NUMBERS; a++){
            fscanf(fp,"%d\n", &numEntered[a]);
        }
        
        fclose(fp);

        pthread_t tid[NUMBERS];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        for(int i=0; i<NUMBERS; i++){
            pthread_create(&tid[i],NULL,factorial_calc,numEntered[i]);
        }

        for(int j=0; j<NUMBERS; j++){
            pthread_join(tid[j],0);
        }

        FILE *fp2; 
        fp2 = fopen("sum.txt","w");
        fprintf(fp2, "%d", total_sum);
        fclose(fp2);
        return 0;

    }

    else{
        waitpid(pid1,&status,0);
        int printSum;
        if(status==0){
            FILE *fp;
            fp = fopen("sum.txt","r");
            fscanf(fp, "%d", &printSum);
            printf("Total Sum: %d", printSum);
            fclose(fp);
        }
    }

    return 0;
}