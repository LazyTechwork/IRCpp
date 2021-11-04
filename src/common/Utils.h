#pragma once

#include <string>
#include <vector>
#include <sstream>


namespace Utils {
    std::vector<std::string> SplitString(std::string &str, char delimiter = ' ');

    template <typename Iter>
    std::string JoinString(Iter begin, Iter end, const std::string &separator);
}