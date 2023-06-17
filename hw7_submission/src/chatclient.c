#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

int client_socket;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];
bool user = false;
//user id = 1116, 16116

int handle_stdin() {

    char temp[MAX_MSG_LEN+1];
    char c;
    int i = 0;  
    while ((c = getc(stdin)) != '\n' && i < sizeof(temp) - 1) {
        temp[i++] = c;
    }
    if (c == EOF){ // checking for EOF
        return 3;
    }
    temp[i] = '\0';
            
    int len = strlen(temp);
           
    if (temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    } 
    memcpy(outbuf, temp, len + 1); // +1 to include the null terminator
	    
	    
    //check for length of stdin, if not too long, send to server
    if (strlen(outbuf) > MAX_MSG_LEN) {
        fprintf(stderr, "Max username limit of %d characters surpassed.\n", MAX_NAME_LEN);
    } else {
        if (send(client_socket, outbuf, strlen(outbuf)+1, 0) < 0) {
            perror("send");
        }	
    }

    if(!strcmp(outbuf, "bye")){
        printf("Goodbye.\n");
        fflush(stdout);

        return 2;
    }
    
    return EXIT_SUCCESS;
}
/*
int handle_client_socket() {
    
}*/

int print_username(char *username){
    printf("[%s]: ", username);
    fflush(stdout);
    
    return EXIT_SUCCESS;
}



int main(int argc, char **argv) {
    char *ip_serv;
    int port_number;
    //int sock;
    struct sockaddr_in server;


    if (argc!=3){
        fprintf(stderr,"Usage: %s <server IP> <port>\n",argv[0]);
        return EXIT_FAILURE;
    }

    // Parse IP address
    //AF_INET is used to specify PIv4, store conversion in ip_serv
    if (inet_pton(AF_INET, argv[1], &ip_serv) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Parse port number
    port_number = atoi(argv[2]);
    if (port_number < 1024 || port_number > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port_number);
        return EXIT_FAILURE;
    }
    //run handle stdin


    while(1){
        memset(username, 0, MAX_NAME_LEN);
        printf("Enter a username: ");
        int count = 0;
        char c;
        
        while ((c = getc(stdin)) != '\n') {
            username[count++] = c; // POSSIBLE OVERFLOW ERROR.
        }

        if (count > MAX_NAME_LEN){
            puts("Sorry, limit your username to %d characters.");
            continue;
        } else {
            break;
        }
    }

    printf("Hello, %s. Let's try to connect to the server.\n",username);
    
    //Creating TCP Socket with error message
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    //TCP Client Server Connection
    //Code taken from slides
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port_number);
    if (connect(client_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        fprintf(stderr, "Error connecting to server: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (recv(client_socket, inbuf, BUFLEN, 0)==0){
        fprintf(stderr, "recv error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("\n %s \n \n", inbuf);
    fflush(stdout);
    
    //Send username to server
    
    for(int i=0; i<strlen(username); i++){
        outbuf[i] = username[i];
    }
    outbuf[strlen(username)] = '\0';
   
    if(send(client_socket, outbuf, strlen(username)+1, 0)<0){
        fprintf(stderr, "send error: %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    //part 4
    //handle sets for client management
    fd_set active_fd_set, read_fd_set; // COMMENT OUT LATER, USE 1 SET
    
    
    //main loop
    while(1){

        if (isatty(STDIN_FILENO)) {
            print_username(username);
        }

        active_fd_set=read_fd_set;
        FD_ZERO(&active_fd_set);
        FD_SET(STDIN_FILENO, &active_fd_set);
        FD_SET(client_socket, &active_fd_set);
        read_fd_set = active_fd_set;

       // need to adress redirected case
    
         
    

	//block until inputs arrived/ decide what do do next
	if(select(client_socket+1,&read_fd_set, NULL, NULL, NULL)<0){
	    perror("select");
	    exit(EXIT_FAILURE);
	}

        // Check for user input
	// missing test for piped files
        if (FD_ISSET(STDIN_FILENO, &read_fd_set)) {
            int flag = handle_stdin();
            
            if (flag == 2 || flag == 3 ){
                return EXIT_SUCCESS;
            }
	        
        }
	// Check for incoming message from server
	    
        if (FD_ISSET(client_socket, &read_fd_set)) {
            int num_bytes=0;
            if ((num_bytes = recv(client_socket, inbuf, sizeof(inbuf), 0)) < 0 && errno != EINTR) {
                printf(" Warning: Failed to receive incoming message.");
            } else if (num_bytes == 0|| strcmp(inbuf,"bye")==0) {
                // Connection closed by server
                printf("Server closed the connection.\n");
                break;
            } else {
                inbuf[num_bytes] = '\0';
                printf("\n%s\n", inbuf);
                fflush(stdout);
            }
        }
    }
    //Close Socket
    close(client_socket);
    return EXIT_SUCCESS; 

}
