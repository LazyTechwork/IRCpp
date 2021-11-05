#include <ServerSocket.h>
#include <Logger.h>
#include <CommandDefinitions.h>

ServerSocket::ServerSocket(SocketConnection *connection) {
    this->connection = connection;
}

int ServerSocket::WaitForConnection() {
    SOCKET sock = this->connection->getSocket();
    SOCKET incomingConnection = accept(sock, nullptr, nullptr);
    if (incomingConnection == INVALID_SOCKET) {
        printf("%s Accept failed: %d\n", Logger::getFormattedTime().c_str(), WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    this->clients.push_back({incomingConnection, true});
    return (int) this->clients.size() - 1;
}

std::string ServerSocket::ListenForData(int clientId) {
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET Client = this->clients.at(clientId).client;
    if (Client == 0 || !this->clients.at(clientId).isAlive)
        return "";
    int iResult = recv(Client, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == -1) {
        printf("%s Client #%d closed connection. removing from clients list\n", Logger::getFormattedTime().c_str(),
               clientId);
        this->DeleteClient(clientId);
    } else
        printf("%s Client #%d sent %d byte(s)\n", Logger::getFormattedTime().c_str(), clientId, iResult);

    std::string result(recvbuf);
    if (iResult > 0 && result.size() > iResult)
        result.resize(iResult);
    return result;
}

int ServerSocket::SendData(int clientId, const std::string &data) {
    SOCKET Client = this->clients.at(clientId).client;
    if (Client == 0)
        return 0;
    int iResult = send(Client, data.c_str(), (int) strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        printf("%s Client #%d closed connection. removing from clients list\n", Logger::getFormattedTime().c_str(),
               clientId);
        this->DeleteClient(clientId);
        return 0;
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
    *(this->clients.begin() + clientId) = {0, false};
}

bool ServerSocket::IsClientAlive(int clientId) {
    return this->clients.at(clientId).isAlive;
}

bool ServerSocket::IsSocketAlive() {
    return this->connection->getSocket() != INVALID_SOCKET;
}

void ServerSocket::PingAllClients() {
    for (int i = 0, l = (int) this->clients.size(); i < l; ++i) {
        if (this->clients.at(i).isAlive)
            this->SendData(i, Commands[CMD_PING]);
    }
}

const std::vector<ServerClient> &ServerSocket::getClients() const {
    return clients;
}

void ServerSocket::setClientNickname(int clientId, std::string nickname) {
    this->clients.at(clientId).nickname = nickname;
}

