#include <sys/sysinfo.h>
#include <stdio.h>

int main() 
{
	struct sysinfo info;
	int errChk;

	errChk = sysinfo(&info);
	if (errChk < 0) {
		perror("sysinfo");
		return 1;
	}
	printf("%d\n", info.loads[1]);
	return 0;
}
