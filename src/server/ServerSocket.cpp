#include <ServerSocket.h>

ServerSocket::ServerSocket(SocketConnection *connection) {
    this->connection = connection;
}

int ServerSocket::WaitForConnection() {
    SOCKET sock = this->connection->getSocket();
    SOCKET incomingConnection = accept(sock, nullptr, nullptr);
    if (incomingConnection == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    this->clients.push_back(incomingConnection);
    return (int) this->clients.size() - 1;
}

std::string ServerSocket::ListenForData(int clientId) {
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET Client = this->clients.at(clientId);
    int iResult = recv(Client, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == 0) {
        printf("client #%d closed connection. removing from clients list\n", clientId);
        this->DeleteClient(clientId);
    } else
        printf("client #%d sent %d byte(s)\n", clientId, iResult);

    return (recvbuf);
}

int ServerSocket::SendData(int clientId, const std::string &data) {
    SOCKET Client = this->clients.at(clientId);
    if (Client == 0)
        return -1;
    int iResult = send(Client, data.c_str(), (int) strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        printf("client #%d closed connection. removing from clients list\n", clientId);
        this->DeleteClient(clientId);
        return -1;
    }
    return iResult;
}

int ServerSocket::Broadcast(const std::string &data, int excluding) {
    int bytes_sent = 0;
    for (int i = 0, l = (int) this->clients.size(); i < l; ++i)
        if (i == excluding)
            continue;
        else
            bytes_sent += this->SendData(i, data);

    return 0;
}

void ServerSocket::DeleteClient(int clientId) {
    *(this->clients.begin() + clientId) = 0;
    printf("client #%d removed from clients list\n", clientId);
}

bool ServerSocket::IsClientAlive(int clientId) {
    if (this->SendData(clientId, "ping") == -1)
        return false;
    return true;
}

bool ServerSocket::IsSocketAlive() {
    return this->connection->getSocket() != INVALID_SOCKET;
}
