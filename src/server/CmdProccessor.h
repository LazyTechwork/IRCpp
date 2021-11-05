#pragma once

#include <ServerSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>

typedef void (*HandlePrint)(std::string msg);

namespace Server {
    class CmdProccessor {
    private:
        ServerSocket *server;
    public:
        explicit CmdProccessor(ServerSocket *server);

        void acceptMessage(int clientId, const std::string& msg, HandlePrint handlePrint);
    };
}