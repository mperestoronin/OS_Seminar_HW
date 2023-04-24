#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void send_bit(pid_t receiver_pid, int bit) {
    int sig = bit ? SIGUSR2 : SIGUSR1;
    kill(receiver_pid, sig);
    usleep(1000);
}

int main() {
    pid_t my_pid = getpid();
    printf("Transmitter PID: %d\n", my_pid);

    pid_t receiver_pid;
    printf("Enter the receiver PID: ");
    scanf("%d", &receiver_pid);

    int number;
    printf("Enter an integer to send: ");
    scanf("%d", &number);

    for (int i = 0; i < (sizeof(int) * 8); ++i) {
        int bit = (number >> i) & 1;
        send_bit(receiver_pid, bit);
    }

    return 0;
}
