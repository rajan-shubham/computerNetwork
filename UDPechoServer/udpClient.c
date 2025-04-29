#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char message[MAXLINE];
    char server_ip[INET_ADDRSTRLEN];
    int server_port;
    struct sockaddr_in servaddr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Get server IP from user
    printf("Enter server IP address: ");
    if (fgets(server_ip, sizeof(server_ip), stdin) == NULL) {
        fprintf(stderr, "Failed to read IP address\n");
        exit(EXIT_FAILURE);
    }
    server_ip[strcspn(server_ip, "\n")] = 0; // Remove trailing newline

    // Get server port from user
    printf("Enter server port: ");
    if (scanf("%d", &server_port) != 1) {
        fprintf(stderr, "Invalid port number\n");
        exit(EXIT_FAILURE);
    }
    getchar(); // Consume leftover newline from scanf

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported\n");
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(servaddr);

    printf("Connected to server %s:%d. Type 'exit' to quit.\n", server_ip, server_port);

    while (1) {
        // Get message from user
        printf("You: ");
        if (fgets(message, MAXLINE, stdin) == NULL) {
            printf("\nInput error or EOF detected. Exiting.\n");
            break;
        }
        message[strcspn(message, "\n")] = 0; // Remove newline

        // Check if user wants to exit
        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Send message to server
        if (sendto(sockfd, (const char *)message, strlen(message), 0,
                   (const struct sockaddr *)&servaddr, len) == -1) {
            perror("sendto failed");
            continue;
        }

        // Receive response from server
        int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0,
                         NULL, NULL); // We don't need source address here
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
//client