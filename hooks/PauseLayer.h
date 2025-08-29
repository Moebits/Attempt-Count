#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ModifyPauseLayer, PauseLayer) {
public:
	static auto onModify(auto& self) -> void;
	auto customSetup() -> void;
	auto buttonPress(CCObject* sender) -> void;
};