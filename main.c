#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int uid = (int)getuid();
	int gid = (int)getgid();
	printf("Uid = %d; Gid = %d", uid, gid);
	return 0;
}
