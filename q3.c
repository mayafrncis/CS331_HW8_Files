#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	char file[256];
	printf("Enter file source: ");
	scanf("%256s", file);

	int fd = open(file, O_RDONLY);
	if (fd == -1) perror("opening");

	int size = lseek(fd, 0, SEEK_END);
	int ret1 = 0;
	char c[1];
	char *buf = c;
	for (int i = 0; i < size; i++) {
		if (i == 0) ret1 = lseek(fd, -i, SEEK_END);
		else ret1 = lseek(fd, -i-1,SEEK_END);
		if (ret1 == -1) perror("lseek");
		ssize_t ret = read(fd, buf, 1);
		printf("%c", buf[0]);
	}
	printf("\n");
	return 0;
}
