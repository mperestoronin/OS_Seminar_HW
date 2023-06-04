#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// Функция для вычисления числа Фибоначчи
int fibonacci(int n) {
    if (n <= 1)
        return n;
    else
        return fibonacci(n-1) + fibonacci(n-2);
}

// Функция для вычисления факториала
int factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n-1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Необходимо передать один аргумент.\n");
        return 1;
    }

    int n = atoi(argv[1]);

    pid_t pid = fork();

    if (pid == 0) {
        // Это код процесса-ребенка
        printf("Факториал %d: %d\n", n, factorial(n));
    } else if (pid > 0) {
        // Это код родительского процесса
        printf("Число Фибоначчи %d: %d\n", n, fibonacci(n));
    } else {
        printf("Не удалось создать процесс.\n");
        return 1;
    }

    return 0;
}
