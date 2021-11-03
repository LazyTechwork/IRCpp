#include <SocketConnection.h>
#include <conio.h>
#include <ServerSocket.h>
#include <thread>

[[noreturn]] void dataListenThread(ServerSocket *server, int clientId) {
    printf("Started listening for data of client #%d\n", clientId);
    while (server->IsClientAlive(clientId))
        printf("%s\n", server->ListenForData(clientId).c_str());
    printf("Client #%d dead, stopping listening for data\n", clientId);
}

[[noreturn]] void connectionsListenThread(ServerSocket *server, std::vector<std::thread *> *dataListenThreads) {
    printf("Started connections listening\n");
    while (server->IsSocketAlive()) {
        int clientId = server->WaitForConnection();
        std::thread dlt(dataListenThread, server, clientId);
        dlt.join();
        dataListenThreads->push_back(&dlt);
    }
    printf("Connections listening stopped\n");
}

int main() {
    const char PORT[] = "1376";
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateServer(PORT);
    socketConnection->OpenServerConnection();
    auto *server = new ServerSocket(socketConnection);

    printf("Creating threads for listening...\n");
    std::vector<std::thread *> dataListenThreads;
    std::thread connsThread(connectionsListenThread, server, &dataListenThreads);

    connsThread.join();

    _getch();
    socketConnection->CloseConnection();
    delete socketConnection;
    delete server;
    return 0;
}
