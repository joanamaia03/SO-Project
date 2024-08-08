#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    if (argc != 4 && argc != 5) {
        printf("ERROR incorrect number of parameters");
        return -1;

    }
    int n_cycles;
    if (argc == 4){
        n_cycles = 4000;
    }
    else{
        n_cycles = atoi(argv[4]);
    }
    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    float t = atof(argv[3]);
    int token = 0;
    pid_t pid = 1;
    char craetorNamedPipe[BUFFER_SIZE];
    char namedPipe[BUFFER_SIZE];
    int pidx;
    int fd;

    for(int i = 1; i <= n; i++){
        if(pid > 0){
            pidx = i;
            pid = fork();
        }
        if(pid > 0){
            if(pidx != n)
                snprintf(craetorNamedPipe, BUFFER_SIZE, "pipe%dto%d", i, i+1);
            else
                snprintf(craetorNamedPipe, BUFFER_SIZE, "pipe%dto%d", i, 1);
            mkfifo(craetorNamedPipe, 0777);
        }
    }
    if (pid == 0){
        printf("in child process %d\n", pidx);
        if (pidx==1){ 
            printf("tokenring started on p1\n");
            snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d",1,2);
            fd = open(namedPipe, O_WRONLY);
            write(fd, &token, sizeof(int));
            close(fd);
        }
        while(token != n_cycles){
            if (pidx == 1){
                //process #1 first process
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", n, pidx);
                fd = open(namedPipe, O_RDONLY);
                //waiting for token to arrive
                while(read(fd, &token, sizeof(int)) == 0);
                close(fd);
                //run the probability
                float rng = (float)(random()%100) /100;
                if(rng <= p){
                    printf("[p%d] lock on token (val = %d)\n", pidx, token);
                    sleep(t);
                    printf("[p%d] unlock token\n", pidx);
                }
                token++;
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", pidx, pidx+1);
                fd = open(namedPipe, O_WRONLY);
                write(fd, &token, sizeof(int));
                close(fd);
            }
            else if(pidx == n){
                //process #2 last process
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", pidx-1, pidx);
                fd = open(namedPipe, O_RDONLY);
                //waiting for token to arrive
                while(read(fd, &token, sizeof(int)) == 0);
                close(fd);
                //run the probability
                float rng = (float)(random()%100) /100;
                if(rng <= p){
                    printf("[p%d] lock on token (val = %d)\n", pidx, token);
                    sleep(t);
                    printf("[p%d] unlock token\n", pidx);
                }
                token++;
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", pidx, 1);
                fd = open(namedPipe, O_WRONLY);
                write(fd, &token, sizeof(int));
                close(fd);
            }
            else{
                //process #3 middle of ring processes
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", pidx, pidx+1);
                fd = open(namedPipe, O_RDONLY);
                //waiting for token to arrive
                while(read(fd, &token, sizeof(int)) == 0);
                close(fd);
                //run the probability
                float rng = (float)(random()%100) /100;
                if(rng <= p){
                    printf("[p%d] lock on token (val = %d)\n", pidx, token);
                    sleep(t);
                    printf("[p%d] unlock token\n", pidx);
                }
                token++;
                snprintf(namedPipe, BUFFER_SIZE, "pipe%dto%d", pidx, pidx+1);
                fd = open(namedPipe, O_WRONLY);
                write(fd, &token, sizeof(int));
                close(fd);
            }
        }
    }
    
}