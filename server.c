#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/dgram_server_socket"
#define BUFFER_SIZE 1024

int create_unix_server_socket(const char *socket_path) {
    int sockfd;
    struct sockaddr_un addr;
    
    // Create socket
    // AF_UNIX for IPC
    // SOCK_DGRAM for UDP (as opposed to SOCK_STREAM, e.g. TCP)
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
	perror("socket");
	exit(EXIT_FAILURE);
    }
    
    // Set up server address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Remove existing socket file if it exists
    unlink(SOCKET_PATH);
    
    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int main() {
    int server_socket;
    struct sockaddr_un client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len;

    // Create the server socket
    server_socket = create_unix_server_socket(SOCKET_PATH);
    if (server_socket == -1) {
        fprintf(stderr, "Failed to create server socket\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s\n", SOCKET_PATH);

    // Constantly receive data packets
    while (1) {
        client_addr_len = sizeof(struct sockaddr_un);
        ssize_t received_bytes = recvfrom(server_socket, buffer, BUFFER_SIZE, 0,
                                          (struct sockaddr *)&client_addr, &client_addr_len);

        if (received_bytes == -1) {
            perror("recvfrom");
            continue;
        }

        buffer[received_bytes] = '\0';
        printf("Received message: %s\n", buffer);

        // Echo the message back to the client
        if (sendto(server_socket, buffer, received_bytes, 0,
                   (struct sockaddr *)&client_addr, client_addr_len) == -1) {
            perror("sendto");
        }
    }

    // Close the socket (this part is never reached in this example)
    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}

