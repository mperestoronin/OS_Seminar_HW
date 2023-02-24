#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char * fifo = "/tmp/fifo";
    mkfifo(fifo, 0666);
    char str1[80], str2[80];
    while (1) {
        fd = open(fifo, O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd, str2, strlen(str2) + 1);
        close(fd);
        fd = open(fifo, O_RDONLY);
        read(fd, str1, 80);
        printf("К2: %s\n", str1);
        close(fd);
    }
    return 0;
}
