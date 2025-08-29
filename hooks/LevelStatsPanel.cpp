#include "LevelStatsPanel.h"
#include "LevelStatsPopup.h"

auto SharedStatsPanel::addButton(FLAlertLayer* alert, GJGameLevel* level) -> void {
    if (!alert) return;

    auto menu = alert->getChildByIDRecursive("main-menu");

    if (menu) {
        // @geode-ignore(unknown-resource)
        auto buttonSprite = CCSprite::create("logo.png"_spr);
        buttonSprite->setScale(0.4f);

        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite, alert, menu_selector(SharedStatsPanel::buttonPress)
        );

        button->setUserObject(new ButtonParameters(level, alert));

        if (!menu->getLayout()) {
            menu->setLayout(AxisLayout::create(Axis::Row)
                ->setGap(10.0f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center));
        }

        menu->addChild(button);
        menu->updateLayout();
    }
}

auto SharedStatsPanel::buttonPress(CCObject* sender) -> void {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto params = static_cast<ButtonParameters*>(button->getUserObject());
    LevelStatsPopup::create(params->level, params->alert)->show();
}

auto ModifyEditLevelLayer::onLevelInfo(CCObject* sender) -> void {
    EditLevelLayer::onLevelInfo(sender);

    bool levelStatsEnabled = Mod::get()->getSettingValue<bool>("level-stats-enabled");
	if (!levelStatsEnabled) return;

    auto scene = CCDirector::get()->getRunningScene();
    auto alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));

    if (alert) {
        statsPanel.addButton(alert, this->m_level);
    }
}

auto ModifyLevelInfoLayer::onLevelInfo(CCObject* sender) -> void {
    LevelInfoLayer::onLevelInfo(sender);

    bool levelStatsEnabled = Mod::get()->getSettingValue<bool>("level-stats-enabled");
	if (!levelStatsEnabled) return;

    auto scene = CCDirector::get()->getRunningScene();
    auto alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));

    if (alert) {
        statsPanel.addButton(alert, this->m_level);
    }
}

auto ModifyLevelSelectLayer::onInfo(CCObject* sender) -> void {
    LevelSelectLayer::onInfo(sender);
    
    bool levelStatsEnabled = Mod::get()->getSettingValue<bool>("level-stats-enabled");
	if (!levelStatsEnabled) return;

    auto scene = CCDirector::get()->getRunningScene();
    auto alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));

    if (alert) {
        auto title = static_cast<CCLabelBMFont*>(alert->getChildByIDRecursive("title"));
        if (static_cast<gd::string>(title->getString()) != "Level Stats") return;

        int totalPages = this->m_scrollLayer->getTotalPages();
        int page = ((this->m_scrollLayer->m_page % totalPages) + totalPages) % totalPages;
        auto level = GameLevelManager::sharedState()->getMainLevel(page + 1, true);
        statsPanel.addButton(alert, level);
    }
}