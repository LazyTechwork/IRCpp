#pragma once

#include <map>

#define CMD_MESSAGE 0
#define CMD_SYSTEM 1
#define CMD_JOIN 2
#define CMD_PART 3
#define CMD_COLOR 4
#define CMD_WHISPER 5
#define CMD_PING 6
#define CMD_PONG 7

#define StringCommandsIterator std::map<std::string, int>::iterator

const std::string Commands[]{"MSG", "SYS", "JOIN", "PART", "COLOR", "WHISPER", "PING", "PONG"};
const std::map<std::string, int> StringCommands = {
        {Commands[CMD_MESSAGE], CMD_MESSAGE},
        {Commands[CMD_SYSTEM],  CMD_SYSTEM},
        {Commands[CMD_JOIN],    CMD_JOIN},
        {Commands[CMD_PART],    CMD_PART},
        {Commands[CMD_COLOR],   CMD_COLOR},
        {Commands[CMD_WHISPER], CMD_WHISPER},
        {Commands[CMD_PING],    CMD_PING},
        {Commands[CMD_PONG],    CMD_PONG}
};


