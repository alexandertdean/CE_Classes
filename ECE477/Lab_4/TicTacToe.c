#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int doClient();
void printBoard(char **board);
int doServer(char **clientMove, char **board);
int checkWin(char **board);
int checkMove(char **clientMove, char **board);
int checkCode(int code);
int serverMove(char **board);

int main ()
 {
	while (1) {
		if(doClient() == 0) {
			printf("Game over.\n");
			break;
		}
	}
	return 0;
}

int doClient()
{
	char *board;
	char *move;
	int i;
	board = malloc(2);
	move = malloc(2);
	for (i = 0; i < 9; i++) {
		board[i] = 'B';
	}
	int serverCode;

	while (1) {
		printf("----------------------------------------------------------\n");
		printBoard(&board);
		printf("Move: ");
		scanf("%s", move);
		serverCode = doServer(&move, &board);
		if (serverCode == 1) {
			printf("Something went wrong with the server\n");
			return 1;
		}
		if (checkCode(serverCode)) {
			return 0;
		}
	}
}

void printBoard(char **board)
{	
	char *printBoard = *board;
	printf("%c | %c | %c\n",printBoard[0],printBoard[1],printBoard[2]);
	printf("---------\n");
	printf("%c | %c | %c\n", printBoard[3], printBoard[4], printBoard[5]);
	printf("---------\n");
	printf("%c | %c | %c\n", printBoard[6], printBoard[7], printBoard[8]);
}

int doServer(char **clientMove, char **board)
{
	int moveInvalid;
	int win;
	int move;
	int i;
	moveInvalid = checkMove(&clientMove[0], &board[0]);
	if (moveInvalid) return moveInvalid;
	for (i = 0; i < 9; i++) {
		board[0][i] = clientMove[0][i];
	}
	win = checkWin(&board[0]);
	if (win) return win;
	move = serverMove(&board[0]);
	if (move) return move;
	return 1;
}

int checkMove(char **clientMove, char **board)
{
	int numDifferent = 0;
	int i;				//iterator for for loop
	if (strlen(*clientMove) != 9) {
		return 400;
	}
	for (i = 0; i < 9; i++) {
		if (clientMove[0][i] != 'B' && clientMove[0][i] != 'X' && clientMove[0][i] != 'O') {		//ensures that a valid move was made 
			return 400;
		}
 		if (clientMove[0][i] != board[0][i]) {
			if (board[0][i] != 'B') {						//checks if an existing X or O was replaced by client
				return 401;
			}
			numDifferent++;										//counts number of different characters
		}
	}
	if (numDifferent != 1) {										//checks if more than one move was made simultaneously
		return 401;
	}
	return 0;
}

int checkWin(char **board)		//checks each possible win scenario, then checks whether X or O won
{
	char *localBoard = *board;
	if ((localBoard[0] == localBoard[1]) && (localBoard[0] == localBoard[2])) {		//checks for win on top row
		if (localBoard[0] == 'X') {
			return 300;
		} else if (localBoard[0] == 'O') {
			return 200;
		}
	}
	if ((localBoard[3] == localBoard[4]) && (localBoard[3] == localBoard[5])) {		//checks for win on middle row
		if (localBoard[3] == 'X') {
			return 301;
		} else if (localBoard[3] == 'O') {
			return 201;
		}
	}
	if ((localBoard[6] == localBoard[7]) && (localBoard[6] == localBoard[8])) {		//checks for win on bottom row
		if (localBoard[6] == 'X') {
			return 302;
		} else if (localBoard[6] == 'O') {
			return 202;
		}
	}
	if ((localBoard[0] == localBoard[3]) && (localBoard[0] == localBoard[6])) {		//checks for win on left column
		if (localBoard[0] == 'X') {
			return 303;
		} else if (localBoard[0] == 'O') {
			return 203;
		}
	}
	if ((localBoard[1] == localBoard[4]) && (localBoard[1] == localBoard[7])) {		//checks for win on middle column
		if (localBoard[1] == 'X') {
			return 304;
		} else if (localBoard[1] == 'O') {
			return 204;
		}
	}
	if ((localBoard[2] == localBoard[5]) && (localBoard[2] == localBoard[8])) {		//checks for win on right column
		if (localBoard[2] == 'X') {
			return 305;
		} else if (localBoard[2] == 'O') {
			return 205;
		}
	}
	if ((localBoard[0] == localBoard[4]) && (localBoard[0] == localBoard[8])) {		//checks for win diagonally down and right
		if (localBoard[0] == 'X') {
			return 306;
		} else if (localBoard[0] == 'O') {
			return 206;
		}
	}
	if ((localBoard[6] == localBoard[4]) && (localBoard[6] == localBoard[2])) {		//checks for win diagonally up and right
 		if (localBoard[6] == 'X') {
			return 307;
		} else if (localBoard[6] == 'O') {
			return 207;
		}
	}
	return 0;
}

int serverMove(char **board)
{
	char *localBoard = board[0];
	int moveIndex;										//index for server move
	int win;					
	printf("Server Move\n");	
	while (1) {
		moveIndex = rand() % 9;
		if (localBoard[moveIndex] != 'X' && localBoard[moveIndex] != 'O') {
			localBoard[moveIndex] = 'O';
			win = checkWin(&localBoard);
			board[0] = localBoard;
			if (win) {
				return win;
			}
			return 100;
		}
	}	
}

int checkCode(int code)
{
	if ((code / 100) == 2 || (code / 100) == 3) {
		return 1;
	}
	return 0;
}
