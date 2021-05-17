#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/file.h>
#include <errno.h>
int main() {
    int msqid;

    char pathname[] = "11-3b.c";

    key_t key;

    int len, maxlen;

struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float number;
        } info;
    } clientbuf;

    struct sermsgbuf {
        long mtype;
        struct {
            float number;
        } info;
    } serverbuf;
	
	int pidfile = open("11-3b.c", O_CREAT | O_RDWR, 0666);
	int rc = flock(pidfile, LOCK_EX | LOCK_NB);
	if (!rc) {
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    while (1) {
        maxlen = sizeof(clientbuf.info);
        if (len = msgrcv(msqid, (struct clientmsgbuf *) &clientbuf, maxlen, -2, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }
	
		if (clientbuf.mtype == 2) {
			printf("I just killed\n");
			exit(-1);
		}

        printf("Client %d: %.2f\n", clientbuf.info.pid, clientbuf.info.number);

        serverbuf.mtype = clientbuf.info.pid;

        serverbuf.info.number = clientbuf.info.number * clientbuf.info.number;
        len = sizeof(serverbuf.info);

        if (msgsnd(msqid, (struct sermsgbuf *) &serverbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Sent response\n");
    } 
	} else {
		printf("Server is running");
	}
    return 0;
}
