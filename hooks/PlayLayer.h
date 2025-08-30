#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ModifyPlayLayer, PlayLayer) {
public:
    struct Fields {
        double levelTime = 0;
    };

    auto init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) -> bool;
    auto postUpdate(float dt) -> void override;

    auto updateEndTime(float dt) -> void;

    auto levelComplete() -> void;
    auto fullReset() -> void;
    auto onQuit() -> void;
};