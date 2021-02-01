#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int pid = fork();
	if (pid == -1) {
		printf("New process error\n");
	}
	else if (pid == 0) {
		printf("New child process\n");
	}
	else {
		printf("New parent process\n");
	}
	return 0;
}
