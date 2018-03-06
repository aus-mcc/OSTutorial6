#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int total_sum, status;
sem_t sem;

//Calculates the factorial of the number entered as arguement
void* factorial_calc(int numb){
    int factorial = 1;

    for(int i=1;i<=numb;i++){
        factorial *= i;
    }

    printf("Factorial: %d\n", factorial);
    sem_wait(&sem);
    total_sum += factorial;
    sem_post(&sem);
}

int main(void)
{
    FILE *fp;
    //Amount of numbers asked to input
    int NUMBERS = 5;
    
    //Array for the numbers entered
    int enteredNum[NUMBERS];


    printf("Please enter 5 numbers to write to file.\n");
    
    for(int i=0; i<NUMBERS; i++){
        scanf("%d", &enteredNum[i]);
    }


    fp = fopen("numbers.txt", "w");
    for(int b=0; b<NUMBERS; b++){
        fprintf(fp, "%d", enteredNum[NUMBERS]);
    }
    fclose(fp);

    pid_t pid1 = 0;

    //Creates a child process
    pid1 = fork();

    //pid1 does not return 1 or 0 meaning it is not the child or parent
    if(pid1==-1){
        printf("Error creating process");
        return 0;
    }

    //Initalizes semaphore
    sem_init(&sem, 0, 1);

    //Child process
    if(pid1==0){

        //Array for all the numbers entered
        int numEntered[NUMBERS];

        //File pointer created to read the file and store inside numEntered array
        FILE *fp = fopen("numbers.txt", "r");
        for(int a = 0; a<NUMBERS; a++){
            fscanf(fp,"%d\n", &numEntered[a]);
        }
        fclose(fp);

        //Creates an array of threads
        pthread_t tid[NUMBERS];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        //Creates 5 threads
        for(int i=0; i<NUMBERS; i++){
            pthread_create(&tid[i],NULL,factorial_calc,numEntered[i]);
        }

        //Joins all the threads
        for(int j=0; j<NUMBERS; j++){
            pthread_join(tid[j],0);
        }

        //File pointer created to create/overwrite sum.txt with total
        FILE *fp2; 
        fp2 = fopen("sum.txt","w");
        fprintf(fp2, "%d", total_sum);
        fclose(fp2);
        return 0;

    }

    else{
        //Waits for child process to terminate
        waitpid(pid1,&status,0);
        int printSum;
        //Child process is terminated
        if(status==0){
            //File pointer created to read from the file and print the total sum
            FILE *fp;
            fp = fopen("sum.txt","r");
            fscanf(fp, "%d", &printSum);
            printf("Total Sum: %d", printSum);
            fclose(fp);
        }
    }

    return 0;
}