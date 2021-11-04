#include <SocketConnection.h>
#include <conio.h>
#include <ServerSocket.h>
#include <thread>
#include <Logger.h>

void dataListenThread(ServerSocket *server, int clientId) {
    printf("%s Started listening for data of client #%d\n", Logger::getFormattedTime().c_str(), clientId);
    while (server->IsClientAlive(clientId)) {
        printf("%s Client #%d >> %s\n", Logger::getFormattedTime().c_str(), clientId,
               server->ListenForData(clientId).c_str());
        Sleep(150);
    }
    printf("%s Client #%d dead, stopping listening for data\n", Logger::getFormattedTime().c_str(), clientId);
}

void connectionsListenThread(ServerSocket *server, std::vector<std::thread *> *dataListenThreads) {
    printf("%s Started connections listening\n", Logger::getFormattedTime().c_str());
    while (server->IsSocketAlive()) {
        int clientId = server->WaitForConnection();
        server->SendData(clientId, "Successfully connected!");
        std::thread dlt(dataListenThread, server, clientId);
        dlt.detach();
        dataListenThreads->push_back(&dlt);
        Sleep(150);
    }
    printf("%s Connections listening stopped\n", Logger::getFormattedTime().c_str());
}

void pingThread(ServerSocket *server) {
    while (server->IsSocketAlive()) {
        server->PingAllClients();
        Sleep(5000);
    }
}

int main() {
    setlocale(0, "ru_RU.UTF-8");
    system("chcp 65001");
    system("cls");

    const char PORT[] = "1376";
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateServer(PORT);
    socketConnection->OpenServerConnection();
    auto *server = new ServerSocket(socketConnection);

    printf("%s Creating threads for listening...\n", Logger::getFormattedTime().c_str());
    std::vector<std::thread *> dataListenThreads;
    std::thread connectionsThread(connectionsListenThread, server, &dataListenThreads);
    std::thread pinger(pingThread, server);

    _getch();
//    Terminating threads
    connectionsThread.detach();
    pinger.detach();
    TerminateThread((HANDLE) pinger.native_handle(), 0);
    TerminateThread((HANDLE) connectionsThread.native_handle(), 0);
    for (auto &thread: dataListenThreads)
        TerminateThread((HANDLE) thread->native_handle(), 0);

//    Closing socket connection
    socketConnection->CloseConnection();
    return 0;
}
