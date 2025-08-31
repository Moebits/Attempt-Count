#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct PlusButtonParameters : public CCObject {
    TextInput* input;
    bool timeFilter;

    PlusButtonParameters(TextInput* input, bool timeFilter) 
        : input(input), timeFilter(timeFilter) {
        this->autorelease();
    }
};

class PopupFunctions {
public:
    static auto createTitleRow(const std::string& text, float scale = 0.8f, const char* font = "bigFont.fnt") -> CCNode* {
        auto* row = CCNode::create();
        row->setLayout(AxisLayout::create(Axis::Row)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center));
    
        auto* label = CCLabelBMFont::create(text.c_str(), font);
        label->setScale(scale);
        label->setAnchorPoint({0.5f, 0.5f});
        row->addChild(label);
    
        row->setContentSize(label->getScaledContentSize());
        row->updateLayout();
    
        return row;
    }

    static auto createInputRow(const char* text, TextInput*& input, float width, const std::string& value, 
        bool timeFilter = false) -> CCNode* {
        auto* row = CCNode::create();
        row->setLayout(AxisLayout::create(Axis::Row)
            ->setGap(10.f)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center));
    
        auto* label = CCLabelBMFont::create(text, "goldFont.fnt");
        label->setScale(0.8f);
        label->setAnchorPoint({0.f, 0.5f});
        row->addChild(label);
    
        input = TextInput::create(width, "0");
        input->setFilter(timeFilter ? ".:-0123456789" : "-0123456789");
        input->setString(value);
        input->setAnchorPoint({0.f, 0.5f});
        row->addChild(input);
    
        row->setContentSize({label->getScaledContentSize().width + 10 + width,
        std::max(label->getScaledContentSize().height, input->getScaledContentSize().height)});
        row->updateLayout();
    
        return row;
    }

    static auto createInputRowPlus(const char* text, TextInput*& input, float width, const std::string& value, 
        CCObject* target, SEL_MenuHandler plusFunc, bool timeFilter = false) -> CCNode* {
        auto* row = CCNode::create();
        row->setLayout(AxisLayout::create(Axis::Row)
            ->setGap(10.f)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center));
    
        auto* label = CCLabelBMFont::create(text, "goldFont.fnt");
        label->setScale(0.8f);
        label->setAnchorPoint({0.f, 0.5f});
        row->addChild(label);
    
        input = TextInput::create(width, "0");
        input->setFilter(timeFilter ? ".:-0123456789" : "-0123456789");
        input->setString(value);
        input->setAnchorPoint({0.f, 0.5f});
        row->addChild(input);
    
        auto* plusButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("+"), target, plusFunc);
        plusButton->setUserObject(new PlusButtonParameters(input, timeFilter));
    
        auto* menu = CCMenu::create();
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

    static auto createButtonRow(CCObject* target, const char* cancelLabel, const char* okLabel, 
        SEL_MenuHandler cancelFunc, SEL_MenuHandler okFunc, float padding = 20.0f) -> CCNode* {
        auto* row = CCMenu::create();
        row->setLayout(AxisLayout::create(Axis::Row)
            ->setGap(padding)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center));
    
        auto* leftButton  = CCMenuItemSpriteExtra::create(ButtonSprite::create(cancelLabel), target, cancelFunc);
        auto* rightButton = CCMenuItemSpriteExtra::create(ButtonSprite::create(okLabel), target, okFunc);
    
        row->addChild(leftButton);
        row->addChild(rightButton);
        row->updateLayout();
    
        return row;
    }
};