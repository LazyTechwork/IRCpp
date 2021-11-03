#include "ServerSocket.h"

ServerSocket::ServerSocket(SocketConnection *connection) {
    this->connection = connection;
}

void ServerSocket::ListenForConnection() {
    SOCKET sock = this->connection->getSocket();
    SOCKET incomingConnection = accept(sock, nullptr, nullptr);
    if (incomingConnection == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return;
    }
    this->clients.push_back(incomingConnection);
}

std::string ServerSocket::ListenForData(int clientId) {
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET Client = this->clients.at(clientId);
    int iResult = recv(Client, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == 0) {
        printf("client #%d closed connection. removing from clients list\n", clientId);
        this->clients.erase(this->clients.begin() + clientId);
    } else
        printf("client #%d sent %d byte(s)\n", clientId, iResult);

    return (recvbuf);
}
