#include "ClientSocket.h"

ClientSocket::ClientSocket(SocketConnection *connection) {
    this->connection = connection;
}

std::string ClientSocket::ListenForData() {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult = recv(this->connection->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == 0) {
        this->Shutdown();
        return "Server closed connection";
    }
    return (recvbuf);
}

int ClientSocket::SendData(const std::string &data) {
    int iResult = send(this->connection->getSocket(), data.c_str(), (int) strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        this->Shutdown();
        return -1;
    }
    return iResult;
}

void ClientSocket::Shutdown() {
    this->connection->CloseConnection(true);
}

bool ClientSocket::IsServerAlive() {
    if (this->connection->getSocket() == INVALID_SOCKET || this->SendData("ping") == -1)
        return false;
    return true;
}
