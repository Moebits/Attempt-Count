#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LevelStatsPopup : public Popup<GJGameLevel*, FLAlertLayer*> {
public:
    GJGameLevel* level;
    FLAlertLayer* alert;
    TextInput* attemptsInput;
    TextInput* jumpsInput;

    static constexpr auto popupSize = CCSize{300.f, 250.f};

    static auto create(GJGameLevel* level, FLAlertLayer* alert) -> LevelStatsPopup*;

protected:
    auto setup(GJGameLevel* level, FLAlertLayer* alert) -> bool override;
    auto ok(CCObject* sender) -> void;
    auto cancel(CCObject* sender) -> void;
    auto plusMenu(CCObject* sender) -> void;
};