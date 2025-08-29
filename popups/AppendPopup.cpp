#include "AppendPopup.h"
#include "PopupFunctions.h"

auto AppendPopup::create(TextInput* targetInput, bool floatFilter) -> AppendPopup* {
    auto ret = new AppendPopup();
    if (ret->initAnchored(popupSize.width, popupSize.height, targetInput, floatFilter)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

auto AppendPopup::setup(TextInput* targetInput, bool floatFilter) -> bool {
    this->targetInput = targetInput;
    this->floatFilter = floatFilter;
    this->m_closeBtn->setVisible(false);
    this->m_closeBtn->setEnabled(false);

    auto titleRow = PopupFunctions::createTitleRow("Append Count");
    auto appendRow = PopupFunctions::createInputRow("Append:", this->appendInput, 100.f, "", floatFilter);
    auto buttonRow = PopupFunctions::createButtonRow(this, "Cancel", "Append", menu_selector(AppendPopup::cancel), menu_selector(AppendPopup::ok));

    auto col = CCNode::create();
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

auto AppendPopup::ok(CCObject* sender) -> void {
    auto appendValue = numFromString<double>(this->appendInput->getString());
    auto targetValue = numFromString<double>(this->targetInput->getString());

    if (appendValue && targetValue) {
        auto newValue = *targetValue + *appendValue;
        if (!this->floatFilter) newValue = static_cast<int>(newValue);
        this->targetInput->setString(fmt::format("{}", newValue));
    }

    this->onClose(nullptr);
}

auto AppendPopup::cancel(CCObject* sender) -> void {
    this->onClose(nullptr);
}