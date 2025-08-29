#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class PauseMenuPopup : public Popup<> {
public:
    TextInput* attemptInput;
    TextInput* jumpInput;
    TextInput* timeInput;

    static constexpr auto popupSize = CCSize{300.f, 270.f};

    static auto create() -> PauseMenuPopup*;

protected:
    auto setup() -> bool override;
    auto ok(CCObject* sender) -> void;
    auto cancel(CCObject* sender) -> void;
    auto plusMenu(CCObject* sender) -> void;
};