#include <CmdProcessor.h>

Client::CmdProcessor::CmdProcessor(ClientSocket *client) {
    this->client = client;
}

void Client::CmdProcessor::acceptMessage(const std::string& msg, HandlePrint handlePrint) {
    std::vector<std::string> args = Utils::SplitString(msg);
    if (StringCommands.find(args.at(0)) == StringCommands.end())
        return;
    int cmd = StringCommands.at(args.at(0));
    args.erase(args.begin());
    if (cmd == CMD_PING) {
        this->client->SendData(Commands[CMD_PONG]);
    } else if (cmd == CMD_JOIN) {
        handlePrint(args.at(0) + " joined server");
    } else if (cmd == CMD_WHISPER) {
        std::string message = Utils::JoinString(std::vector(args.begin() + 1, args.end()), " ");
        handlePrint(args.at(0) + " whispered you: " + message);
    } else if (cmd == CMD_MESSAGE) {
        std::string message = Utils::JoinString(std::vector(args.begin() + 1, args.end()), " ");
        handlePrint(args.at(0) + " >> " + message);
    } else if (cmd == CMD_SYSTEM) {
        std::string message = Utils::JoinString(args, " ");
        handlePrint("SYSTEM: " + message);
    }
}
