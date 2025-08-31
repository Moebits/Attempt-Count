#include "PauseLayer.h"
#include "PauseMenuPopup.h"
#include "Functions.h"

auto ModifyPauseLayer::onModify(auto& self) -> void {
	(void) self.setHookPriorityPost("PauseLayer::customSetup", Priority::Late);
	Functions::bindHookToSetting(self, "PauseLayer::customSetup", "pause-menu-enabled");
}

auto ModifyPauseLayer::customSetup() -> void {
	PauseLayer::customSetup();

	// @geode-ignore(unknown-resource)
	auto* buttonSprite = CCSprite::create("logo.png"_spr);

	buttonSprite->setScale(0.5f);

	#ifdef GEODE_MOBILE
		buttonSprite->setScale(0.3f);
	#endif

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

auto ModifyPauseLayer::buttonPress(CCObject* sender) -> void {
	PauseMenuPopup::create()->show();
}