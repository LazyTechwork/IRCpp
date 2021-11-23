#include <SocketConnection.h>
#include <conio.h>
#include <ClientSocket.h>
#include <thread>
#include <Logger.h>
#include <iostream>
#include <CmdProcessor.h>

void WriteToConsole(std::string &msg) {
    printf("%s %s\n", Logger::getFormattedTime().c_str(), msg.c_str());
}

void dataListenThread(ClientSocket *client) {
    printf("%s Started listening for data\n", Logger::getFormattedTime().c_str());
    Client::CmdProcessor cmdProccessor(client);
    while (client->IsServerAlive()) {
        cmdProccessor.acceptMessage(client->ListenForData(), (HandlePrint) WriteToConsole);
    }
    printf("%s Server dead, disconnecting\n", Logger::getFormattedTime().c_str());
    exit(0);
}

int main() {
    setlocale(0, "ru_RU.UTF-8");
    system("chcp 65001");
    system("cls");

    std::string nickname;
    std::cout << "Enter nickname: ";
    std::cin >> nickname;
    std::cout << std::endl;

    const char IP[] = "127.0.0.1";
    const char PORT[] = "1376";
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateClient(IP, PORT);
    auto *client = new ClientSocket(socketConnection);

    printf("%s Creating threads for listening...\n", Logger::getFormattedTime().c_str());
    std::thread dataThread(dataListenThread, client);

    client->SendData(Commands[CMD_JOIN] + " " + nickname);

    std::string buf;
    std::getline(std::cin, buf);
    while (buf != "!quit") {
        if (!buf.empty())
            client->SendData(Commands[CMD_MESSAGE] + " " + buf);
        std::getline(std::cin, buf);
    }

    _getch();
    client->SendData(Commands[CMD_PART]);
    //    Terminating threads
    dataThread.detach();
    TerminateThread((HANDLE) dataThread.native_handle(), 0);
    return 0;
}
