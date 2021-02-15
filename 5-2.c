#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int fp[2], fc[2], result;
  	size_t size;
  	char  resstring[14];
 
 	if (pipe(fp) < 0) {
    	printf("Can\'t open pipe for parent\n");
    	exit(-1);
  	}

	if (pipe(fc) < 0) {
		printf("Cant open pipe for child");
		exit(-1);
	}

  	result = fork();

  	if (result < 0) {
    	printf("Can\'t fork child\n");
    	exit(-1);
  	} else if (result > 0) {
	   	/* Parent process */
		sleep(1);
    	close(fp[0]);
		close(fc[1]);
   		size = write(fp[1], "Hello, child!", 14);
    
		if (size != 14) {
      		printf("Can\'t write all string to pipe\n");
    	  	exit(-1);
		}
		close(fp[1]);

		size = read(fc[0], resstring, 14);
		if (size != 14) {
			printf("Parent can\'t read from child");
			exit(-1);
		} 
    
		printf("Parent from child: %s\n", resstring);
    	close(fc[0]);

		printf("Parent exit\n");
  	} else {
		sleep(1);
    	/* Child process */
   		close(fc[0]);
		close(fp[1]); 
		size = read(fp[0], resstring, 14);
    	if (size < 0) {
      		printf("Can\'t read string from pipe\n");
      		exit(-1);
    	}
		
		printf("Child from parent: %s\n", resstring);
    	size = write(fc[1], "Hello, batya!", 14);
 
		close(fc[1]);
		close(fp[0]);
		printf("Child exit\n");
  	}
  return 0;
}
