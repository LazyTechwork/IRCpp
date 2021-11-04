#pragma once

#include <ClientSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>

namespace Client {
    class CmdProccessor {
    private:
        ClientSocket *client;
    public:
        explicit CmdProccessor(ClientSocket *client);

        void acceptMessage(std::string &msg, HandlePrint &handlePrint);
    };
}