#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() {
	char a[4000];
	char b[4000];
	printf("Enter first file path: ");
	scanf("%4000s", a);
	printf("Enter second file path: ");
	scanf("%4000s", b);

	errno = 0;

	int fda = open(a, O_RDONLY);
	int fdb = open(b, O_RDONLY);
	if (fda == -1 || fdb == -1) {
		perror("open");
		return 1;
	}

	char bufa[1000], bufb[1000];
	ssize_t reta, retb, diff, min;
	long int compared = 0;
	while (1) {
		reta = read(fda, bufa, sizeof(bufa));
		retb = read(fdb, bufb, sizeof(bufb));
		if (reta == -1 || retb == -1) {
			perror("read");
			close (fda);
			close(fdb);
			return 1;
		}

		if (reta < retb) min = reta;
		else min = retb;

		off_t p;
		for (int i = 0; i < min; i++) {
			if (bufa[i] != bufb[i]) {
				printf("Files differ at byte %ld\n", compared - 1);
				if (close(fda) == -1 || close(fdb) == -1) perror("close");
				return 1;
			}
			compared++;
		}

		if (reta == 0 && retb == 0) {
			printf("Files are identical\n");
			break;
		} else if (reta == 0 || retb == 0) {
			printf("Files differ at byte %ld\n", compared - 1);
			if (close(fda) == -1 || close(fdb) == -1) perror("close");
			return 1;
		}

		if (reta < retb) p = lseek(fdb, min - retb, SEEK_CUR);
    		else p = lseek(fda, min - reta, SEEK_CUR);

	}
	if (close(fda) == -1 || close(fdb) == -1) perror("close");
	return 0;
}
