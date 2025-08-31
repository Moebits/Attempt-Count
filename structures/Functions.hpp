#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct TimeParseInfo {
    bool hasHours;
    bool hasMinutes;
    bool hasMilliseconds;

    friend auto operator|(const TimeParseInfo& a, const TimeParseInfo& b) -> TimeParseInfo {
        return {
            .hasHours = a.hasHours || b.hasHours,
            .hasMinutes = a.hasMinutes || b.hasMinutes,
            .hasMilliseconds = a.hasMilliseconds || b.hasMilliseconds
        };
    }

    auto operator|=(const TimeParseInfo& other) -> TimeParseInfo& {
        this->hasHours = this->hasHours || other.hasHours;
        this->hasMinutes = this->hasMinutes|| other.hasMinutes;
        this->hasMilliseconds = this->hasMilliseconds || other.hasMilliseconds;
        return *this;
    }
};

class Functions {
public:
    static auto formatPlatformerTime(double rawSeconds) -> gd::string {
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

    static auto formatTime(double rawSeconds) -> gd::string {
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

    static auto parseTime(const std::string& str) -> std::optional<std::pair<double, TimeParseInfo>> {
        if (str.find(":") == std::string::npos) return std::nullopt;
    
        int hours = 0;
        int minutes = 0;
        double seconds = 0.0;
        int count = std::sscanf(str.c_str(), "%d:%d:%lf", &hours, &minutes, &seconds);
    
        auto info = TimeParseInfo{};
    
        if (count == 2) {
            double seconds = hours * 60 + minutes;
            info.hasMinutes = true;
            info.hasMilliseconds = (str.find(".") != std::string::npos);
            return std::make_pair(seconds, info);
        } else if (count == 3) {
            double seconds = hours * 3600 + minutes * 60 + seconds;
            info.hasHours = true;
            info.hasMinutes = true;
            info.hasMilliseconds = (str.find(".") != std::string::npos);
            return std::make_pair(seconds, info);
        }
        return std::nullopt;
    }

    static auto parseDoubleTime(double rawSeconds) -> TimeParseInfo {
        auto info = TimeParseInfo{};
        info.hasMilliseconds = std::fmod(rawSeconds, 1.0) > 1e-6;
        info.hasMinutes = rawSeconds >= 60.0;
        info.hasHours = rawSeconds >= 3600.0;
        return info;
    }

    static auto formatTimeConditional(double rawSeconds, const TimeParseInfo& info) -> std::string {
        int hours = static_cast<int>(rawSeconds / 3600);
        int minutes = static_cast<int>((rawSeconds / 60)) % 60;
        double seconds = std::fmod(rawSeconds, 60.0);
    
        bool showHours = info.hasHours || hours > 0;
        bool showMinutes = info.hasMinutes || showHours || minutes > 0;
    
        if (info.hasMilliseconds) {
            if (showHours) return fmt::format("{}:{:02d}:{:06.3f}", hours, minutes, seconds);
            if (showMinutes) return fmt::format("{}:{:06.3f}", minutes, seconds);
            return fmt::format("{:.3f}", seconds);
        } else {
            if (showHours) return fmt::format("{}:{:02d}:{:02.0f}", hours, minutes, std::floor(seconds + 0.5));
            if (showMinutes) return fmt::format("{}:{:02.0f}", minutes, std::floor(seconds + 0.5));
            return fmt::format("{}", static_cast<int>(std::floor(rawSeconds + 0.5)));
        }
    }

    static auto bindHookToSetting(auto& self, const gd::string& hookName, const gd::string& settingName) -> void {
        auto hookRes = self.getHook(hookName);
        if (!hookRes) return;
        auto* hook = static_cast<Hook*>(hookRes.unwrap());

        auto toggle = [hook](bool enabled) {
            if (enabled) {
                (void) hook->enable();
            } else {
                (void) hook->disable();
            }
        };

        toggle(Mod::get()->getSettingValue<bool>(settingName));
        listenForSettingChanges(settingName, toggle);
    }
};