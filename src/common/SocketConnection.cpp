#include "SocketConnection.h"

SocketConnection::SocketConnection() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
    Socket = INVALID_SOCKET;
}

int SocketConnection::CreateServer(const char *port) {
    struct addrinfo *result = nullptr, *ptr = nullptr, sockaddr{};

    ZeroMemory(&sockaddr, sizeof(sockaddr));
    sockaddr.ai_family = AF_INET;
    sockaddr.ai_socktype = SOCK_STREAM;
    sockaddr.ai_protocol = IPPROTO_TCP;
    sockaddr.ai_flags = AI_PASSIVE;

    // Resolving the local address and port to be used by the server
    int iResult = getaddrinfo(nullptr, port, &sockaddr, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (Socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(Socket, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    return 0;
}

int SocketConnection::ListenServer(int _maxConnections) {
    this->maxConnections = _maxConnections ? _maxConnections : SOMAXCONN;
    if (listen(Socket, this->maxConnections) == SOCKET_ERROR) {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }
    return 0;
}

SOCKET SocketConnection::getSocket() const {
    return Socket;
}

void SocketConnection::CloseConnection() {
    closesocket(this->Socket);
    this->Socket = INVALID_SOCKET;
    WSACleanup();
}
