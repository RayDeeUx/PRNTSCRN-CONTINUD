#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Popup.hpp>

class ScreenshotPopup : public geode::Popup<>, public TextInputDelegate {
public:
    constexpr static float POPUP_WIDTH = 340.f;
    constexpr static float POPUP_HEIGHT = 240.f;

    static ScreenshotPopup* create();

protected:
    bool copyToClipboard = false;

    CCMenuItemToggler* copyClipboardToggle;
    CCMenuItemToggler* hidePlayerToggle;
    CCMenuItemToggler* hideUIToggle;
    geode::TextInput* autoPercentInput;
    geode::TextInput* resolutionWidthInput;
    geode::TextInput* resolutionHeightInput;

    bool setup() override;

    cocos2d::CCMenu* createSetting(const std::string& title, const std::string& key);

    void onScreenshot(cocos2d::CCObject*);
    void onCopyClipboard(cocos2d::CCObject*);

    void textChanged(CCTextInputNode* p0) override;
};