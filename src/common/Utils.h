#pragma once

#include <string>
#include <vector>
#include <sstream>


namespace Utils {
    std::vector<std::string> SplitString(const std::string &str, char delimiter = ' ');

    template <typename I>
    std::string JoinString(I begin, I end, std::string separator);
}