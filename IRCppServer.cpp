#include <SocketConnection.h>
#include <conio.h>
#include <ServerSocket.h>
#include <thread>
#include <Logger.h>
#include <CmdProcessor.h>

void WriteToConsole(std::string &msg) {
    printf("%s %s\n", Logger::getFormattedTime().c_str(), msg.c_str());
}

void dataListenThread(ServerSocket *server, int clientId) {
    printf("%s Started listening for data of client #%d\n", Logger::getFormattedTime().c_str(), clientId);
    Server::CmdProcessor cmdProccessor(server);
    while (server->IsClientAlive(clientId)) {
        cmdProccessor.acceptMessage(clientId, server->ListenForData(clientId), (HandlePrint) WriteToConsole);
        Sleep(150);
    }
    printf("%s Client #%d dead, stopping listening for data\n", Logger::getFormattedTime().c_str(), clientId);
}

void connectionsListenThread(ServerSocket *server, std::vector<std::thread *> *dataListenThreads) {
    printf("%s Started connections listening\n", Logger::getFormattedTime().c_str());
    while (server->IsSocketAlive()) {
        int clientId = server->WaitForConnection();
        if (clientId < 0)
            return;
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
//        server->PingAllClients();
        Sleep(25000);
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
    TerminateThread((HANDLE) pinger.native_handle(), 0);
    TerminateThread((HANDLE) connectionsThread.native_handle(), 0);
    for (auto &thread: dataListenThreads)
        TerminateThread((HANDLE) thread->native_handle(), 0);

//    Closing socket connection
    socketConnection->CloseConnection();
    system("pause");
    return 0;
}
