#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "LevelStatsPopup.hpp"
#include "Functions.hpp"

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
    auto addButton(FLAlertLayer* alert, GJGameLevel* level) -> void {
        if (!alert) return;
    
        auto* menu = alert->getChildByIDRecursive("main-menu");
    
        if (menu) {
            // @geode-ignore(unknown-resource)
            auto* buttonSprite = CCSprite::create("logo.png"_spr);
    
            buttonSprite->setScale(0.65f);
            GEODE_MOBILE(buttonSprite->setScale(0.3f));
    
            auto* button = CCMenuItemSpriteExtra::create(
                buttonSprite, alert, menu_selector(SharedStatsPanel::buttonPress)
            );
    
            button->setUserObject(new ButtonParameters(level, alert));
    
            if (!menu->getLayout()) {
                menu->setLayout(AxisLayout::create(Axis::Row)
                    ->setGap(10.0f)
                    ->setAxisAlignment(AxisAlignment::Center)
                    ->setCrossAxisAlignment(AxisAlignment::Center));
            }
    
            bool leftMenu = Mod::get()->getSettingValue<bool>("left-stats-menu");
    
            if (leftMenu) {
                auto* existingButton = menu->getChildByIDRecursive("button-1");
                menu->insertBefore(button, existingButton);
            } else {
                menu->addChild(button);
            }
    
            menu->updateLayout();
        }
    }

    auto buttonPress(CCObject* sender) -> void {
        auto* button = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto* params = static_cast<ButtonParameters*>(button->getUserObject());
        LevelStatsPopup::create(params->level, params->alert)->show();
    }
};

static auto statsPanel = SharedStatsPanel{};

class $modify(ModifyEditLevelLayer, EditLevelLayer) {
public:
    static auto onModify(auto& self) -> void {
        (void) self.setHookPriorityPost("EditLevelLayer::onLevelInfo", Priority::Normal);
        Functions::bindHookToSetting(self, "EditLevelLayer::onLevelInfo", "level-stats-enabled");
    }

    auto onLevelInfo(CCObject* sender) -> void {
        EditLevelLayer::onLevelInfo(sender);
    
        auto* scene = CCDirector::get()->getRunningScene();
        auto* alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));
    
        if (alert) {
            statsPanel.addButton(alert, this->m_level);
        }
    }
};

class $modify(ModifyLevelInfoLayer, LevelInfoLayer) {
public:
    static auto onModify(auto& self) -> void {
        (void) self.setHookPriorityPost("LevelInfoLayer::onLevelInfo", Priority::Normal);
        Functions::bindHookToSetting(self, "LevelInfoLayer::onLevelInfo", "level-stats-enabled");
    }

    auto onLevelInfo(CCObject* sender) -> void {
        LevelInfoLayer::onLevelInfo(sender);
    
        auto* scene = CCDirector::get()->getRunningScene();
        auto* alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));
    
        if (alert) {
            statsPanel.addButton(alert, this->m_level);
        }
    }
};

class $modify(ModifyLevelSelectLayer, LevelSelectLayer) {
public:
    static auto onModify(auto& self) -> void {
        (void) self.setHookPriorityPost("LevelSelectLayer::onInfo", Priority::Normal);
        Functions::bindHookToSetting(self, "LevelSelectLayer::onInfo", "level-stats-enabled");
    }

    auto onInfo(CCObject* sender) -> void {
        LevelSelectLayer::onInfo(sender);
        
        auto* scene = CCDirector::get()->getRunningScene();
        auto* alert = static_cast<FLAlertLayer*>(scene->getChildByIDRecursive("FLAlertLayer"));
    
        if (alert) {
            auto* title = static_cast<CCLabelBMFont*>(alert->getChildByIDRecursive("title"));
            if (static_cast<std::string>(title->getString()) != "Level Stats") return;
    
            int totalPages = this->m_scrollLayer->getTotalPages();
            int page = ((this->m_scrollLayer->m_page % totalPages) + totalPages) % totalPages;
            auto* level = GameLevelManager::sharedState()->getMainLevel(page + 1, true);
            statsPanel.addButton(alert, level);
        }
    }
};