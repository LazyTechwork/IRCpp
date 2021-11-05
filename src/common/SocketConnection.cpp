#include "SocketConnection.h"
#include "Logger.h"

SocketConnection::SocketConnection() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("%s WSAStartup failed: %d\n", Logger::getFormattedTime().c_str(), iResult);
    }
    Socket = INVALID_SOCKET;
}

int SocketConnection::CreateServer(const char *port, const char *ip) {
    struct addrinfo *result = nullptr, sockinfo{};

    ZeroMemory(&sockinfo, sizeof(sockinfo));
    sockinfo.ai_family = AF_INET;
    sockinfo.ai_socktype = SOCK_STREAM;
    sockinfo.ai_protocol = IPPROTO_TCP;
    sockinfo.ai_flags = AI_PASSIVE;

    // Resolving the local address and port to be used by the server
    int iResult = getaddrinfo(ip, port, &sockinfo, &result);
    printf("%s Resolving address...\n", Logger::getFormattedTime().c_str());
    if (iResult != 0) {
        printf("%s getaddrinfo failed: %d\n", Logger::getFormattedTime().c_str(), iResult);
        WSACleanup();
        return 1;
    }

    printf("%s Opening socket...\n", Logger::getFormattedTime().c_str());
    Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (Socket == INVALID_SOCKET) {
        printf("%s Error at socket(): %ld\n", Logger::getFormattedTime().c_str(), WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    printf("%s Binding IP address...\n", Logger::getFormattedTime().c_str());
    iResult = bind(Socket, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        printf("%s bind failed with error: %d\n", Logger::getFormattedTime().c_str(), WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    /*printf("%s Server binded at %s:%s\n", Logger::getFormattedTime().c_str(),
           inet_ntoa((struct in_addr) ((struct sockaddr_in *) result->ai_addr)->sin_addr), port);*/

    return 0;
}

int SocketConnection::OpenServerConnection(int _maxConnections) {
    this->maxConnections = _maxConnections ? _maxConnections : SOMAXCONN;
    if (listen(Socket, this->maxConnections) == SOCKET_ERROR) {
        printf("%s Listen failed with error: %ld\n", Logger::getFormattedTime().c_str(), WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }
    printf("%s Server started listening\n", Logger::getFormattedTime().c_str());
    return 0;
}

SOCKET SocketConnection::getSocket() const {
    return Socket;
}

void SocketConnection::CloseConnection(bool isClient) {
    if (isClient)
        shutdown(this->Socket, SD_SEND);

    closesocket(this->Socket);
    this->Socket = INVALID_SOCKET;
    WSACleanup();
}

int SocketConnection::CreateClient(const char *ip, const char *port) {
    struct addrinfo *result = nullptr, *ptr = nullptr, sockinfo{};

    ZeroMemory(&sockinfo, sizeof(sockinfo));
    sockinfo.ai_family = AF_UNSPEC;
    sockinfo.ai_socktype = SOCK_STREAM;
    sockinfo.ai_protocol = IPPROTO_TCP;

    printf("%s Resolving address...\n", Logger::getFormattedTime().c_str());
    int iResult = getaddrinfo(ip, port, &sockinfo, &result);
    if (iResult != 0) {
        printf("%s getaddrinfo failed: %d\n", Logger::getFormattedTime().c_str(), iResult);
        WSACleanup();
        return 1;
    }

    ptr = result;
    printf("%s Opening socket...\n", Logger::getFormattedTime().c_str());
    this->Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (this->Socket == INVALID_SOCKET) {
        printf("%s Error at socket(): %ld\n", Logger::getFormattedTime().c_str(), WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    printf("%s Connecting...\n", Logger::getFormattedTime().c_str());
    iResult = connect(this->Socket, ptr->ai_addr, (int) ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(this->Socket);
        this->Socket = INVALID_SOCKET;
    }

    printf("%s Server listening at %s:%s\n", Logger::getFormattedTime().c_str(),
           inet_ntoa((struct in_addr) ((struct sockaddr_in *) result->ai_addr)->sin_addr),
           port);
    freeaddrinfo(result);

    if (this->Socket == INVALID_SOCKET) {
        printf("%s Unable to connect to server!\n", Logger::getFormattedTime().c_str());
        WSACleanup();
        return 1;
    }

    return 0;
}

