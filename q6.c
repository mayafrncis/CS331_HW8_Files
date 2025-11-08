#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	int fd = open("numbers.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if (fd == -1) perror("open");
	ssize_t ret;
	errno = 0;
	for (int i = 0; i < 10; i++){
		char text[5];
		snprintf(text,5,"%d\n", i+1);
		ret = write(fd, text, strlen(text));
		if (ret == -1) perror("write");
	}

	if (close(fd) == -1) perror("close");

	fd = open("numbers.txt", O_RDWR);
	if (fd == -1) perror("open");

	int line = 0;
	int b = 0;
	char buf[1];

	while(line<3) {
		ret = read(fd, buf, 1);
		if (ret == -1) {
                        if (errno == EINTR) continue;
                        perror("read");
                        break;
                }

		b++;
		if (buf[0] == '\n')
			line++;
	}

	off_t p = lseek(fd, b, SEEK_SET);
	if (p == -1) perror("lseek");

	char c[15];
	char *buf2 = c;
	int len = 15;
	while ((ret = read(fd, buf2, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR) continue;
			perror("read");
			break;
		}
		len -= ret;
		buf2 += ret;
	}
	p = lseek(fd, b, SEEK_SET);
	ret = write(fd, "100\n", 4);

	p = lseek(fd, 0, SEEK_CUR);
	ftruncate(fd, p);

	int written = 0;
	buf2 = c+2;
	len = 13;
	while ((ret = write(fd, buf2 + written, len - written)) != 0) {
		if (ret == -1) perror("write");
		written += ret;
	}
	p = lseek(fd, 0, SEEK_SET);
	if (p == -1) perror("lseek");
	ssize_t ret2;
	while ((ret = read(fd, buf, 1)) != 0) {
		ret2 = write(1, buf,1);
                if (ret == -1) {
                        if (errno = EINTR) continue;
                        perror("read");
                        break;
                }
		if (ret2 == -1) perror("write");
	}

	return 0;

}

