#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ModifyPlayLayer, PlayLayer) {
public:
    struct Fields {
        double levelTime = 0;
    };

    auto init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) -> bool {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        this->m_fields->levelTime = 0;
        return true;
    }

    auto postUpdate(float dt) -> void override {
        PlayLayer::postUpdate(dt);
        this->m_fields->levelTime += dt;
    }

    auto fullReset() -> void {
        PlayLayer::fullReset();
        this->m_fields->levelTime = 0;
    }

    auto onQuit() -> void {
        PlayLayer::onQuit();
        this->m_fields->levelTime = 0;
    }

    auto setupHasCompleted() -> void {
        PlayLayer::setupHasCompleted();

		bool hideStartposLabel = Mod::get()->getSettingValue<bool>("hide-startpos-label");
        if (hideStartposLabel) {
            if (this->m_startPosObject) this->m_attemptLabel->setScale(0.f);
        }
    }
};