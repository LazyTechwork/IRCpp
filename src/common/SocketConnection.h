#pragma once

#pragma comment(lib, "Ws2_32.lib")

class SocketConnection {
private:
    SOCKET Socket;
    int maxConnections = 0;
public:
    SocketConnection();

    int CreateServer(const char *port);

    int ListenServer(int _maxConnections = 0);
};
