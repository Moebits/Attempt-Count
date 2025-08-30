#include "Functions.h"

auto Functions::formatSeconds(double rawSeconds) -> gd::string {
    int total = static_cast<int>(rawSeconds);
    int hours = total / 3600;
    int minutes = (total / 60) % 60;
    int seconds = total % 60;
    int milliseconds = static_cast<int>(std::round((rawSeconds - total) * 1000));

    if (milliseconds == 1000) { milliseconds = 0; if (++seconds == 60) { seconds = 0; if (++minutes == 60) { minutes = 0; ++hours; }}}

    if (hours > 0) {
        return fmt::format("{}:{:02}:{:02}.{:03}", hours, minutes, seconds, milliseconds).c_str();
    } else if (minutes > 0) {
        return fmt::format("{}:{:02}.{:03}", minutes, seconds, milliseconds).c_str();
    } else {
        return fmt::format("{}.{}", seconds, fmt::format("{:03}", milliseconds)).c_str();
    }
}

auto Functions::formatTime(double rawSeconds) -> gd::string {
    int total = static_cast<int>(std::round(rawSeconds));
    int hours = total / 3600;
    int minutes = (total / 60) % 60;
    int seconds = total % 60;

    if (hours > 0) {
        return fmt::format("{:02}:{:02}:{:02}", hours, minutes, seconds).c_str();
    } else {
        return fmt::format("{:02}:{:02}", minutes, seconds).c_str();
    }
}