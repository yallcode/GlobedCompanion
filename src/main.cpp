#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

// Simple mic button that sits in the top HUD between the % label and pause button.
// Toggles Globed voice chat when tapped.

class MicHudButton : public CCNode {
public:
    bool m_active = false;
    CCSprite* m_icon = nullptr;

    static MicHudButton* create() {
        auto* node = new MicHudButton();
        if (node->init()) {
            node->autorelease();
            return node;
        }
        delete node;
        return nullptr;
    }

    bool init() {
        if (!CCNode::init()) return false;

        // Background circle so it matches GD's HUD style
        auto bg = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        if (bg) {
            bg->setScale(0.85f);
            bg->setColor({0, 0, 0});
            bg->setOpacity(120);
            this->addChild(bg, 0);
        }

        // Mic icon — we use a built-in GD frame as a placeholder.
        // Swap this out with a custom mic sprite later (resources/mic.png).
        m_icon = CCSprite::createWithSpriteFrameName(resources/mic.png);
        if (!m_icon) {
            // fallback: just a dot so the button is still tappable
            m_icon = CCSprite::create();
        }
        m_icon->setScale(0.6f);
        this->addChild(m_icon, 1);

        // Make it tappable
        auto* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        this->addChild(menu, 2);

        auto* btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"),
            this,
            menu_selector(MicHudButton::onTap)
        );
        // Invisible tap target over the icon
        btn->setOpacity(0);
        btn->setScale(1.2f);
        menu->addChild(btn);

        setMicVisual(false);
        return true;
    }

    void onTap(CCObject*) {
        m_active = !m_active;
        setMicVisual(m_active);

        // TODO: replace with Globed's actual API call once confirmed
        // Something like: GlobedAudioManager::get()->setMicEnabled(m_active);
        log::info("[GlobedCompanion] mic toggled -> {}", m_active);
    }

    void setMicVisual(bool on) {
        // Green = active, white = muted
        m_icon->setColor(on ? ccColor3B{80, 220, 80} : ccColor3B{255, 255, 255});
        m_icon->setOpacity(on ? 255 : 160);
    }
};


// We hook UILayer (the HUD layer) instead of PlayLayer so the button
// sits alongside the pause button and doesn't interfere with game logic.
class $modify(UILayer) {
    bool init(GJBaseGameLayer* bgl) {
        if (!UILayer::init(bgl)) return false;

        auto* winSize = &CCDirector::sharedDirector()->getWinSize();

        // Pause button is at top-right. Percentage is top-left-ish.
        // We place the mic button just to the left of the pause button.
        // Pause button is typically around x = winSize->width - 20, y = winSize->height - 13
        // So we go a bit further left, same height.

        auto* micNode = MicHudButton::create();
        if (!micNode) return true; // don't crash if it fails

        float x = winSize->width - 58.f; // left of pause button
        float y = winSize->height - 13.f; // same row as pause button

        micNode->setPosition({x, y});
        this->addChild(micNode, 10);

        return true;
    }
};
