#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {

    srand(time(NULL));

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

    struct servermsgbuf {
        long mtype;
        struct {
           	float number;
        } info;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();

    len = sizeof(clientbuf.info);
	printf("Write float number: ");
    scanf("%f", &clientbuf.info.number);

    printf("Client #%d sends message %.2f\n", clientbuf.info.pid, clientbuf.info.number);
    if (msgsnd(msqid, (struct clientmsgbuf *) &clientbuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Waiting for response\n");

    maxlen = sizeof(serverbuf.info);
    if (len = msgrcv(msqid, &serverbuf, maxlen, getpid(), 0) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }
    printf("Server: %.2f\n", serverbuf.info.number);

    return 0;

}
