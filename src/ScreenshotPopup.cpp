#include "ScreenshotPopup.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
    CCMenu* thing = Build<CCMenu>(CCMenu::create())
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(5.f))
        .width(235.f)
        .collect();

    CCMenuItemToggler* toggler = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), 
    [key, mod = Mod::get()](CCMenuItemToggler* toggler) { 
        toggler->toggle(mod->getSavedValue<bool>(key));
        mod->setSavedValue<bool>(key, !mod->getSavedValue<bool>(key));
    })
        .scale(0.75f)
        .parent(thing)
        .collect();

    toggler->toggle(Mod::get()->getSavedValue<bool>(key));

    Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
        .scale(0.4f)
        .parent(thing);
    
    thing->updateLayout();
    return thing;
}

bool ScreenshotPopup::setup() {
    this->setTitle("Screenshot");
    this->setID("ScreenshotPopup");

    float centerX = CCDirector::get()->getWinSize().width / 2;
    float centerY = CCDirector::get()->getWinSize().height / 2;

    CCMenu* resolutionMenu = CCMenu::create();
    resolutionMenu->setPosition(ccp(75.f, 185.f));
    resolutionMenu->setContentWidth(240.f);
    resolutionMenu->setLayout(RowLayout::create()->setAutoScale(false));

    resolutionWidthInput = Build<InputNode>::create(55.f, "Width", "bigFont.fnt", "1234567890", 4)
        .scale(0.75f)
        .parent(resolutionMenu)
        .collect();

    resolutionWidthInput->setString(std::to_string(Mod::get()->getSavedValue<int64_t>("resolution-width")));
    resolutionWidthInput->setID("resolution-width-input");
    resolutionWidthInput->getInput()->setDelegate(this);
    
    CCLabelBMFont* xLabel = CCLabelBMFont::create("x", "bigFont.fnt");
    xLabel->setScale(0.7f);
    resolutionMenu->addChild(xLabel);

    resolutionHeightInput = Build<InputNode>::create(55.f, "Height", "bigFont.fnt", "1234567890", 4)
        .scale(0.75f)
        .parent(resolutionMenu)
        .collect();

    resolutionHeightInput->setString(std::to_string(Mod::get()->getSavedValue<int64_t>("resolution-height")));
    resolutionHeightInput->setID("resolution-height-input");
    resolutionHeightInput->getInput()->setDelegate(this);

    CCMenu* settingsMenu = CCMenu::create();
    settingsMenu->setPosition(ccp(275.f, 115.f));
    settingsMenu->setContentWidth(240.f);
    settingsMenu->setLayout(ColumnLayout::create()->setAutoScale(false)->setAxisReverse(true));

    settingsMenu->addChild(createSetting("Copy To Clipboard", "copy-clipboard"));
    settingsMenu->addChild(createSetting("Hide Player Icon", "hide-player"));
    settingsMenu->addChild(createSetting("Hide UI Layer", "hide-ui"));
    settingsMenu->addChild(createSetting("JPEG", "jpeg-mafia"));
    settingsMenu->addChild(createSetting("Auto Screenshot", "auto-screenshot"));

    CCMenu* autoPercent = Build<CCMenu>::create()
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(4.f))
        .width(230.f)
        .parent(settingsMenu)
        .collect();

    autoPercentInput = Build<InputNode>::create(35.f, "%", "bigFont.fnt", "1234567890", 2)
        .scale(0.75f)
        .parent(autoPercent)
        .collect();

    autoPercentInput->setString(std::to_string(Mod::get()->getSavedValue<int64_t>("auto-percent")));
    autoPercentInput->setID("auto-percent-input");
    autoPercentInput->getInput()->setDelegate(this);

    Build<CCLabelBMFont>::create("Auto Percent", "bigFont.fnt")
        .scale(0.4f)
        .parent(autoPercent);

    autoPercent->updateLayout();

    m_mainLayer->addChild(settingsMenu);
    m_mainLayer->addChild(resolutionMenu);
    settingsMenu->updateLayout();
    resolutionMenu->updateLayout();

    return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
    //Mod::get()->setSavedValue<int64_t>("auto-percent", std::stoi(input->getString()));
}

void ScreenshotPopup::textChanged(CCTextInputNode* p0) {
    std::string inputID = p0->getID();
    if (inputID == "auto-percent-input") Mod::get()->setSavedValue<int64_t>("auto-percent", std::stoi(p0->getString()));
    if (inputID == "resolution-width-input") Mod::get()->setSavedValue<int64_t>("resolution-width", std::stoi(p0->getString()));
    if (inputID == "resolution-height-input") Mod::get()->setSavedValue<int64_t>("resolution-height", std::stoi(p0->getString()));
}

ScreenshotPopup* ScreenshotPopup::create() {
    auto ret = new ScreenshotPopup;
    if (ret->initAnchored(POPUP_WIDTH, POPUP_HEIGHT, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}