#include "PopupFunctions.h"

auto PopupFunctions::createTitleRow(const gd::string& text, float scale, const char* font) -> CCNode* {
    auto row = CCNode::create();
    row->setLayout(AxisLayout::create(Axis::Row)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center));

    auto label = CCLabelBMFont::create(text.c_str(), font);
    label->setScale(scale);
    label->setAnchorPoint({0.5f, 0.5f});
    row->addChild(label);

    row->setContentSize(label->getScaledContentSize());
    row->updateLayout();

    return row;
}

auto PopupFunctions::createInputRow(const char* text, TextInput*& input, float width, const std::string& value, 
    bool floatFilter) -> CCNode* {
    auto row = CCNode::create();
    row->setLayout(AxisLayout::create(Axis::Row)
        ->setGap(10.f)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center));

    auto label = CCLabelBMFont::create(text, "goldFont.fnt");
    label->setScale(0.8f);
    label->setAnchorPoint({0.f, 0.5f});
    row->addChild(label);

    input = TextInput::create(width, "0");
    input->setCommonFilter(floatFilter ? CommonFilter::Float : CommonFilter::Int);
    input->setString(value);
    input->setAnchorPoint({0.f, 0.5f});
    row->addChild(input);

    row->setContentSize({label->getScaledContentSize().width + 10 + width,
    std::max(label->getScaledContentSize().height, input->getScaledContentSize().height)});
    row->updateLayout();

    return row;
}

auto PopupFunctions::createInputRowPlus(const char* text, TextInput*& input, float width, const gd::string& value, 
    CCObject* target, SEL_MenuHandler plusFunc, bool floatFilter) -> CCNode* {
    auto row = CCNode::create();
    row->setLayout(AxisLayout::create(Axis::Row)
        ->setGap(10.f)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center));

    auto label = CCLabelBMFont::create(text, "goldFont.fnt");
    label->setScale(0.8f);
    label->setAnchorPoint({0.f, 0.5f});
    row->addChild(label);

    input = TextInput::create(width, "0");
    input->setCommonFilter(floatFilter ? CommonFilter::Float : CommonFilter::Int);
    input->setString(value);
    input->setAnchorPoint({0.f, 0.5f});
    row->addChild(input);

    auto plusButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("+"), target, plusFunc);
    plusButton->setUserObject(new PlusButtonParameters(input, floatFilter));

    auto menu = CCMenu::create();
    menu->setLayout(AxisLayout::create(Axis::Row)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center));
    menu->addChild(plusButton);
    menu->setContentSize(plusButton->getScaledContentSize());
    menu->updateLayout();
    row->addChild(menu);

    row->setContentSize({label->getScaledContentSize().width + 10 + width,
    std::max(label->getScaledContentSize().height, input->getScaledContentSize().height)});
    row->updateLayout();

    return row;
}

auto PopupFunctions::createButtonRow(CCObject* target, const char* cancelLabel, const char* okLabel, 
    SEL_MenuHandler cancelFunc, SEL_MenuHandler okFunc, float padding) -> CCNode* {
    auto row = CCMenu::create();
    row->setLayout(AxisLayout::create(Axis::Row)
        ->setGap(padding)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center));

    auto leftButton  = CCMenuItemSpriteExtra::create(ButtonSprite::create(cancelLabel), target, cancelFunc);
    auto rightButton = CCMenuItemSpriteExtra::create(ButtonSprite::create(okLabel), target, okFunc);

    row->addChild(leftButton);
    row->addChild(rightButton);
    row->updateLayout();

    return row;
}