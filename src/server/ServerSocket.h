#pragma once

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <SocketConnection.h>
#include <SocketConstants.h>
#include <vector>
#include <winsock2.h>
#include <cstdio>
#include <string>

struct ServerClient {
    SOCKET client;
    bool isAlive;
    int color = 0xF0;
    std::string nickname;
};

class ServerSocket {
private:
    SocketConnection *connection;
    std::vector<ServerClient> clients = {};
public:
    explicit ServerSocket(SocketConnection *connection);
    const std::vector<ServerClient> &getClients() const;

    /**
     * Remove client from vector without shutdowning and closing connection
     * */
    void DeleteClient(int clientId);

    /**
     * Listen for next connection. Recommended to run in separate thread.
     * Functions adds new client automatically in clients vector.
     */
    int WaitForConnection();

    /**
     * Listen for next bytes on specified connection. Recommended to run in separate thread.
     *
     * @param clientId describes what client listen to
     * @return std::string data
     */
    std::string ListenForData(int clientId);

    /**
     * Listen for next bytes on specified connection. Recommended to run in separate thread.
     *
     * @param clientId describes what client listen to
     * @return std::string data
     */
    int SendData(int clientId, const std::string &data);

    /**
     * Listen for next bytes on specified connection. Recommended to run in separate thread.
     *
     * @param clientId describes what client listen to
     * @return std::string data
     */
    int Broadcast(const std::string &data, int excluding = -1);

    /**
     * Checking is client alive
     * */
    bool IsClientAlive(int clientId);

    /**
     * Checking is server socket alive
     * */
    bool IsSocketAlive();

    /**
     * Pinging all clients
     * */
    void PingAllClients();

    void setClientNickname(int clientId, std::string nickname);
};
