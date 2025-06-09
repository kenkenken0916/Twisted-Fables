#ifndef CLIENT_H
#define CLIENT_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

#include <stddef.h>
#include "game_state.h"

void init_client();
void receive(game *game_status);
void send_data(const void *data, size_t size);  // 修改為 const void*
void destroy_client();

#endif /* CLIENT_H */