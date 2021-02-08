#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char *envp[]) {
	int pid = fork();
	if (pid == -1) {
		printf("New process error\n");
	}
	else if (pid == 0) {
		if (argc < 2) {
			printf("You should write executable file path as first argument line param");
			exit(-1);
		}
	
		(void) execle("/bin/cat", "/bin/cat", argv[1], 0, envp);
		
		printf("Exec error");
		exit(-1);
	}
	return 0;
}
