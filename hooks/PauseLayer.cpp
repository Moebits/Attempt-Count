#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "PauseMenuPopup.hpp"
#include "Functions.hpp"

using namespace geode::prelude;

class $modify(ModifyPauseLayer, PauseLayer) {
public:
	static auto onModify(auto& self) -> void {
		(void) self.setHookPriorityPost("PauseLayer::customSetup", Priority::Late);
		Functions::bindHookToSetting(self, "PauseLayer::customSetup", "pause-menu-enabled");
	}

	auto customSetup() -> void {
		PauseLayer::customSetup();
	
		// @geode-ignore(unknown-resource)
		auto* buttonSprite = CCSprite::create("logo.png"_spr);
	
		buttonSprite->setScale(0.5f);
		GEODE_MOBILE(buttonSprite->setScale(0.3f))
	
		auto* button = CCMenuItemSpriteExtra::create(
			buttonSprite, this, menu_selector(ModifyPauseLayer::buttonPress)
		);
	
		bool rightMenu = Mod::get()->getSettingValue<bool>("right-pause-menu");
		auto* menu = this->getChildByIDRecursive(rightMenu ? "right-button-menu" : "left-button-menu");
		
		if (menu) {
			menu->addChild(button);
			menu->updateLayout();
		}
	}

	auto buttonPress(CCObject* sender) -> void {
		PauseMenuPopup::create()->show();
	}
};