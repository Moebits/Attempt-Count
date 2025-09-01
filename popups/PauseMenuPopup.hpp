#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "PlayLayer.hpp"
#include "AppendPopup.hpp"
#include "Functions.hpp"

using namespace geode::prelude;

class PauseMenuPopup : public Popup<> {
public:
    TextInput* attemptInput;
    TextInput* jumpInput;
    TextInput* timeInput;

    static constexpr auto popupSize = CCSize{300.f, 270.f};

    static auto create() -> PauseMenuPopup* {
        auto* ret = new PauseMenuPopup();
        if (ret->initAnchored(popupSize.width, popupSize.height)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

protected:
    auto setup() -> bool override {
        auto* playLayer = static_cast<ModifyPlayLayer*>(PlayLayer::get());
        if (!playLayer) return false;
        int attempts = playLayer->m_attempts;
        int jumps = playLayer->m_jumps;
        double time = playLayer->m_isPlatformer ? playLayer->m_timePlayed : playLayer->m_fields->levelTime;
        auto timeInfo = Functions::parseDoubleTime(time);
        auto timeStr = Functions::formatTimeConditional(time, timeInfo);
    
        this->m_closeBtn->setVisible(false);
        this->m_closeBtn->setEnabled(false);
    
        auto* titleRow = PopupFunctions::createTitleRow("Edit Attempt Count");
        auto* attemptsRow = PopupFunctions::createInputRowPlus("Attempt:", this->attemptInput, 100.f, fmt::format("{}", attempts), this, menu_selector(PauseMenuPopup::plusMenu));
        auto* jumpsRow = PopupFunctions::createInputRowPlus("Jumps:", this->jumpInput, 100.f, fmt::format("{}", jumps), this, menu_selector(PauseMenuPopup::plusMenu));
        auto* timeRow = PopupFunctions::createInputRowPlus("Time:", this->timeInput, 100.f, timeStr, this, menu_selector(PauseMenuPopup::plusMenu), true);
        auto* buttonRow = PopupFunctions::createButtonRow(this, "Cancel", "Edit", menu_selector(PauseMenuPopup::cancel), menu_selector(PauseMenuPopup::ok));
        
        auto* col = CCNode::create();
        col->setLayout(AxisLayout::create(Axis::Column)
            ->setGap(25.f)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setAxisReverse(true));
    
        col->addChild(titleRow);
        col->addChild(attemptsRow);
        col->addChild(jumpsRow);
        col->addChild(timeRow);
        col->addChild(buttonRow);
    
        col->setContentSize(popupSize);
        col->setAnchorPoint({0.5f, 0.5f});
        col->setPosition(popupSize / 2);
        col->updateLayout();
        this->m_mainLayer->addChild(col);
    
        return true;
    }

    auto ok(CCObject* sender) -> void {
        auto newAttempts = numFromString<int>(this->attemptInput->getString());
        auto newJumps = numFromString<int>(this->jumpInput->getString());
        
        double newTime = 0;
        auto timeStr = this->timeInput->getString();

        auto parsedTime = Functions::parseTime(timeStr);
        if (parsedTime) {
            auto [seconds, info] = *parsedTime;
            newTime = seconds;
        } else {
            auto newTimeRes = numFromString<double>(timeStr);
            if (newTimeRes) newTime = *newTimeRes;
        }
    
        if (newAttempts && newJumps) {
            auto* playLayer = static_cast<ModifyPlayLayer*>(PlayLayer::get());
            if (playLayer) {
                int oldAttempts = playLayer->m_attempts;
                int oldJumps = playLayer->m_jumps;
                int attemptDiff = *newAttempts - oldAttempts;
                int jumpDiff = *newJumps - oldJumps;
    
                playLayer->m_attempts = *newAttempts;
                playLayer->m_jumps = *newJumps;
                if (playLayer->m_isPlatformer) {
                    playLayer->m_timePlayed = newTime;
                } else {
                    playLayer->m_fields->levelTime = newTime;
                }
    
                if (playLayer->m_level) {
                    int newTotalAttempts = playLayer->m_level->m_attempts + attemptDiff;
                    playLayer->m_level->m_attempts = newTotalAttempts;
    
                    int newTotalJumps = playLayer->m_level->m_jumps + jumpDiff;
                    playLayer->m_level->m_jumps = newTotalJumps;
                }
    
                if (playLayer->m_attemptLabel) {
                    playLayer->m_attemptLabel->setString(fmt::format("Attempt {}", *newAttempts).c_str());
                }
    
                if (playLayer->m_isPlatformer) {
                    auto* pauseLayer = static_cast<PauseLayer*>(CCDirector::get()->getRunningScene()->getChildByID("PauseLayer"));
                    if (pauseLayer) {
                        auto* playTimeLabel = static_cast<CCLabelBMFont*>(pauseLayer->getChildByIDRecursive("play-time"));
                        playTimeLabel->setString(Functions::formatPlatformerTime(playLayer->m_timePlayed).c_str());
                    }
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