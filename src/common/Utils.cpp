#include "Utils.h"

std::vector<std::string> Utils::SplitString(const std::string &str, char delimiter) {
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(test, segment, delimiter))
        seglist.push_back(segment);

    return seglist;
}

std::string Utils::JoinString(const std::vector<std::string> &str, const std::string &separator) {
    std::ostringstream result;
    auto begin = str.begin();
    auto end = str.end();
    if (begin != end)
        result << *begin++;
    while (begin != end)
        result << separator << *begin++;
    return (result.str());
}
