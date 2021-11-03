#pragma once

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <SocketConnection.h>
#include <SocketConstants.h>
#include <string>

class ClientSocket {
private:
    SocketConnection *connection;
public:
    explicit ClientSocket(SocketConnection *connection);

    /**
     * Listen for next bytes on connection. Recommended to run in separate thread.
     *
     * @return std::string data
     */
    std::string ListenForData();

    /**
     * Send data to server
     *
     * @param std::string data
     * @return int bytes sent
     */
    int SendData(const std::string& data);

    /**
     * Shutdown client and disconnect
     */
    void Shutdown();

};
