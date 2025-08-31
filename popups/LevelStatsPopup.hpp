#pragma once
#include <Geode/Geode.hpp>
#include "AppendPopup.hpp"
#include "PopupFunctions.hpp"

using namespace geode::prelude;

class LevelStatsPopup : public Popup<GJGameLevel*, FLAlertLayer*> {
public:
    GJGameLevel* level;
    FLAlertLayer* alert;
    TextInput* attemptsInput;
    TextInput* jumpsInput;

    static constexpr auto popupSize = CCSize{300.f, 250.f};

    static auto create(GJGameLevel* level, FLAlertLayer* alert) -> LevelStatsPopup* {
        auto* ret = new LevelStatsPopup();
        if (ret->initAnchored(popupSize.width, popupSize.height, level, alert)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

protected:
    auto setup(GJGameLevel* level, FLAlertLayer* alert) -> bool override {
        this->level = level;
        this->alert = alert;
        int attempts = level->m_attempts;
        int jumps = level->m_jumps;
    
        this->m_closeBtn->setVisible(false);
        this->m_closeBtn->setEnabled(false);
    
        auto* titleRow = PopupFunctions::createTitleRow("Edit Attempt Count");
        auto* attemptsRow = PopupFunctions::createInputRowPlus("Attempts:", this->attemptsInput, 100.f, fmt::format("{}", attempts), this, menu_selector(LevelStatsPopup::plusMenu));
        auto* jumpsRow = PopupFunctions::createInputRowPlus("Jumps:", this->jumpsInput, 100.f, fmt::format("{}", jumps), this, menu_selector(LevelStatsPopup::plusMenu));
        auto* buttonRow = PopupFunctions::createButtonRow(this,"Cancel", "Edit", menu_selector(LevelStatsPopup::cancel), menu_selector(LevelStatsPopup::ok));
        
        auto* col = CCNode::create();
        col->setLayout(AxisLayout::create(Axis::Column)
            ->setGap(25.f)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setAxisReverse(true));
    
        col->addChild(titleRow);
        col->addChild(attemptsRow);
        col->addChild(jumpsRow);
        col->addChild(buttonRow);
    
        col->setContentSize(popupSize);
        col->setAnchorPoint({0.5f, 0.5f});
        col->setPosition(popupSize / 2);
        col->updateLayout();
        this->m_mainLayer->addChild(col);
    
        return true;
    }

    auto ok(CCObject* sender) -> void {
        auto newAttempts = numFromString<int>(this->attemptsInput->getString());
        auto newJumps = numFromString<int>(this->jumpsInput->getString());
    
        if (newAttempts && newJumps) {
            int oldAttempts = this->level->m_attempts;
            int oldJumps = this->level->m_jumps;
            int attemptDiff = *newAttempts - oldAttempts;
            int jumpDiff = *newJumps - oldJumps;
    
            int newTotalAttempts = this->level->m_attempts + attemptDiff;
            this->level->m_attempts = newTotalAttempts;
    
            int newTotalJumps = this->level->m_jumps + jumpDiff;
            this->level->m_jumps = newTotalJumps;
    
            auto* textArea = static_cast<TextArea*>(this->alert->getChildByIDRecursive("content-text-area"));
            auto* lines = textArea->m_label->m_lines;
    
            for (size_t i = 0; i < lines->count(); i++) {
                auto* label = static_cast<CCLabelBMFont*>(lines->objectAtIndex(i));
                std::string str = label->getString();
    
                if (str.find("Total Attempts") != std::string::npos) {
                    label->setString(fmt::format("Total Attempts: {}", this->level->m_attempts).c_str());
                } else if (str.find("Total Jumps") != std::string::npos) {
                    label->setString(fmt::format("Total Jumps: {}", this->level->m_jumps).c_str());
                }
            }
        }
        this->onClose(nullptr);
    }

    auto cancel(CCObject* sender) -> void {
        this->onClose(nullptr);
    }

    auto plusMenu(CCObject* sender) -> void {
        auto* button = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto* params = static_cast<PlusButtonParameters*>(button->getUserObject());
        AppendPopup::create(params->input, params->timeFilter)->show();
    }
};