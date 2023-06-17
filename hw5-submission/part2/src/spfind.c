#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(int argc, char **argv) {
    // instantiate variables
    int fd_pfind[2];
    int fd_sort[2];
    pid_t pid_pfind;
    pid_t pid_sort;

    ssize_t current_read;
    char buf[BUF_SIZE];
    int matches = 0;

    // make pipes
    if (pipe(fd_pfind) < 0) {
        perror("first pipe error");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd_sort) < 0) {
        perror("second pipe error");
        exit(EXIT_FAILURE);
    }

    // fork first child (pfind)
    // pfind process will only run for fork, aka pid = 0
    if ((pid_pfind = fork()) < 0) {
        perror("first fork error");
        exit(EXIT_FAILURE);
    } else if (pid_pfind == 0) {
        // child process (pfind)
	 
	// (write pipe)
	if(close(fd_pfind[0])<0){
            perror("first close in pfind error");
            exit(EXIT_FAILURE);
        }// close read end of pipe from pfind to sort(not used)
        if(dup2(fd_pfind[1], STDOUT_FILENO)<0){
	    perror("pipe redirect out of pfind to sort");
	    exit(EXIT_FAILURE);
	} // redirect stdout to pipe
	if(close(fd_pfind[1])<0){
            perror("second close in pfind error");
            exit(EXIT_FAILURE);
        }//close write end of pipe after using it
					
        //get arguments from pfind from arguments of spfind, but change the executable string
	argv[0] = "./pfind";
        //check for execv error
        if (execv("./pfind", argv) < 0) {
            perror("pfind execv error");
            fprintf(stderr, "Error: pfind failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fork second child (sort)
    // sort process will only run for forked child, aka pid = 0
    // child of parent without pfind will be sorting nothing, so no outputs will be passed, making no difference to the program
    if ((pid_sort = fork()) < 0) {
        perror("second fork error");
        exit(EXIT_FAILURE);
    } else if (pid_sort == 0) {
        // child process (sort)
	
	// pfind: read pipe
	if(close(fd_pfind[1])<0){
	    perror("first close in sort error");
	    exit(EXIT_FAILURE);
	}// close write end of pipe from pfind to sort(not used)
        if(dup2(fd_pfind[0], STDIN_FILENO)<0){
	    perror("pfind to sort pipe error");
	    exit(EXIT_FAILURE);
	} // redirect stdin from pipe
	if(close(fd_pfind[0])<0){
            perror("second close in sort error");
            exit(EXIT_FAILURE);
        }//close read end of pfind pipe after reading is done

	//sort: write pipe
	if(close(fd_sort[0])<0){
            perror("third close in sort error");
            exit(EXIT_FAILURE);
        }// close read end of pipe from sort to main(not used)
        if(dup2(fd_sort[1], STDOUT_FILENO)<0){
	    perror("sort to parent pipe error");
	} // redirect stdout to pipe
	if(close(fd_sort[1])<0){
            perror("fourth close in sort error");
            exit(EXIT_FAILURE);
        }//close write end of sort pipe after writing is done
        
	// make arguments array for sort executable
        char *args_sort[3] = {"sort", NULL, NULL};
        if (execvp("sort", args_sort) < 0) {
            perror("sort execvp error");
            fprintf(stderr, "Error: sort failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    // these pipe actions only apply to parent, as they have been dealt with by children
   
    // pipe between pfind child and parent are not used, so just close both here
    if(close(fd_pfind[0])<0){
            perror("first close in parent error");
            exit(EXIT_FAILURE);
    }
    if(close(fd_pfind[1])<0){
            perror("second close in parent error");
            exit(EXIT_FAILURE);
    }
    // close write end from sort to main(not used)
    if(close(fd_sort[1])<0){
            perror("third close in parent error");
            exit(EXIT_FAILURE);
    }

    //output sorted, resulting files and number of matches
    while ((current_read = read(fd_sort[0], buf, BUF_SIZE)) > 0) {
        //buf contains string printed by pfind and sorted by sort
	//to count number of invidiual files, used the newline character present between each file
        const char* const_buf = (const char*)buf; 
        while (*const_buf != '\0') {
            if (*const_buf == '\n') {
                matches++;
            }
            const_buf++;
        }
	//take off extra newline character
        matches--;

        //write each match read to output to be printed out by the program
        //size to pass is stored by current read, so thats the input in write
        write(STDOUT_FILENO, buf, current_read);
    }
    
    //handle reading from sort output errors
    if (current_read < 0) {
        perror("reading number of files error");
        exit(EXIT_FAILURE);
    }

    //parent wait for both children to finish
    //error handling for both waitpid's
    if (waitpid(pid_pfind, NULL, 0) < 0) {
        perror("pfind waitpid");
        exit(EXIT_FAILURE);
    }
    if (waitpid(pid_sort, NULL, 0) < 0) {
        perror("sort waitpid");
        exit(EXIT_FAILURE);
    }

    //print the number of files found
    printf("Total Matches: %d\n", matches);
    

    return EXIT_SUCCESS;
}

