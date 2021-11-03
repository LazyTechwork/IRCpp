#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")

class SocketConnection {
private:
    SOCKET Socket;
    int maxConnections = 0;
public:
    SocketConnection();

    int CreateServer(const char *port);

    int ListenServer(int _maxConnections = 0);

    void CloseConnection();

    [[nodiscard]] SOCKET getSocket() const;
};
