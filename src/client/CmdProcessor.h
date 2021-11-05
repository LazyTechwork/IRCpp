#pragma once

#include <ClientSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>

typedef void (*HandlePrint)(std::string msg);

namespace Client {
    class CmdProcessor {
    private:
        ClientSocket *client;
    public:
        explicit CmdProcessor(ClientSocket *client);

        void acceptMessage(const std::string& msg, HandlePrint handlePrint);
    };
}