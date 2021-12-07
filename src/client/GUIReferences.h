#pragma once

#include <list>
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"

struct ChatMessage {
    std::string timestamp;
    std::string msg;
};
using MessageList = std::list<ChatMessage>;

auto renderChatMessage = [](const ChatMessage &msg) {
    return hbox(
            ftxui::text("    "),
            ftxui::color(ftxui::Color::GrayDark, ftxui::text(msg.timestamp)),
            ftxui::text("  "),
            ftxui::color(ftxui::Color::White, ftxui::text(msg.msg)),
            ftxui::text("    ")
    );
};
