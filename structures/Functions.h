#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Functions {
public:
    static auto formatSeconds(double rawSeconds) -> gd::string;
    static auto formatTime(double rawSeconds) -> gd::string;
};