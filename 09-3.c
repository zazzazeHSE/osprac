#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

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
	//create pipe
	if (pipe(pch) < 0) {
		printf("Pipe error\n");
		exit(-1);
	}
	
	if ((semid = semget(key, 1, 0666)) < 0) {
		printf("Semaphore does does not exists. Create...\n");
		if((semid = semget(key, 1, 0666 | IPC_CREAT) < 0)) {
			printf("Semaphore creation error");
			exit(-1);
		}
		printf("Semaphore created");
	}


	if ((res = fork()) < 0) {
		printf("Fork error\n");
		exit(-1);
	} else if (res > 0) {
		//parent process
		int N;
		printf("Write N ");
		scanf("%d", &N);
		for (size_t i = 0; i < N; ++i) {
			if (write(pch[1], "Hello, world!", 14) != 14) {
				printf("Parent message write error!");
				exit(-1);
			}
			
			//child can read and write
			increment(semid, &buffer);
			//parent wait
			decrement(semid, &buffer);
			if ((size = read(pch[0], result_string, 14) != 14)) {
				printf("Parent read from child error!\n");
				exit(-1);
			}
			printf("Parent read from child %s\n", result_string);
		}
		close(pch[0]);
	} else {
		//child process
		while (1) {
			//wait parent
			decrement(semid, &buffer);
			if ((size = read(pch[0], result_string, 14)) < 0) {
				close(pch[1]);
				close(pch[0]);
				printf("This is the end\n");
				return 0;
			}
			printf("Child get message from parent: %s\n", result_string);
			if (write(pch[1], "Hello, batya!", 14) != 14) {
				printf("Child write error\n");
				exit(-1);
			}
			//parent access
			increment(semid, &buffer);
		}
	}
}
