#include "PlayLayer.h"
#include "Functions.h"

auto ModifyPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) -> bool {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
    this->m_fields->levelTime = 0;
    return true;
}

auto ModifyPlayLayer::postUpdate(float dt) -> void {
    PlayLayer::postUpdate(dt);
    this->m_fields->levelTime += dt;
}

auto ModifyPlayLayer::updateEndTime(float dt) -> void {
    auto* endLayer = typeinfo_cast<EndLevelLayer*>(this->getChildByIDRecursive("EndLevelLayer"));
    if (endLayer) {
        auto* timeLabel = static_cast<CCLabelBMFont*>(endLayer->getChildByIDRecursive("time-label"));
        timeLabel->setString(fmt::format("Time: {}", Functions::formatTime(this->m_fields->levelTime)).c_str());
        this->unschedule(schedule_selector(ModifyPlayLayer::updateEndTime));
    }
}

auto ModifyPlayLayer::levelComplete() -> void {
    PlayLayer::levelComplete();
    this->schedule(schedule_selector(ModifyPlayLayer::updateEndTime));
}

auto ModifyPlayLayer::fullReset() -> void {
    PlayLayer::fullReset();
    this->m_fields->levelTime = 0;
}

auto ModifyPlayLayer::onQuit() -> void {
    PlayLayer::onQuit();
    this->m_fields->levelTime = 0;
}