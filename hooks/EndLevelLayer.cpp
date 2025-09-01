#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "PlayLayer.hpp"
#include "Functions.hpp"

using namespace geode::prelude;

class $modify(ModifyEndLevelLayer, EndLevelLayer) {
public:
    auto customSetup() -> void {
        EndLevelLayer::customSetup();
        auto* playLayer = static_cast<ModifyPlayLayer*>(PlayLayer::get());
        auto* timeLabel = static_cast<CCLabelBMFont*>(this->getChildByIDRecursive("time-label"));
        timeLabel->setString(fmt::format("Time: {}", Functions::formatTime(playLayer->m_fields->levelTime)).c_str());
    }
};