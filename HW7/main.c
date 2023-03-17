#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#define SHM_SIZE 1024

void *client(void *arg);
void *server(void *arg);

int main() {
    srand(time(0));
    int *shm_ptr;
    int input_file;
    input_file = shm_open("/my_memory", O_CREAT | O_RDWR, 0666);
    if (input_file < 0) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(input_file, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }
    shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, input_file, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    if (close(input_file) == -1) {
        perror("close");
    }
    pthread_t client_thread, server_thread;
    int client_tid, server_tid;
    client_tid = pthread_create(&client_thread, NULL, client, (void*) shm_ptr);
    if (client_tid != 0) {
        perror("pthread_create");
        exit(1);
    }
    server_tid = pthread_create(&server_thread, NULL, server, (void*) shm_ptr);
    if (server_tid != 0) {
        perror("pthread_create");
        exit(1);
    }
    pthread_join(client_thread, NULL);
    pthread_join(server_thread, NULL);
    if (munmap(shm_ptr, SHM_SIZE) == -1) {
        perror("munmap");
        exit(1);
    }
    if (shm_unlink("/my_memory") == -1) {
        perror("shm_unlink");
        exit(1);
    }
    return 0;
}

void *client(void *arg) {
    int *shm_ptr = (int*)arg;
    for (int i = 0; i < SHM_SIZE / sizeof(int); i++) {
        shm_ptr[i] = rand() % 11;
    }
    pthread_exit(NULL);
}

void *server(void *arg) {
    int *shm_ptr = (int*)arg;
    for (int i = 0; i < SHM_SIZE / sizeof(int); i++) {
        printf("%d\n", shm_ptr[i]);
    }
    pthread_exit(NULL);
}
