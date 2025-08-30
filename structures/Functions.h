#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Functions {
public:
    static auto formatSeconds(double rawSeconds) -> gd::string;
    static auto formatTime(double rawSeconds) -> gd::string;

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