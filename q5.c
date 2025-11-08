#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	errno = 0;
	int fd = open ("sparse.bin", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if (fd == -1) perror("open");

	char s[] = "START";
	ssize_t ret = write(fd, s, strlen(s));
	off_t p = lseek(fd, 1024 * 1024, SEEK_CUR);
	if (p == -1) perror("lseek");

	char e[] = "END";
	ret = write(fd,e, strlen(e));
	if (ret == -1) perror("write");

	if (close(fd) == -1) perror("close");

	fd = open ("sparse.bin", O_RDONLY);
	p = lseek(fd, 0, SEEK_END);
	if (p == -1) perror("lseek");

	printf("Number of bytes in file: %ld\n", p);

	// we just jumped 1024*1024 bytes down, leaving a ‘hole’ of size 1MiB
	// As a way of optimisation, the system avoids storing that hole as bytes, creating what’s known as a sparse file.
	// To keep track of the theoretical size of the file, it just stores this information in its metadata, noting that hole.
	// That way, we get our file looking exactly how we asked, but we also avoid wasting storage space.

	return 0;
}
