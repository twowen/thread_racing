#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main (void)
{
    int pid;
    pid = fork();
    srand(pid);
    int fd, len = 10;
    struct stat sb;
    char *p;

	fd = shm_open("tst", O_RDWR | O_CREAT, 0777);
	if (fd == -1) {
		perror("open");
		return 1;
	}

	sb.st_size = len;
	if (fstat(fd, &sb) < 0) {
		perror("fstat");
		return 1;
	}
#if 0
	if (ftruncate(fd, len) < 0) {
		perror("ftruncate");
		return 1;
	}
#endif
	p = mmap(NULL, len, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	for (len = 0; len < sb.st_size; len++) {
		putchar(p[len]);
	}
	fprintf(stderr, "\n");

	shm_unlink("tst");

    if (pid < 0)
    {
        perror("fork"); exit(EXIT_FAILURE);
    }
    else if (pid)	// Parent process
    {
        char *s = "abcdefgh";
        int l = strlen(s);
        for (int i = 0; i < l; ++i)
        {
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
        }
    }
    else 			// Child process
    {
        char *s = "ABCDEFGH";
        int l = strlen(s);
        for (int i = 0; i < l; ++i)
        {
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
        }
    }

	return 0;
}