#pragma once

#include <string>
#include <vector>
#include <sstream>


namespace Utils {
    std::vector<std::string> SplitString(const std::string &str, char delimiter = ' ');

    std::string JoinString(const std::vector<std::string>& str, const std::string& separator);
}