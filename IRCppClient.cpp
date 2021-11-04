#include <SocketConnection.h>
#include <conio.h>
#include <ClientSocket.h>
#include <thread>
#include <Logger.h>
#include <iostream>

void dataListenThread(ClientSocket *client) {
    printf("%s Started listening for data\n", Logger::getFormattedTime().c_str());
    while (client->IsServerAlive()) {
        printf("%s >> %s\n", Logger::getFormattedTime().c_str(), client->ListenForData().c_str());
    }
    printf("%s Server dead, disconnecting\n", Logger::getFormattedTime().c_str());
}

int main() {
    const char IP[] = "127.0.0.1";
    const char PORT[] = "1376";
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateClient(IP, PORT);
    auto *client = new ClientSocket(socketConnection);
    std::string buf;
    std::cin >> buf;
    client->SendData(buf);
    printf("%s Creating threads for listening...\n", Logger::getFormattedTime().c_str());
    std::thread dataThread(dataListenThread, client);
    dataThread.detach();

    _getch();
    //    Terminating threads
    TerminateThread((HANDLE) dataThread.native_handle(), 0);
    return 0;
}
