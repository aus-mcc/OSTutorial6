#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
int main(void)
{
    int pid; // create variable to hold process id

    pid = fork(); // fork to create child process

    if(pid == -1){ // if there is an error with forking
        printf("Unable to fork"); // print error message
        exit(EXIT_FAILURE); // exit program
    }
    else if(pid == 0){
        // Child Process
        sleep(1); // sleep for a second
        printf("Child Process\n"); // print message
        exit(1); // exit process
    }
    else{
        int status; // variable to hold status
        if(wait(&status)){ // wait until child has exited
            printf("Parent Process\n"); // print message
        }
    }
    return 0;
}