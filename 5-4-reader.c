#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int fd;
	size_t size;
	char resstring[14];
	char name[] = "fifo.fifo";
	(void)umask(0);	
	if ((fd = open(name, O_RDONLY)) < 0) {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

	size = read(fd, resstring, 14);

	if (size < 0) {
      printf("Can\'t read string from FIFO\n");
      exit(-1);
    }

	printf("Resstring: %s\n", resstring);


	if (close(fd) < 0) {
      printf("Can\'t close FIFO\n"); exit(-1);
    }


	return 0;
}
