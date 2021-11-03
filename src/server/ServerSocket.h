#pragma once

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 2048

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

    /**
     * Listen for next connection. Recommended to run in separate thread.
     * Functions adds new client automatically in clients vector.
     */
    void ListenForConnection();

    /**
     * Listen for next bytes on specified connection. Recommended to run in separate thread.
     *
     * @param clientId describes what client listen to
     * @return std::string data
     */
    std::string ListenForData(int clientId);
};
