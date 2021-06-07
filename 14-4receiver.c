#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value;
int byte;

void zero(int nsig) {
  byte <<= 1;
}

void one(int nsig) {
  value |= byte;
  byte <<= 1;
}

int main(void) {
  pid_t my_pid, sender_pid;
  value = 0;
  byte = 1;
  (void) signal(SIGUSR1, zero);
  (void) signal(SIGUSR2, one);

  my_pid = getpid();
  printf("Receiver pid = %d\n", my_pid);

  printf("Sender pid = ");
  scanf("%d", &sender_pid);

  while (1) {
    if(byte == 0) {
      printf("Received = %d\n", value);
      break;
    }
  }
}
