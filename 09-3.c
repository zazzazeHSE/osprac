#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <errno.h>

int increment(int semid, struct sembuf* buf) {
	buf->sem_op = 1;
	buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int decrement(int semid, struct sembuf* buf) {
	buf->sem_op = -1;
	buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int main() {
	int pch[2], res;
	size_t size = 0;
	key_t key;
	char pathname[] = "09-3.c";
	struct sembuf buffer;
	char result_string[14];
	int semid;
	
	int N;
	printf("Write N: ");
	scanf("%d", &N);			

	//create pipe
	if (pipe(pch) < 0) {
		printf("Pipe error\n");
		exit(-1);
	}

	if ((key = ftok(pathname, 0)) < 0) {
		printf("Key generation error\n");
		exit(-1);
	}
	
	if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
		if (errno != EEXIST) {
			printf("Creation error\n");
			exit(-1);
		}
		if((semid = semget(key, 1, 0666 | IPC_CREAT) < 0)) {
			printf("Semaphore find error");
			exit(-1);
		}
	}

	if ((res = fork()) < 0) {
		printf("Fork error\n");
		exit(-1);
	} else if (res > 0) {
		//parent process
		for (size_t i = 0; i < N; ++i) {
			if (write(pch[1], "Hello, world!", 14) != 14) {
				printf("Parent message write error!");
				exit(-1);
			}
			
			//parent wait - -1
			if (decrement(semid, &buffer) < 0) {
				printf("Sem cant wait\n");
				exit(-1);
			}
			
			if ((size = read(pch[0], result_string, 14) != 14)) {
				printf("Parent read from child error!\n");
				exit(-1);
			}
			printf("Parent read from child %s\n", result_string);
		}
		if (close(pch[0]) < 0) {
			printf("Cant close pipe\n");
			exit(-1);
		}
		if (close(pch[1]) < 0) {
			printf("Cant close pipe\n");
			exit(-1);
		}
	} else {
		//child process
		for (int i = 0; i < N; ++i) {
			if ((size = read(pch[0], result_string, 14)) < 0) {
				printf("This is the end\n");
				return 0;
			}
			printf("Child get message from parent: %s\n", result_string);
			if (write(pch[1], "Hello, batya!", 14) != 14) {
				printf("Child write error\n");
				exit(-1);
			}
			//parent access - increment semahpore
			if (increment(semid, &buffer) < 0) {
				printf("Cant\'t inc sem\n");
				exit(-1);
			}
		}
		
		if (close(pch[0]) < 0) {
			printf("Cant close pipe\n");
			exit(-1);
		}
		if (close(pch[1]) < 0) {
			printf("Cant close pipe\n");
			exit(-1);
		}
	}
}
