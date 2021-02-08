#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[], char *envp[]) {
	printf("Argument line params:\n");
	for (int i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}
	
	printf("Environment params:\n");
	int i = 0;
	while (envp[i] != NULL) {
		printf("%s\n", envp[i++]);
	}
	return 0;
}
