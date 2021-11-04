#include <CmdProccessor.h>

Client::CmdProccessor::CmdProccessor(ClientSocket *client) {
    this->client = client;
}

void Client::CmdProccessor::acceptMessage(std::string &msg, HandlePrint &handlePrint) {
    std::vector<std::string> args = Utils::SplitString(msg);
    std::string cmd = args.at(0);
    args.erase(args.begin());
    std::string argMonolyth = Utils::JoinString(args.begin(), args.end(), ' ');
    if (cmd.empty())
        return;
    if (cmd == CMD_PING) {
        this->client->SendData(CMD_PONG);
    } else if (cmd == CMD_CONNECT) {

    }
}
