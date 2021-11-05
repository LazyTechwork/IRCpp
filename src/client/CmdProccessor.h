#pragma once

#include <ClientSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>

typedef void (*HandlePrint)(std::string msg);

namespace Client {
    class CmdProccessor {
    private:
        ClientSocket *client;
    public:
        explicit CmdProccessor(ClientSocket *client);

        void acceptMessage(const std::string& msg, HandlePrint handlePrint);
    };
}