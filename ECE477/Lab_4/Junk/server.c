/* server.c */

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
	char hi[100] = "hello\n";
	char *move;
	move = malloc(2);
	char *board;
	board = malloc(2);
	char *serverCode;
	char *confirmCode;
	int counter =0;
	confirmCode = malloc(1);
	serverCode = malloc(2);
        int sock, connected, bytes_recieved , true = 1;  
        char send_data [1024] , recv_data[1024];       

        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
	

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        int portno;
        int i =0;
        for(i=1;i<argc;i++){
          if(strcmp(argv[i],"-p"))
          {
            portno = atoi(argv[i]);
            printf("Port number entered: %d \n", portno);
          }        
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

            printf("\nI got a connection from (%s , %d)", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				
				printf("\n Want to play Tic Tac Toe ? (y or n): ");
              	gets(send_data);

				if (strcmp(send_data , "n") == 0 || strcmp(send_data , "N") == 0)
              	{
                	send(connected, send_data,strlen(send_data), 0); 
                	close(connected);
                	break;
              	}
        		else if (strcmp(send_data , "y") == 0 || strcmp(send_data , "Y") == 0)
                	send(connected, send_data,strlen(send_data), 0);


       		while (1)
        	{
				bytes_recieved = recv(connected, move, 9, 0);
				move[bytes_recieved] = '\0';
				for (i = 0; i < 9; i++){
						board[i] = move[i];
				}

				if (strcmp(recv_data , "n") == 0 || strcmp(recv_data , "N") == 0)
              	{
                	close(connected);
                	break;
              	}
              	else 
              		printf("\n RECIEVED DATA = %s " , move);
              	fflush(stdout);
              printf("\n Move (q or Q to quit) : ");
              gets(send_data);
              
              if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
              {
                send(connected, send_data,strlen(send_data), 0); 
                close(connected);
                break;
              }
               
              else
                 send(connected, send_data,strlen(send_data), 0); 
				/*if (checkMove(move, board, serverCode)) {
					send(connected, serverCode, 3, 0);
				} else {
					for (i = 0; i < 9; i++) {
						board[i] = move[i];
					}
					if (checkWin(board, serverCode)) {
						send(connected, serverCode, 3, 0);
					} else if (serverMove(board, serverCode)) {
						send(connected, &serverCode, 3, 0);
					} else {
						serverCode = "1";
						send(connected, serverCode, 3, 0);
					}
				}*/
				//printf("%d\n", counter); // Checks to see how many loops of while(1)
				//bytes_recieved = recv(connected, confirmCode, 1, 0);
				/*if (strcmp(confirmCode, "1")) {
					printf("\n\nPositive confirm code recieved\n");
					printf("Server sending move : ");
					for (i = 0; i < 9; i++) {
						printf("%c", board[i]);
					}
					printf("\n");
					send(connected, board, strlen(board), 0);
				} /*else {
					printf(" HELLLOOOOO\n");
					close(connected);
					free(move);
					free(board);
					free(serverCode);
					free(confirmCode);
					break;
				}
				counter++;*/
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
	while (1) {
		moveIndex - rand() % 9;
		if (board[moveIndex] != 'X' && board[moveIndex] != 'O') {
			board[moveIndex] = 'O';
			if (checkWin(board, serverCode) ){
				return 1;
			}
			serverCode = "100";
			return 1;
		}
	}
}
