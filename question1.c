#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
int main(void)
{
    //File pointers for each file (child1.txt and child2.txt)
    FILE *file1;
    FILE *file2;

    
    pid_t pid1 = 0;
    //Create child process
    pid1 = fork();

    //Not parent or child
    if(pid1 == -1){
        fprintf(stderr, "can't fork, error %d\n",errno);
        exit(EXIT_FAILURE);
    }
 
    //Child process
    if(pid1 == 0){
        char read[256];
        sleep(1);
        file1 = fopen("child1.txt", "r");
        fscanf(file1, "%s", read);
        printf("%s",read);
        return 0;
    }

    //Parent process
    else{

        //Creates second child process
        pid_t pid2 = fork();
        
        //Neither parent or child
        if(pid2 == -1){
               fprintf(stderr, "can't fork, error %d\n",errno);
                exit(EXIT_FAILURE);
        }

         //Second child process
         if(pid2 == 0){
            char read2[256];
            sleep(1);
            file2 = fopen("child2.txt", "r");
            fscanf(file2, "%s", read2);
            printf("%s",read2);
           return 0;
        }

        //Parent - creates 2 text files and writes to it 
        else{

            //Opens each text file
            file1 = fopen("child1.txt","w");
            file2 = fopen("child2.txt","w");

            //If either are NULL, print error
            if(file1 == NULL|| file2 == NULL){
                printf("Error opening file\n");
                exit(1);
             }

            //Required words to write to file
            const char *text = "Child1";
            const char *text2 = "Child2";

            //Writes words above to respective file
            fprintf(file1, "%s", text);
            fprintf(file2, "%s", text2);
        }
    }
    return 0;
}
