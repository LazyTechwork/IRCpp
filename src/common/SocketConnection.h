#pragma once

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>

class SocketConnection {
private:
    SOCKET Socket;
    int maxConnections = 0;
public:
    SocketConnection();

    /**
     * Creates connection suitable for server
     *
     * @param port of server
     * @param ip of server
     * @return int connection creation status
     */
    int CreateServer(const char *port, const char *ip = "localhost");

    /**
     * Creates connection suitable for client
     *
     * @param ip of server
     * @param port of server
     * @return int connection creation status
     */
    int CreateClient(const char *ip, const char *port);

    /**
     * Open port and listen on it
     *
     * @param _maxConnections if zero then default value applied
     * @return int status
     */
    int OpenServerConnection(int _maxConnections = 0);

    /**
     * Closes connection on socket
     */
    void CloseConnection(bool isClient = false);

    [[nodiscard]] SOCKET getSocket() const;
};
