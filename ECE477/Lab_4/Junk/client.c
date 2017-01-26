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

void printBoard(char **passedboard);
int checkCode(int code);

int main()
{
	int sock, bytes_recieved;  
    char *move;
	move  = malloc(2);
	char *board; 
	board = malloc(2);
	char *confirmCode;
	char serverCode[3];
    struct hostent *host;
    struct sockaddr_in server_addr;  
	int i;
	char recv_data[1024];
	for (i = 0; i < 9; i++) {
		board[i] = 'B';
	}
    host = gethostbyname("127.0.0.1");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(5000);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8); 

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
        perror("Connect");
        exit(1);
    }
    	bytes_recieved=recv(sock,recv_data,1024,0);
        recv_data[bytes_recieved] = '\0';
 
        if (strcmp(recv_data , "n") == 0 || strcmp(recv_data , "N") == 0)
        {
           close(sock);
           return 0;
        }
        else
           printf("\nReady ?");
	while(1)
    {	
		printf("________________________________________\n");
		printBoard(&board);
		printf("Move (q or Q to quit): ");
		scanf("%s", move);
        if (strcmp(move , "q") != 0 && strcmp(move , "Q") != 0)
        	send(sock,move,strlen(move), 0); 
        else
        {
           send(sock,move,strlen(move), 0);   
           close(sock);
           break;
        }

		  bytes_recieved=recv(sock,board,1024,0);
          board[bytes_recieved] = '\0';
 
          if (strcmp(board , "q") == 0 || strcmp(board , "Q") == 0)
          {
           close(sock);
           break;
          }

          else
           	printf("\nServer Move : %s " , board);
		/*if (checkCode(atoi(serverCode))) {
			*confirmCode = '1';
			send(sock, confirmCode, 1, 0);
			bytes_recieved = recv(sock, board, 2, 0);
		}
		else {
			printf("BREAk !\n");
			close(sock);
			free(move);
			break;
		}*/
    }   
	return 0;
}

void printBoard(char **passedboard)
{
	char *board = *passedboard;
	printf("%c | %c | %c\n", board[0], board[1], board[2]);
	printf("---------\n");
	printf("%c | %c | %c\n", board[3], board[4], board[5]);
	printf("---------\n");
	printf("%c | %c | %c\n", board[6], board[7], board[8]);
}

int checkCode(int code)
{
	if ((code / 100) != 4) {
		return 1;
	}
	return 0;
}
