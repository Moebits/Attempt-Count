#include "PauseLayer.h"
#include "PauseMenuPopup.h"

auto ModifyPauseLayer::onModify(auto& self) -> void {
	[[maybe_unused]] auto hook = self.setHookPriorityPost("PauseLayer::customSetup", Priority::Late);
}

auto ModifyPauseLayer::customSetup() -> void {
	PauseLayer::customSetup();

	bool pauseEnabled = Mod::get()->getSettingValue<bool>("pause-menu-enabled");
	if (!pauseEnabled) return;

	// @geode-ignore(unknown-resource)
	auto buttonSprite = CCSprite::create("logo.png"_spr);
	buttonSprite->setScale(0.3f);

	auto button = CCMenuItemSpriteExtra::create(
		buttonSprite, this, menu_selector(ModifyPauseLayer::buttonPress)
	);

	bool rightMenu = Mod::get()->getSettingValue<bool>("right-pause-menu");

	if (auto menu = this->getChildByIDRecursive(rightMenu ? "right-button-menu" : "left-button-menu")) {
		menu->addChild(button);
		menu->updateLayout();
	}
}

auto ModifyPauseLayer::buttonPress(CCObject* sender) -> void {
	PauseMenuPopup::create()->show();
}