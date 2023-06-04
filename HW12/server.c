#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>



#define PORT 8080

#define MAXLINE 1024



int main() {

    int sockfd;

    char buffer[MAXLINE];

    struct sockaddr_in servaddr;

    

    // Creating socket file descriptor

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

        perror("socket creation failed");

        exit(EXIT_FAILURE);

    }



    int broadcast = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {

        perror("setsockopt (SO_BROADCAST)");

        exit(EXIT_FAILURE);

    }



    memset(&servaddr, 0, sizeof(servaddr));

    

    // Filling server information

    servaddr.sin_family = AF_INET;

    servaddr.sin_port = htons(PORT);

    servaddr.sin_addr.s_addr = INADDR_BROADCAST;

    

    while(1) {

        printf("Enter message: ");

        fgets(buffer, MAXLINE, stdin);

        

        // Remove newline character from buffer

        buffer[strcspn(buffer, "\n")] = 0;



        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        printf("Message sent.\n"); 

    }



    return 0;

}

