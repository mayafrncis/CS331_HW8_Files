#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	char source[256];
	char dest[256];
	printf("Enter source file path: ");
	scanf("%256s", source);
	printf("\nEnter destination file path: ");
	scanf("%256s", dest);

	int fds, fdd;
	errno = 0;
	char c[10]; // where the characters will be stored
	errno = 0;
	fds = open(source, O_RDONLY);
	fdd = open(dest, O_WRONLY|O_CREAT|O_TRUNC);
	if (fds == -1 || fdd == -1) {
		perror("opening");
	}
	printf("\nopened");
	ssize_t ret;
	char * buf;
	int len;
	while (1) {
		buf = c;
		len = 8;
		while((ret = read(fds, buf, len)) != 0) {
			if (ret == -1) {
				if (errno == EINTR)
					continue;
				perror("read");
				break;
			}
			if (ret == 0) break;
			printf("\nread 8");
			len -= ret;
			buf += ret;
		}
		// buf ends up pointing to the next empty spot
		// len makes sure we keep going until we read 8 characters for sure since the process of reading can be interrupted

		if (ret == 0)
			break; // reached EOF and read nothing, so we have nothing to write
		int read = buf - c; // subtracting the addresses from each other gives us how many characters we read and so how many we need to write
		buf = c; // resetting to start writing from the start
		printf("\nread: %d", read);
		while(read > 0) {
			ret = write(fdd, buf, read);
			if (ret == -1) {
				if (errno == EINTR)
					continue;
				perror("write");
				break;
			}
			printf("\nwrote 8");
			read -= ret; // in case we got interrupted and didn't manage to write all, to keep track of how many we have left to write
			buf += ret;
		}
	}
	int cd = close(fdd);
	int cs = close(fds);
	if (cd == -1 || cs == -1) {
		perror("close");
	}
	printf("\nclosed\n");
	printf("%d", errno);
	return 0;
}
