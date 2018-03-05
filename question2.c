#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
int main(void)
{
    int pid;

    pid = fork();

    if(pid == -1){
        printf("Unable to fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        // Child Process
        sleep(1);
        printf("Child Process\n");
        exit(0);
    }
    else{
        wait(NULL);
        printf("Parent Process\n");
    }
    return 0;
}