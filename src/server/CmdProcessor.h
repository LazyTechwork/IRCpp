#pragma once

#include <ServerSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>

typedef void (*HandlePrint)(std::string msg);

namespace Server {
    class CmdProcessor {
    private:
        ServerSocket *server;
    public:
        explicit CmdProcessor(ServerSocket *server);

        void acceptMessage(int clientId, const std::string& msg, HandlePrint handlePrint);
    };
}