#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	char source[4000];
	char dest[4000];
	printf("Enter source file path: ");
	scanf("%256s", source);
	printf("Enter destination file path: ");
	scanf("%4000s", dest);

	errno = 0;
	int fds, fdd;
	errno = 0;
	char c[10];
	fds = open(source, O_RDONLY);
	fdd = open(dest, O_WRONLY|O_CREAT|O_TRUNC);
	if (fds == -1 || fdd == -1) {
		perror("opening");
	}

	ssize_t retr, retw;
	char * buf;
	int len;
	buf = c;
	while ((retr = read(fds, buf, 8)) > 0) { // keeps going until EOF or reading fails
 		int written = 0; // how many we've written total
		while (retr > written) { // checks if we have bytes that were read to still write then starts writing from where we last left off in the buffer and writes however many bytes were read
			retw = write(fdd, buf + written, retr - written);
			if (retw == -1) {
				perror("writing");
			}
			written += retw; // in case it doesn't manage to write all 8 bytes before being interrupted, we would need to keep track of what we still have left to write
		}
	}

	if (retr == -1)	{
		perror("read");
	} else if (retr == 0) {
		printf("File contents copied.\n");
	}

	int cd = close(fdd);
	int cs = close(fds);
	if (cd == -1 || cs == -1) {
		perror("close");
	}
	return 0;
}
