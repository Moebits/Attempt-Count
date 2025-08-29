#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class AppendPopup : public Popup<TextInput*, bool> {
public:
    TextInput* targetInput;
    bool floatFilter;
    TextInput* appendInput;

    static constexpr auto popupSize = CCSize{320.f, 200.f};

    static auto create(TextInput* targetInput, bool floatFilter) -> AppendPopup*;

protected:
    auto setup(TextInput* targetInput, bool floatFilter) -> bool override;
    auto ok(CCObject* sender) -> void;
    auto cancel(CCObject* sender) -> void;
};