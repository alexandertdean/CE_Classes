#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc, char * argv[], char * env[])
{
	if(argc<2){
		printf("Usage is %s program [arguments]\n",
			argv[0]);return -1;
	}
	if(iopl(3)!=0) {
		perror("iopl");return -2;
	}
	setuid(getuid());
	if(argc >1)
	{
		execve(argv[1],&argv[1],env);
	}
}

