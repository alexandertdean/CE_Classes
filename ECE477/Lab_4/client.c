/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int handle_args(int argc, char *argv[]); // Handles Arguments

int main(int argc, char *argv[])
{
    // Initialize Variables
    int sock, bytes_recieved, portno;  
    char send_data[1024],recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;  
    // Handle Arguments and set up connection to server based on argv[]
    portno = handle_args(argc, argv);
    host = gethostbyname("127.0.0.1");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(portno);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8); 
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
        perror("Connect");
        exit(1);
    }
    // Wait for a termination signal from server
    while(1){
        bytes_recieved=recv(sock,recv_data,1024,0);
        recv_data[bytes_recieved] = '\0';
        // Tell user to terminate program with q or Q
        printf("\n(q or Q to quit) : ");
        scanf("%s",send_data);   
        if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
        send(sock,send_data,strlen(send_data), 0); 
        else{
            send(sock,send_data,strlen(send_data), 0);   
            close(sock);
            break;
        }   
    }   
    return 0;
}
// Function to handle the arguments passed to client program
int handle_args(int argc, char *argv[])
{
    int i;
    int portno = 2048;
    // Arguments handling
    if(argc == 1){
        printf("Enter -p and the port number of server.\n");
        exit(0);
    }
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],"-p")==0){
            if(atoi(argv[i+1])>=1024 && atoi(argv[i+1])<=65536){
                portno = atoi(argv[i+1]);
                printf("Port number entered: %d \n", portno);
                return portno;
            }
            else{
                printf("Port number out of range 1024 - 65536\n");
                printf("Default port is 2048\n");
                return portno;
            }
        }
    }
}