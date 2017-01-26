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
	confirmCode = malloc(1);
	char serverCode[3];
        struct hostent *host;
        struct sockaddr_in server_addr;  
	int i;
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

        if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1) 
        {
            perror("Connect");
            exit(1);
        }

	while(1)
        {	
		fflush(stdout);	
		printf("________________________________________\n");
		fflush(stdout);
		printBoard(&board);
		printf("Move: ");
		fflush(stdout);
		scanf("%s", move);
		printf("Before send\n");
		send(sock, move, strlen(move), 0);
		printf("After Send\n");
		bytes_recieved = recv(sock, board, 9, 0);
		if (board[0] == 'P') {
			printf("Game over! You've won!\n");
			free(move);
			free(board);
			close(sock);
			return 0;
		} else if (board[0] == 'S') {
			printf("Game over! You lost!\n");	
			free(move);
			free(board);	
			close(sock);
			return 0;
		}
        }   
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
