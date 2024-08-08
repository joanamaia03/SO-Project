#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    pid_t processid;
    char *args[argc + 1];
    args[0] = "zip";
    args[1]="ebooks.zip";
    for(int i=1; i<argc; i++){
        char *argumentCopy = (char *) malloc(strlen(argv[i]+1));
        strcpy(argumentCopy, argv[i]);
        char *fileNameTxt = argv[i];
        char *fileNameEpub = strtok(argumentCopy, ".");
        strcat(fileNameEpub, ".epub");
        if((processid = fork())<0){
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        else if(processid == 0){
            char *pandocArgs[] = {"pandoc",fileNameTxt, "-o", fileNameEpub,NULL};
            printf("%s being converted by [%d]\n",fileNameTxt, getpid());
            execvp("pandoc",pandocArgs);
        }
        else if(processid >0){
            int status;
            if(wait(&status)<0){
                fprintf(stderr, "can't wait for child: %s\n", strerror(errno));
            }
            args[i+1]=fileNameEpub;
        }
    }
    if(processid>0){
        args[argc +1]=NULL;
        if(execvp("zip",args)==-1){
            perror("execvp error");
            return EXIT_FAILURE;
        }
    }
    exit(EXIT_SUCCESS);
}
