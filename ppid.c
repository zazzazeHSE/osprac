#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int pid = (int)getpid();
	int ppid = (int)getppid();
	printf("Pid = %d; Ppid = %d", pid, ppid);
	return 0;
}
