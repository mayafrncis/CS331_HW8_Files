#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	int fd = open ("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0644);
	if (fd == -1) perror("open");
	char *buf;
	char c[1000];
	buf = c;
	errno = 0;
	int n = 0;
	printf("Enter some line: \n");
	while (n < 1000) {
		ssize_t ret = read(0,buf + n,1); //0 for stdin
		if (ret == -1) {
			if (errno == EINTR) continue;
			perror("read");
		}
		if (ret == 0) break; // via ctrl d
		n++;
		if (buf[n-1] == '\n') break;
	}

	char text[15];
	snprintf(text, 15, "PID=%d: ", getpid());
	ssize_t ret;
	if ((ret = write(fd, text, strlen(text))) == -1) perror("write");
	if ((ret = write(fd, buf, n)) == -1) perror("write");
	if (n==0 || buf[n-1] != '\n')
		if ((ret == write(fd,"\n", 1)) == -1) perror("write");
	off_t pos = lseek(fd, 0, SEEK_CUR);
	printf("Current new file position: %ld\n", pos);
	// Opening a file with O_APPEND means anything written will immediately be added to the end of the file, regardless of where the offset is
	// so even if we move arround the offset with SEEK_SET, O_APPEND ignores it and writes at the end of the file anyway
	// Whenever we write a number of bytes, the offset increases exactly by that number of bytes, regardless of where the offset is currently
	// So although we might be appending at the end and the offset may not actually be at that end, it would still increase
	// https://stackoverflow.com/questions/24223661/why-is-data-written-to-a-file-opened-with-o-append-flag-always-written-at-the-e 

	if (close(fd) == -1) perror("close");

	return 0;
}
