#include <iostream>
#include <SocketConnection.h>
#include <conio.h>

int main() {
    const char PORT[] = "1376";
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateServer(PORT);
    socketConnection->OpenServerConnection();
    _getch();
    socketConnection->CloseConnection();
    delete socketConnection;
    return 0;
}
