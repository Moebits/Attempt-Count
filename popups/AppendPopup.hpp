#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "PopupFunctions.hpp"
#include "Functions.hpp"

using namespace geode::prelude;

class AppendPopup : public Popup<TextInput*, bool> {
public:
    TextInput* targetInput;
    bool timeFilter;
    TextInput* appendInput;

    static constexpr auto popupSize = CCSize{320.f, 200.f};

    static auto create(TextInput* targetInput, bool timeFilter) -> AppendPopup* {
        auto* ret = new AppendPopup();
        if (ret->initAnchored(popupSize.width, popupSize.height, targetInput, timeFilter)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

protected:
    auto setup(TextInput* targetInput, bool timeFilter) -> bool override {
        this->targetInput = targetInput;
        this->timeFilter = timeFilter;
        this->m_closeBtn->setVisible(false);
        this->m_closeBtn->setEnabled(false);
    
        auto* titleRow = PopupFunctions::createTitleRow("Append Count");
        auto* appendRow = PopupFunctions::createInputRow("Append:", this->appendInput, 100.f, "", timeFilter);
        auto* buttonRow = PopupFunctions::createButtonRow(this, "Cancel", "Append", menu_selector(AppendPopup::cancel), menu_selector(AppendPopup::ok));
    
        auto* col = CCNode::create();
        col->setLayout(AxisLayout::create(Axis::Column)
            ->setGap(25.f)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setAxisReverse(true));
    
        col->addChild(titleRow);
        col->addChild(appendRow);
        col->addChild(buttonRow);
    
        col->setContentSize(popupSize);
        col->setAnchorPoint({0.5f, 0.5f});
        col->setPosition(popupSize / 2);
        col->updateLayout();
        this->m_mainLayer->addChild(col);
    
        return true;
    }

    auto ok(CCObject* sender) -> void {
        auto appendStr = this->appendInput->getString();
        auto targetStr = this->targetInput->getString();

        auto appendTime = Functions::parseTime(appendStr);
        auto targetTime = Functions::parseTime(targetStr);

        if (targetTime) {
            auto [targetSeconds, targetInfo] = *targetTime;

            double newTotal = targetSeconds;
            auto info = targetInfo;

            if (appendTime) {
                auto [appendSeconds, appendInfo] = *appendTime;
                newTotal += appendSeconds;
                info |= appendInfo;
            } else {
                auto appendValue = numFromString<double>(appendStr);
                if (appendValue) newTotal += *appendValue;
            }

            this->targetInput->setString(Functions::formatTimeConditional(newTotal, info));
        } else {
            auto appendValue = numFromString<double>(appendStr);
            auto targetValue = numFromString<double>(targetStr);
        
            if (targetValue) {
                double newValue = *targetValue;
                if (appendTime) {
                    auto [appendSeconds, appendInfo] = *appendTime;
                    newValue += appendSeconds;
                } else {
                    if (appendValue) newValue += *appendValue;
                }
                if (!this->timeFilter) newValue = static_cast<int>(newValue);
                this->targetInput->setString(fmt::format("{}", newValue));
            }
        }

        this->onClose(nullptr);
    }

    auto cancel(CCObject* sender) -> void {
        this->onClose(nullptr);
    }
};