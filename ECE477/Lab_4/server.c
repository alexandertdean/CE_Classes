/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void handle_args(int argc, char *argv[], int *portno, char **execute); // Handles Arguments

int main(int argc, char *argv[])
{
    // Initialize Variables
    int sock, connected, bytes_recieved , true = 1;  
    char send_data [1024] , recv_data[1024];       
    struct sockaddr_in server_addr,client_addr;    
    int sin_size;
    int *portno = (int *)malloc(sizeof(int));
    char *execute = (char *)malloc(sizeof(char));
    *portno = 2048;
    execute = "a.out";
    int my_pid, parent_pid, child_pid;
    // Handle Arguments
    handle_args(argc, argv, portno, &execute);
    // Set up server with error checking
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(*portno);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1) {
        perror("Unable to bind");
        exit(1);
    }
    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }
    printf("\nTCPServer Waiting for client on port %d \n",*portno);
    fflush(stdout);
    while(1){  
        sin_size = sizeof(struct sockaddr_in);
        connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
        printf("\nI got a connection from (%s , %d)", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        if((my_pid = fork())<0){
            perror("fork failure");
            exit(1);
        }
        printf("   PID Number: %d\n", my_pid);
        if(my_pid == 0){
            close(sock);
            // Could not get dup2 to work
            //dup2(connected,0);
            //dup2(connected,1);
            execl(execute, execute, 0, (char *)0);
        }
        /*while (1){
            //execl(execute, execute, 0, (char *)0);
            /*printf("\n SEND (q or Q to quit) : ");
            scanf("%s",send_data);
            if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0){
                send(connected, send_data,strlen(send_data), 0); 
                close(connected);
                break;
            }               
            else
                send(connected, send_data,strlen(send_data), 0);  
            bytes_recieved = recv(connected,recv_data,1024,0);
            recv_data[bytes_recieved] = '\0';
            if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0){
                close(connected);
                break;
            }
            else 
                printf("\n RECIEVED DATA = %s " , recv_data);
            fflush(stdout);*/
        //}
    }       
    close(sock);
    return 0;
} 
// Function to handle the arguments passed to client program
void handle_args(int argc, char *argv[], int *portno, char **execute)
{
    // Initialize Variables
    int i = 0;
    // Arguments handling
    for(i=0;i<argc-1;i++){
        if(strcmp(argv[i],"-p")==0){
            if(atoi(argv[i+1])>=1024 && atoi(argv[i+1])<=65536){
                *portno = atoi(argv[i+1]);
                printf("Port number entered: %d \n", *portno);
            }
            else{
                printf("Port number out of range 1024 - 65536\n");
                printf("Default port is 2048\n");
            }
        }
        if(strcmp(argv[i],"-s")==0){
            *execute = argv[i+1];
            printf("Executable File entered: %s \n", *execute);
          }
    }
	printf("handle_args done");
}
