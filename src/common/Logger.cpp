#include "Logger.h"

namespace Logger {
    std::string getFormattedTime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "[%H:%M:%S]");
        return oss.str();
    }
    std::string getFormattedDatetime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S]");
        return oss.str();
    }
}
