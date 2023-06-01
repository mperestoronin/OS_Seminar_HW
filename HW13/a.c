#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char oldname[3] = "a";
    char newname[3] = "aa";
    char buf[1] = " ";

    int depth = 0;
    int fd = open(oldname, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        printf("Не удалось создать файл.\n");
        return 1;
    }
    write(fd, buf, 1);
    close(fd);

    while (1) {
        if (symlink(oldname, newname) == -1) {
            printf("Ошибка\n");
            return 1;
        }
        fd = open(newname, O_RDONLY);
        if (fd == -1) {
            break;
        }
        close(fd);
        ++depth;
        oldname[0] = newname[0];
        oldname[1] = newname[1];
        newname[1]++;
    }

    printf("Глубина рекурсии для символьных связей: %d\n", depth);

    return 0;
}
