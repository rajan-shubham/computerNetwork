#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char response[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d...\n", PORT);
    printf("Waiting for messages...\n");

    socklen_t len;
    int n;

    while (1) {
        len = sizeof(cliaddr);
        
        // Receive message from client
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     0, (struct sockaddr *)&cliaddr,
                     &len);
        buffer[n] = '\0';
        
        // Get client IP and port
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(cliaddr.sin_port);
        
        printf("Client (%s:%d): %s\n", client_ip, client_port, buffer);
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit") == 0) {
            printf("Client disconnected.\n");
            continue;
        }
        
        // Prepare response (echo the message back)
        snprintf(response, MAXLINE, "Echo: %s", buffer);
        
        // Send response to client
        sendto(sockfd, (const char *)response, strlen(response),
               0, (struct sockaddr *)&cliaddr,
               len);
    }

    close(sockfd);
    return 0;
}
//server