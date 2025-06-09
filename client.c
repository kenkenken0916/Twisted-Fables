#include "client.h"
#include <stdio.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define close closesocket
#else
    #include <unistd.h>
#endif

static int client_socket;
static const int PORT = 8080;
static const char *ip = "192.168.1.3";

void init_client() {
    struct sockaddr_in server_address;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(ip);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to the server failed");
        exit(EXIT_FAILURE);
    }
}

void receive(game *game_status) {
    #ifdef _WIN32
        int bytes_received = recv(client_socket, (char *)game_status, sizeof(game), 0);
        (void)bytes_received;  // 避免警告
    #else
        ssize_t bytes_received = read(client_socket, (void *)game_status, sizeof(game));
        (void)bytes_received;  // 避免警告
    #endif
}

void send_data(const void *data, size_t size) {
    #ifdef _WIN32
        int bytes_sent = send(client_socket, (const char *)data, (int)size, 0);
        (void)bytes_sent;  // 避免警告
    #else
        ssize_t bytes_sent = write(client_socket, data, size);
        (void)bytes_sent;  // 避免警告
    #endif
}

void destroy_client() { close(client_socket); }