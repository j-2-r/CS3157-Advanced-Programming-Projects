#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <assert.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT    "\x1b[0m"


// FFLUSH BUFFER, MULTIPLE EXIT CALLS REQUIRED SOMETIMES, POSSIBLY MAKE NEW BUFFER FOR LINE_INPUT????

volatile sig_atomic_t interrupted = 0;

void catch_signal(int sig) {
    interrupted = 1;
    //printf("CATCH_SIG TEST!!\n\n");
}


char ** args_maker(char *line_input){
    char **args = malloc(2048 * sizeof(char*));
    if ( args == NULL){
        printf("Error: malloc() failed. %s.\n", strerror(errno));
    }
    char *token;
    for(int i = 0; i<2049; i++){
        args[i] = malloc(4096 * sizeof(char));
    }
    
    token = strtok(line_input, " ");


    int j = 0;
    while(token != NULL){
        //printf("WHILE TOK: %s\n", token); //test

        args[j] = token;

        //printf("ARGS_PRINT: %s\n", args[j]); //test
        token = strtok(NULL, " ");
        ++j;

    }
    args[j]= NULL;
    return args;
}


void free_args(char **args){
    int j = 0;
    for (int j = 0; j < 2049; j++){
        free(args[j]);
    }
    printf("%d",j);
    free(args);
}

int args_size(char **args){
    int i = 0;
    while (args[i] != NULL){
        i++;
    } 
    return i;
}






int main (int argc, char * argv[]){
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = catch_signal;
    //action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(SIGINT, &action, NULL) == -1) {
            perror("sigaction(SIGINT)");
            return EXIT_FAILURE;
        }



    while (true){
        if (interrupted == 1){
            printf("INTERRUPT TEST \n");//test
            interrupted = 0;
            continue;
        }
        char current_path[PATH_MAX];
        char line_input[128];

        if (getcwd(current_path, PATH_MAX +1) == NULL && errno != 0){  // NEEDS TO BE FREED; MIGHT HAVE TO USE AN EXIT_HANDLER
            printf("Error: Cannot get current working directory. %s.\n", strerror(errno));
        };

        printf("%s", BRIGHTBLUE);
        printf("[%s] ",current_path);
        printf("%s", DEFAULT);

        printf("$ ");
        fflush(stdout);

        
        if (fgets(line_input, 128, stdin)== NULL && ferror(stdin) != 0 && interrupted != 1){
            printf("Error: Failed to read from stdin. %s.\n", strerror(errno));
            return EXIT_FAILURE;

        }; 

        printf("%s", line_input);//test
        
        line_input[strcspn(line_input, "\n")] = '\0'; // found on STOV. Strips the \n, turns to eof.
        
        char **args = args_maker(line_input);

        printf("ARGS_SIZE TESTER: %d \n", args_size(args));//test
        
        if (!strcmp(args[0], "exit")){ // we could use strncmp here?
            return EXIT_SUCCESS;



          //CD        
        } else if (!strcmp(args[0], "cd")){ //  NEED TO GET ARGUMENT CHECKER, MIGHT NEED TO PUT ARGS UP TOP.
            if (args_size(args) > 2){
                printf("Error: Too many arguments to cd.\n");
                continue;
            }

            puts("TEST!"); 
        
            //CD to HOME
            if (args_size(args) == 1 || !strcmp(args[1], "~")){

                
                
                puts("TEST_CHDIR");// TEST

                errno = 0;
                struct passwd *pword = getpwuid(getuid());
                if ( pword == NULL && errno != 0){
                    printf("Error: Cannot get passwd entry. %s.\n", strerror(errno));
                    return EXIT_FAILURE;
                }
                char *home_dir = pword->pw_dir;

                if (chdir(home_dir) != 0){
                    printf("Change directory failed. %s", strerror(errno));
                }
            } else {
               
                if (chdir(args[1]) != 0){
                    printf("Change to directory '%s' failed. %s.\n",args[1], strerror(errno)); 
                }
                
            }
            
    
        } else { // FORKING AND EXECING
            
            pid_t pid;
            if ((pid = fork()) < 0) {
                fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
                return EXIT_FAILURE;
            } else if (pid > 0) {
                // We're in the parent.
                // pid is the process id of the child.
                //printf("Child PID: %ld\n", (long)pid);
                int status;
                pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED); //TODO
                if (w == -1 && interrupted == 0) {
                    // waitpid failed.
                    printf("Error: wait() failed. %s.\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                //free_args(args);

            } else {
                // We're in the child.

                // we need to construct char *argv[], to feed to execvp. Needs to null-terminate. Need to use token.
                

                if (interrupted ==0){
                    if (execvp(args[0], args) == -1) {
                        fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                        return EXIT_FAILURE;
                    }
                }

                return EXIT_SUCCESS;
                
                
               
            }// CHILD BLOCK 

        }//FORK_EXEC BLOCK

    }//end of infinite loop
    return 1;

}//main