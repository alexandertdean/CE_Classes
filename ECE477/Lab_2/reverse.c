#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{	
	FILE *fd;
	FILE *out;
	int count = 0;					//counts lines of file
	char **lines;					//array of strings that contains lines of file
	int i;						//iterator for for loop
	char *ch;					//variable for checking for new liens


	if (argc < 3) {
		printf("Usage: %s Input Output", argv[0]);
		return 1;
	}
	fd = fopen(argv[1], "r");			//opens input file to be reversed
	out = fopen(argv[2], "w+");			//opens output file to write reversed input to
	ch = (char *)malloc(1024);			//allocates memory for string for line counting
	while(fgets(ch, 1024, fd) != NULL) {		//continues until EOF, counting number of lines
		count++;
	}
	lines = (char **)malloc(count * sizeof(char *));//allocates memory for string pointers
	if (lines == NULL) {				//error checking
		perror("First malloc()");
		return 1;
	}
	for (i = 0; i < count; i++){
		lines[i] = (char *)malloc(1024 * sizeof(char));		//allocates memory for string in each element of first dimension
		if (lines[i] == NULL) {					//error checking
			perror("Second malloc()");	
			return 1;
		}
	}
	rewind(fd);							//moves file pointer to start of file
	for(i = 0; i < count; i++) {
		fgets(lines[i], 1024, fd);				//reads lines of file into array iteratively
	}
	for (i = count-1; i >= 0; i--) {				//prints lines in reverse order
		fprintf(out, "%s", lines[i]);
	}
	fclose(fd);							//closes file
	return 0;

}
