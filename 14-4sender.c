#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t my_pid, to_pid;
  int value;

  my_pid = getpid();
  printf("Sender pid = %d\n", my_pid);

  printf("Enter pid = ");
  scanf("%d", &to_pid);

  printf("Enter value = ");
  scanf("%d", &value);

  int byte = 1;
  while (byte != 0) {
    if ((value & byte) == 0) {
      kill(to_pid, SIGUSR1);
    } else {
      kill(to_pid, SIGUSR2);
    }
    byte = byte << 1;
	for (int i = 0; i < 100000000; ++i);
  }
}
