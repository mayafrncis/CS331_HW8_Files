#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	const char *buf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int fd;
	int ret;
	errno = 0;

	fd = open ("data.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
        if (fd == -1)
                perror("open");

	ret = write(fd, buf, strlen(buf));
	if (ret == -1)
		perror("write");

	if (close(fd) == -1)
		perror("close");

	fd = open ("data.txt", O_RDWR);
	off_t size = lseek(fd, 0, SEEK_END);
	printf("Current size: %d\n", size);

	ret = ftruncate(fd, 10);
	if (ret == -1)
		perror("truncate");


	size = lseek(fd, 0, SEEK_END);
	printf("New size: %d\n", size);

	off_t ret1 = lseek(fd, 0, SEEK_SET);
	if (ret1 == -1)
		perror("lseek");
	char c[10];
	int len = 10;
	ssize_t ret2;
	char *buf2;
	buf2 = c;
	while (len != 0 && (ret2 = read(fd, buf2, len)) != 0) {
		if (ret2 == -1) {
			if (errno == EINTR) continue;
			perror("read");
			break;
		}
		len -= ret2;
		buf2 += ret2;
	}

	for (int i = 0; i < 10; i++) {
		printf("%c", buf[i]);
	}
	printf("\n");

	return 0;
}
