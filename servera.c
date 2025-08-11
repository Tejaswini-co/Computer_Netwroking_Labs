#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set up server address
    address.sin_family = AF_INET;              // IPv4
    address.sin_addr.s_addr = INADDR_ANY;      // Accept from any IP
    address.sin_port = htons(PORT);            // Port in network byte order

    // 3. Bind socket to address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("✅ Server listening on port %d...\n", PORT);

    // 5. Accept a connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 6. Receive data
    int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
    printf("📩 Received from client: %s\n", buffer);

    // 7. Send data back (echo)
    send(new_socket, buffer, strlen(buffer), 0);
    printf("📤 Echo sent back to client.\n");

    // 8. Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}

