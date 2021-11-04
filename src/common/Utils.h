#pragma once

#include <string>
#include <vector>
#include <sstream>

typedef void (* HandlePrint)(std::string &msg);

namespace Utils {
    std::vector<std::string> SplitString(std::string &str, char delimiter = ' ');

    std::string JoinString(Iter begin, Iter end, const std::string &separator);
}