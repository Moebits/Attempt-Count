#include "PauseMenuPopup.h"
#include "PopupFunctions.h"
#include "AppendPopup.h"
#include "PlayLayer.h"
#include "Functions.h"

auto PauseMenuPopup::create() -> PauseMenuPopup* {
    auto* ret = new PauseMenuPopup();
    if (ret->initAnchored(popupSize.width, popupSize.height)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

auto PauseMenuPopup::setup() -> bool {
    auto* playLayer = static_cast<ModifyPlayLayer*>(PlayLayer::get());
    if (!playLayer) return false;
    int attempts = playLayer->m_attempts;
    int jumps = playLayer->m_jumps;
    double time = playLayer->m_isPlatformer ? playLayer->m_timePlayed : playLayer->m_fields->levelTime;

    this->m_closeBtn->setVisible(false);
    this->m_closeBtn->setEnabled(false);

    auto* titleRow = PopupFunctions::createTitleRow("Edit Attempt Count");
    auto* attemptsRow = PopupFunctions::createInputRowPlus("Attempt:", this->attemptInput, 100.f, fmt::format("{}", attempts), this, menu_selector(PauseMenuPopup::plusMenu));
    auto* jumpsRow = PopupFunctions::createInputRowPlus("Jumps:", this->jumpInput, 100.f, fmt::format("{}", jumps), this, menu_selector(PauseMenuPopup::plusMenu));
    auto* timeRow = PopupFunctions::createInputRowPlus("Time:", this->timeInput, 100.f, fmt::format("{}", static_cast<int>(time)), this, menu_selector(PauseMenuPopup::plusMenu), true);
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

auto PauseMenuPopup::ok(CCObject* sender) -> void {
    auto newAttempts = numFromString<int>(this->attemptInput->getString());
    auto newJumps = numFromString<int>(this->jumpInput->getString());
    auto newTime  = numFromString<double>(this->timeInput->getString());

    if (newAttempts && newJumps && newTime) {
        auto* playLayer = static_cast<ModifyPlayLayer*>(PlayLayer::get());
        if (playLayer) {
            int oldAttempts = playLayer->m_attempts;
            int oldJumps = playLayer->m_jumps;
            int attemptDiff = *newAttempts - oldAttempts;
            int jumpDiff = *newJumps - oldJumps;

            playLayer->m_attempts = *newAttempts;
            playLayer->m_jumps = *newJumps;
            if (playLayer->m_isPlatformer) {
                playLayer->m_timePlayed = *newTime;
            } else {
                playLayer->m_fields->levelTime = *newTime;
            }

            if (playLayer->m_level) {
                int newTotalAttempts = playLayer->m_level->m_attempts + attemptDiff;
                playLayer->m_level->m_attempts = newTotalAttempts;

                int newTotalJumps = playLayer->m_level->m_jumps + jumpDiff;
                playLayer->m_level->m_jumps = newTotalJumps;
            }

            if (playLayer->m_attemptLabel) {
                playLayer->m_attemptLabel->setString(fmt::format("Attempts: {}", *newAttempts).c_str());
            }

            if (playLayer->m_isPlatformer) {
                auto* pauseLayer = typeinfo_cast<PauseLayer*>(CCDirector::get()->getRunningScene()->getChildByID("PauseLayer"));
                if (pauseLayer) {
                    auto* playTimeLabel = static_cast<CCLabelBMFont*>(pauseLayer->getChildByIDRecursive("play-time"));
                    playTimeLabel->setString(Functions::formatSeconds(playLayer->m_timePlayed).c_str());
                }
            }
        }
    }
    this->onClose(nullptr);
}

auto PauseMenuPopup::cancel(CCObject* sender) -> void {
    this->onClose(nullptr);
}

auto PauseMenuPopup::plusMenu(CCObject* sender) -> void {
    auto* button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto* params = static_cast<PlusButtonParameters*>(button->getUserObject());
    AppendPopup::create(params->input, params->floatFilter)->show();
}