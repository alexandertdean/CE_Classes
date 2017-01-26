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

int checkMove(char *board, char *move, char *serverCode);
int checkWin(char *board, char *serverCode);
int serverMove(char *board, char *serverCode);

int main(int argc, char *argv[])
{

	
	char *move;
	char *board;
	char *serverCode;
	char *confirmCode;
        int sock, connected, bytes_recieved , true = 1;  
        char send_data [1024] , recv_data[1024];       
        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        int portno;
	int i;
	move = malloc(2);
	board = malloc(2);
	if (argc < 3) {
		portno = 5000;
		printf("To use a custom port number, do %s -p Port_number", argv[0]);
	} else {
		portno = atoi(argv[2]);
	}
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(portno);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
    
  	printf("\nTCPServer Waiting for client on port %d", portno);
        fflush(stdout);

        while(1)
        {  

            sin_size = sizeof(struct sockaddr_in);
            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n I got a connection from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		for (i = 0; i < 9; i++) {
			board[i] = 'B';
		}
            while (1)
            {
		fflush(stdout);
		bytes_recieved = recv(connected, move, 9, 0);
		printf("After recieve Move: %s\n", move);
		if (checkMove(board, move, serverCode)) {
			printf("Move invalid");
			fflush(stdout);
			send(connected, move, 9, 0);
		} else {
			for (i = 0; i < 9; i++) {
				board[i] = move[i];
			}
			if (checkWin(board, serverCode)) {
				for (i = 0; i < 9; i++) {
					board[i] = 'P';
				}
				send(connected, board, 9, 0);
			} else if (serverMove(board, serverCode)) {
				send(connected, board, 9, 0);
			} else {
				send(connected, board, 9, 0);
			}
		}
            }
        }       
      close(sock);
      return 0;
} 
	
int checkMove(char *board, char *move, char *serverCode)
{
	int numDifferent = 0;
	int i;
	if (strlen(move) != 9) {
		serverCode = "100";
	}
	for (i = 0; i < 9; i++) {
		if (move[i] != 'B' & move[i] != 'X' && move[i] != 'O') {
			serverCode = "400";
		}
		if (move[i] != board[i]) {
			if (board[i] != 'B') {
				serverCode = "401";
			}
			numDifferent++;
		}
	}
	if (numDifferent != 1) {
		serverCode = "401";
	}
	return 0;
}

int checkWin(char *board, char *serverCode)
{
char *localBoard = board;
	if ((localBoard[0] == localBoard[1]) && (localBoard[0] == localBoard[2])) {		//checks for win on top row
		if (localBoard[0] == 'X') {
			serverCode = "300";
			return 1;
		} else if (localBoard[0] == 'O') {
			serverCode = "200";
			return 1;
		}
	}
	if ((localBoard[3] == localBoard[4]) && (localBoard[3] == localBoard[5])) {		//checks for win on middle row
		if (localBoard[3] == 'X') {
			serverCode = "301";
			return 1;
		} else if (localBoard[3] == 'O') {
			serverCode = "201";
			return 1;
		}
	}
	if ((localBoard[6] == localBoard[7]) && (localBoard[6] == localBoard[8])) {		//checks for win on bottom row
		if (localBoard[6] == 'X') {
			serverCode = "302";
			return 1;
		} else if (localBoard[6] == 'O') {
			serverCode = "202";
			return 1;
		}
	}
	if ((localBoard[0] == localBoard[3]) && (localBoard[0] == localBoard[6])) {		//checks for win on left column
		if (localBoard[0] == 'X') {
			serverCode = "303";
			return 1;
		} else if (localBoard[0] == 'O') {
			serverCode = "203";
			return 1;
		}
	}
	if ((localBoard[1] == localBoard[4]) && (localBoard[1] == localBoard[7])) {		//checks for win on middle column
		if (localBoard[1] == 'X') {
			serverCode = "304";
			return 1;
		} else if (localBoard[1] == 'O') {
			serverCode = "204";
			return 1;
		}
	}
	if ((localBoard[2] == localBoard[5]) && (localBoard[2] == localBoard[8])) {		//checks for win on right column
		if (localBoard[2] == 'X') {
			serverCode = "305";
			return 1;
		} else if (localBoard[2] == 'O') {
			serverCode = "205";
			return 1;
		}
	}
	if ((localBoard[0] == localBoard[4]) && (localBoard[0] == localBoard[8])) {		//checks for win diagonally down and right
		if (localBoard[0] == 'X') {
			serverCode = "306";
			return 1;
		} else if (localBoard[0] == 'O') {
			serverCode = "206";
			return 1;
		}
	}
	if ((localBoard[6] == localBoard[4]) && (localBoard[6] == localBoard[2])) {		//checks for win diagonally up and right
 		if (localBoard[6] == 'X') {
			serverCode = "307";
			return 1;
		} else if (localBoard[6] == 'O') {
			serverCode = "207";
			return 1;
		}
	}
	return 0;
}

int serverMove(char *board, char *serverCode)
{
	int moveIndex;
	int win;
	int i;
	time_t t;
	while (1) {
		moveIndex = rand() % 9;
		if (board[moveIndex] != 'X' && board[moveIndex] != 'O') {
			board[moveIndex] = 'O';
			if (checkWin(board, serverCode) ){
				for (i = 0; i < 9; i++) {
					board[i] = 'S';
				}
				return 1;
			}
			serverCode = "100";
			return 1;
		}
	}
}
