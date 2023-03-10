#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

#define SHM_SIZE 1024

void *client(void *arg);
void *server(void *arg);

int main()
{
    srand(time(0));
    int shm_id;
    key_t key = 12345;
    int *shm_ptr;

    shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (int*)-1) {
        perror("shmat");
        exit(1);
    }

    pthread_t client_thread, server_thread;
    int client_tid, server_tid;
    client_tid = pthread_create(&client_thread, NULL, client, (void*)shm_ptr);
    if (client_tid != 0) {
        perror("pthread_create");
        exit(1);
    }
    server_tid = pthread_create(&server_thread, NULL, server, (void*)shm_ptr);
    if (server_tid != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(client_thread, NULL);
    pthread_join(server_thread, NULL);

    if (shmdt(shm_ptr) < 0) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}

void *client(void *arg)
{
    int i;
    int *shm_ptr = (int*)arg;

    for (i = 0; i < SHM_SIZE/sizeof(int); i++) {
        shm_ptr[i] = rand();
    }

    pthread_exit(NULL);
}

void *server(void *arg)
{
    int i;
    int *shm_ptr = (int*)arg;

    for (i = 0; i < SHM_SIZE/sizeof(int); i++) {
        printf("%d\n", shm_ptr[i]);
    }

    pthread_exit(NULL);
}
