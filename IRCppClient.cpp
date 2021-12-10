#include <SocketConnection.h>
#include <ClientSocket.h>
#include <thread>
#include <Logger.h>
#include <CmdProcessor.h>
#include <GUIReferences.h>
#include <fstream>

void WriteToConsole(std::string &msg, MessageList *msglist) {
    using namespace ftxui;
    if (msglist->size() + 1 > 30) {
        msglist->pop_front();
    }
    msglist->push_back({Logger::getFormattedTime(), msg});
}

void dataListenThread(ClientSocket *client, MessageList *msglist, bool *isServerDead) {
    printf("%s Started listening for data\n", Logger::getFormattedTime().c_str());
    Client::CmdProcessor cmdProccessor(client);
    while (client->IsServerAlive()) {
        cmdProccessor.acceptMessage(client->ListenForData(), msglist, (HandlePrint) WriteToConsole);
    }
//    printf("%s Server dead, disconnecting\n", Logger::getFormattedTime().c_str());
    *isServerDead = true;
}

int main() {
    setlocale(0, "ru_RU.UTF-8");
    system("chcp 65001");
    system("cls");
    using namespace ftxui;

//  ===  LOGIN SCREEN ===
    auto SCREEN = ScreenInteractive::Fullscreen();

    std::fstream ConfigurationFile;
    ConfigurationFile.open("IRCpp.cfg", std::fstream::in | std::fstream::app);

    std::string nickname;
    std::string ip;
    bool isFieldsDirty = false;
    if (ConfigurationFile) {
        ConfigurationFile >> nickname;
        ConfigurationFile >> ip;
    }
    if (ip.empty())
        ip = "127.0.0.1:1376";
    ConfigurationFile.close();

    auto LoginProceedButton = Button("Log in", SCREEN.ExitLoopClosure());
    Component IpInput, NicknameInput;
    auto NicknameInputOptions = InputOption();
    NicknameInputOptions.on_enter = [&] {
        if (!nickname.empty())
            IpInput->TakeFocus();
    };
    NicknameInputOptions.on_change = [&] {
        isFieldsDirty = true;
    };
    auto IpInputOptions = InputOption();
    IpInputOptions.on_enter = [&] {
        if (!ip.empty()) {
            LoginProceedButton->TakeFocus();
            LoginProceedButton->OnEvent(Event::Return);
        }
    };
    IpInputOptions.on_change = [&] {
        isFieldsDirty = true;
    };
    NicknameInput = Input(&nickname, "", NicknameInputOptions);
    IpInput = Input(&ip, "", IpInputOptions);
    auto LoginScreenComponent = Container::Vertical({NicknameInput, IpInput, LoginProceedButton});
    auto loginScreen = Renderer(LoginScreenComponent, [&] {
        return vbox(
                text(""),
                text("Connect to IRCpp chat") | center,
                text(""),
                separator(),
                hbox(
                        text("    "),
                        vbox(
                                text(""),
                                hbox(text("  Enter nickname      ") | vcenter,
                                     hbox(text("  "), NicknameInput->Render() | size(WIDTH, EQUAL, 25), text("  ")) |
                                     border) | hcenter,
                                hbox(text("  Enter IP (ip:port)  ") | vcenter,
                                     hbox(text("  "), IpInput->Render() | size(WIDTH, EQUAL, 25), text("  ")) |
                                     border) | hcenter,
                                color(Color::LightGreen, LoginProceedButton->Render()) | hcenter
                        ) | flex,
                        text("    ")
                ) | flex
        );
    });

    while (nickname.empty() || ip.empty() || !isFieldsDirty) { SCREEN.Loop(loginScreen); }

    ConfigurationFile.open("IRCpp.cfg", std::fstream::out | std::fstream::trunc);
    ConfigurationFile << nickname << std::endl << ip;
    ConfigurationFile.close();

    bool isServerDead = false;
    auto *socketConnection = new SocketConnection();
    socketConnection->CreateClient(ip.substr(0, ip.find(':')).c_str(), ip.substr(ip.find(':') + 1).c_str());
    auto *client = new ClientSocket(socketConnection);

    MessageList msglist;
    std::thread dataThread(dataListenThread, client, &msglist, &isServerDead);

    client->SendData(Commands[CMD_JOIN] + " " + nickname);

    std::string buf;

    auto SendAction = [&] {
        if (!buf.empty()) {
            client->SendData(Commands[CMD_MESSAGE] + " " + buf);
            buf.clear();
            return true;
        }
        return false;
    };

    auto PartButton = Button("Part", SCREEN.ExitLoopClosure());
    auto OKCloseButton = Button("    OK    ", SCREEN.ExitLoopClosure());
    auto SendButton = Button("Send", SendAction);
    auto MessageInputOptions = InputOption();
//    BUG: Field does not clear
    MessageInputOptions.on_enter = SendAction;
    auto MessageInput = Input(&buf, "", MessageInputOptions);
    auto ChatScreenComponent = Container::Vertical({MessageInput, SendButton, PartButton, OKCloseButton});
    auto chatScreen = Renderer(ChatScreenComponent, [&] {

        if (isServerDead) {
            return vbox(
                    text(""),
                    text(""),
                    vbox(
                            text(""),
                            color(Color::Red, text("Cannot connect to server!")) | center,
                            color(Color::GrayLight,
                                  text("    Please check is server alive and reconnect manually    ")) |
                            center,
                            text(""),
                            color(Color::BlueLight, OKCloseButton->Render()) | center,
                            text("")
                    ) | border | center,
                    text(""),
                    text(""));
        }

        Elements chatbox;
        for (auto &message: msglist)
            chatbox.push_back(renderChatMessage(message));

        return vbox(
                text(""),
                text("Connected to IRCpp Chat (" + ip + ") as " + nickname) | center,
                separator(),
                hbox(text("    "),
                     vbox(std::move(chatbox)) | frame | size(HEIGHT, LESS_THAN, 30) | size(HEIGHT, GREATER_THAN, 15) |
                     border | flex,
                     text("    ")),
                hbox(text("    "), hbox(text("  "), MessageInput->Render(), text("  ")) | flex | border,
                     SendButton->Render(),
                     color(Color::Red, PartButton->Render()),
                     text("    "))
        ) | flex;
    });

    SCREEN.Loop(chatScreen);

    client->SendData(Commands[CMD_PART]);
    client->Shutdown();
    //    Terminating threads
    if (dataThread.joinable())
        dataThread.join();
    return EXIT_SUCCESS;
}
