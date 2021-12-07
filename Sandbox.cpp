
int main() {
    auto SCREEN = ScreenInteractive::TerminalOutput();
    auto closeScreenButton = Button("OK", SCREEN.ExitLoopClosure());

    std::string nickname;
    std::string ip = "127.0.0.1:1376";

    Component inputNickname = Input(&nickname, "");
    Component inputIp = Input(&ip, "");
    auto component = Container::Vertical({inputNickname, inputIp, closeScreenButton});
    auto loginScreen = Renderer(component, [&] {
        return window(
                "Login",
                vbox({
                             text("IRCpp chat") | center,
                             separator(),
                             hbox(text("  Enter nickname  "), inputNickname->Render()),
                             hbox(text("  Enter IP        "), inputIp->Render()),
                             closeScreenButton->Render()
                     }));
    });

    SCREEN.Loop(loginScreen);
    std::cout << nickname << std::endl << ip << std::endl;
    _getch();
    return 0;
}