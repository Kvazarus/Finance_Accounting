#ifndef KPO2_HELPERS_H
#define KPO2_HELPERS_H

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

inline std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}


constexpr int UNCATEGORIZED_ID = 0;

#endif //KPO2_HELPERS_H
