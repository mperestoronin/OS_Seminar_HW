#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int received_number = 0;
volatile int bit_count = 0;

void sigusr_handler(int sig) {
    if (sig == SIGUSR1) {
        received_number |= (0 << bit_count);
    } else if (sig == SIGUSR2) {
        received_number |= (1 << bit_count);
    }
    bit_count++;
}

int main() {
    pid_t my_pid = getpid();
    printf("Receiver PID: %d\n", my_pid);

    pid_t transmitter_pid;
    printf("Enter the transmitter PID: ");
    scanf("%d", &transmitter_pid);

    signal(SIGUSR1, sigusr_handler);
    signal(SIGUSR2, sigusr_handler);

    while (bit_count < (sizeof(int) * 8)) {
        usleep(1000);
    }

    printf("Received integer: %d\n", received_number);

    return 0;
}
