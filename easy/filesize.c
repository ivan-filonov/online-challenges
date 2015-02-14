#include <stdio.h>
#include <sys/stat.h>
main(int a, char ** v) {
	struct stat buf;
	stat(v[1], &buf);
	printf("%ld\n", buf.st_size);
	return 0;
}

