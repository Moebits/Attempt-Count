#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

struct ButtonParameters : public CCObject {
    GJGameLevel* level;
    FLAlertLayer* alert;

    ButtonParameters(GJGameLevel* level, FLAlertLayer* alert) 
        : level(level), alert(alert) {
        this->autorelease();
    }
};

class SharedStatsPanel {
public:
    auto addButton(FLAlertLayer* alert, GJGameLevel* level) -> void;
    auto buttonPress(CCObject* sender) -> void;
};

static auto statsPanel = SharedStatsPanel{};

class $modify(ModifyEditLevelLayer, EditLevelLayer) {
public:
    static auto onModify(auto& self) -> void;
    auto onLevelInfo(CCObject* sender) -> void;
};

class $modify(ModifyLevelInfoLayer, LevelInfoLayer) {
public:
    static auto onModify(auto& self) -> void;
    auto onLevelInfo(CCObject* sender) -> void;
};

class $modify(ModifyLevelSelectLayer, LevelSelectLayer) {
public:
    static auto onModify(auto& self) -> void;
    auto onInfo(CCObject* sender) -> void;
};