#include <CmdProccessor.h>

Client::CmdProccessor::CmdProccessor(ClientSocket *client) {
    this->client = client;
}

void Client::CmdProccessor::acceptMessage(std::string &msg, HandlePrint &handlePrint) {
    std::vector<std::string> args = Utils::SplitString(msg);
    std::string cmd = args.at(0);
    args.erase(args.begin());
    if (cmd.empty())
        return;
    if (cmd == CMD_PING) {
        this->client->SendData(CMD_PONG);
    } else if (cmd == CMD_JOIN) {
        handlePrint(args.at(0) + " joined server");
    } else if (cmd == CMD_WHISPER) {
        std::string message = Utils::JoinString(args.begin() + 1, args.end(), ' ');
        handlePrint(args.at(0) + " whispered you: " + message);
    } else if (cmd == CMD_MESSAGE) {
        std::string message = Utils::JoinString(args.begin() + 1, args.end(), ' ');
        handlePrint(args.at(0) + " >> " + message);
    } else if (cmd == CMD_SYSTEM) {
        std::string message = Utils::JoinString(args.begin(), args.end(), ' ');
        handlePrint("SYSTEM: " + message);
    }
}