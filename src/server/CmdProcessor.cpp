#include <CmdProcessor.h>

Server::CmdProcessor::CmdProcessor(ServerSocket *server) {
    this->server = server;
}

void Server::CmdProcessor::acceptMessage(int clientId, const std::string &msg, HandlePrint handlePrint) {
    if (msg.empty())
        return;
    std::vector<std::string> args = Utils::SplitString(msg);
    if (StringCommands.find(args.at(0)) == StringCommands.end())
        return;
    int cmd = StringCommands.at(args.at(0));
    if (!this->server->IsClientAlive(clientId))
        return;
    ServerClient clientInfo = this->server->getClients().at(clientId);
    args.erase(args.begin());
    if (cmd == CMD_JOIN) {
        this->server->setClientNickname(clientId, args.at(0));
        this->server->Broadcast(Commands[CMD_JOIN] + " " + args.at(0), clientId);
        handlePrint(args.at(0) + " joined server");
// TODO: First two words splits
    } else if (cmd == CMD_MESSAGE) {
        std::string message = Utils::JoinString(std::vector(args.begin(), args.end()), " ");
        this->server->Broadcast(Commands[CMD_MESSAGE] + " " + clientInfo.nickname + " " + message);
        handlePrint(clientInfo.nickname + " >> " + message);
    }
}
