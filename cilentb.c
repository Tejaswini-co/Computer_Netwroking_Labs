#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from Client!";

    // 1. Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set up server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 3. Set server IP (change this to server’s actual IP!)
    if (inet_pton(AF_INET, "192.168.1.15", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // 4. Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 5. Send message
    send(sock, message, strlen(message), 0);
    printf("📨 Sent to server: %s\n", message);

    // 6. Receive echo
    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    printf("🔁 Server replied: %s\n", buffer);

    // 7. Close socket
    close(sock);

    return 0;
}

