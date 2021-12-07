#pragma once

#include <ClientSocket.h>
#include <Utils.h>
#include <CommandDefinitions.h>
#include "ftxui/component/component_base.hpp"
#include "GUIReferences.h"

typedef void (*HandlePrint)(std::string msg, MessageList *msglist);

namespace Client {
    class CmdProcessor {
    private:
        ClientSocket *client;
    public:
        explicit CmdProcessor(ClientSocket *client);

        void acceptMessage(const std::string &msg, MessageList *msglist, HandlePrint handlePrint);
    };
}