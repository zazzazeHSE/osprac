#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int msqid;

    char pathname[] = "11-2a.c";

    key_t key;

    int len;

    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
			float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    printf("Start passing messages\n");
    for (int i = 0; i < 5; ++i) {
        mybuf.mtype = 1;
        mybuf.info.sinfo = 2222;
		mybuf.info.finfo = 22.22;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    printf("End of message passing\n");
    printf("Start getting messages\n");

    for (int i = 0; i < 6; ++i) {
        len = sizeof(mybuf.info);
        if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 2, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("New message type = %ld, sinfo = %i\n, finfo = %f\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
    }

    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

    printf("Finish");

    return 0;
}
