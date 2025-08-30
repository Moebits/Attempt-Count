#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct PlusButtonParameters : public CCObject {
    TextInput* input;
    bool floatFilter;

    PlusButtonParameters(TextInput* input, bool floatFilter) 
        : input(input), floatFilter(floatFilter) {
        this->autorelease();
    }
};

class PopupFunctions {
public:
    static auto createTitleRow(const gd::string& text, float scale = 0.8f, const char* font = "bigFont.fnt") -> CCNode*;

    static auto createInputRow(const char* text, TextInput*& input, float width, const gd::string& value, 
        bool floatFilter = false) -> CCNode*;

    static auto createInputRowPlus(const char* text, TextInput*& input, float width, const gd::string& value, 
        CCObject* target, SEL_MenuHandler plusFunc, bool floatFilter = false) -> CCNode*;

    static auto createButtonRow(CCObject* target, const char* cancelLabel, const char* okLabel, 
        SEL_MenuHandler cancelFunc, SEL_MenuHandler okFunc, float padding = 20.0f) -> CCNode*;  
};