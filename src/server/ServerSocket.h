#pragma once

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

#include <SocketConnection.h>
#include <vector>
#include <winsock2.h>
#include <cstdio>
#include <string>

class ServerSocket {
private:
    SocketConnection *connection;
    std::vector<SOCKET> clients = {};
public:
    explicit ServerSocket(SocketConnection *connection);
    void ListenForConnection();
    std::string ListenForData(int clientId);
};
