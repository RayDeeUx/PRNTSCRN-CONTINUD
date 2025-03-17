#include "ScreenshotPopup.hpp"
#include "Screenshot.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

#define ADD_NODE(val) uiNodes[#val] = pl->getChildByID(#val)->isVisible(); \
pl->getChildByID(#val)->setVisible(false);

#define ADD_MEM(val) uiNodes[#val] = pl->val->isVisible(); \
pl->val->setVisible(false);

#define RES_NODE(val) pl->getChildByID(#val)->setVisible(uiNodes[#val]);

#define RES_MEM(val) pl->val->setVisible(uiNodes[#val]);

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
    CCMenu* thing = Build<CCMenu>(CCMenu::create())
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(5.f))
        .width(235.f)
        .collect();

    CCMenuItemToggler* toggler = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), 
    [key, mod = Mod::get()](CCMenuItemToggler* toggler) { 
        toggler->toggle(mod->getSettingValue<bool>(key));
        mod->setSettingValue<bool>(key, !mod->getSettingValue<bool>(key));
    })
        .scale(0.75f)
        .parent(thing)
        .collect();

    toggler->toggle(Mod::get()->getSettingValue<bool>(key));

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

    resolutionWidthInput = Build<TextInput>::create(55.f, "Width", "bigFont.fnt")
        .scale(0.75f)
        .parent(resolutionMenu)
        .collect();

    resolutionWidthInput->setString(std::to_string(Mod::get()->getSettingValue<int64_t>("resolution-width")));
    resolutionWidthInput->getInputNode()->setID("resolution-width-input");
    resolutionWidthInput->getInputNode()->setDelegate(this);
    
    CCLabelBMFont* xLabel = CCLabelBMFont::create("x", "bigFont.fnt");
    xLabel->setScale(0.7f);
    resolutionMenu->addChild(xLabel);

    resolutionHeightInput = Build<TextInput>::create(55.f, "Height", "bigFont.fnt")
        .scale(0.75f)
        .parent(resolutionMenu)
        .collect();

    resolutionHeightInput->setString(std::to_string(Mod::get()->getSettingValue<int64_t>("resolution-height")));
    resolutionHeightInput->getInputNode()->setID("resolution-height-input");
    resolutionHeightInput->getInputNode()->setDelegate(this);

    CCMenu* settingsMenu = CCMenu::create();
    settingsMenu->setPosition(ccp(265.f, 110.f));
    settingsMenu->setContentSize({240.f, 175.f});
    settingsMenu->setLayout(ColumnLayout::create()->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End));

    settingsMenu->addChild(createSetting("Copy To Clipboard", "copy-clipboard"));
    settingsMenu->addChild(createSetting("Hide UI Layer", "hide-ui"));
    settingsMenu->addChild(createSetting("Hide Player", "hide-player"));
    settingsMenu->addChild(createSetting("JPEG", "jpeg-mafia"));
    settingsMenu->addChild(createSetting("Auto Screenshot", "auto-screenshot"));

    CCMenu* autoPercent = Build<CCMenu>::create()
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(4.f)->setAxisReverse(true))
        .pos(130, 150)
        .width(230.f)
        .collect();

    autoPercentInput = Build<TextInput>::create(35.f, "%", "bigFont.fnt")
        .scale(0.75f)
        .parent(autoPercent)
        .collect();

    autoPercentInput->setString(std::to_string(Mod::get()->getSettingValue<int64_t>("auto-percent")));
    autoPercentInput->getInputNode()->setID("auto-percent-input");
    autoPercentInput->getInputNode()->setDelegate(this);

    Build<CCLabelBMFont>::create("Auto Percent", "bigFont.fnt")
        .scale(0.4f)
        .parent(autoPercent);

    m_mainLayer->addChild(settingsMenu);
    m_mainLayer->addChild(resolutionMenu);
    m_mainLayer->addChild(autoPercent);
    settingsMenu->updateLayout();
    resolutionMenu->updateLayout();
    autoPercent->updateLayout();

    CCMenuItemSpriteExtra* scrnshotBTN = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Screenshot!"),
        this,
        menu_selector(ScreenshotPopup::onScreenshot)
    );
    scrnshotBTN->setPosition(ccp(170, 30));
    m_buttonMenu->addChild(scrnshotBTN);

    return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
    PlayLayer* pl = PlayLayer::get();

    if (!pl) return;
    std::unordered_map<const char*, bool> uiNodes = {};
			
    bool hideUI = Mod::get()->getSettingValue<bool>("hide-ui");
    bool hidePL = Mod::get()->getSettingValue<bool>("hide-player");

    if (hideUI) {
        ADD_NODE(UILayer);
        ADD_NODE(percentage-label);
        ADD_NODE(progress-bar);
    }
    if (hidePL) {
        ADD_MEM(m_player1);
        ADD_MEM(m_player2);
    }
    Screenshot ss = Screenshot(Mod::get()->getSettingValue<int64_t>("resolution-width"), Mod::get()->getSettingValue<int64_t>("resolution-height"), pl);
    if (hideUI) {
        RES_NODE(UILayer);
        RES_NODE(percentage-label);
        RES_NODE(progress-bar);
    }
    if (hidePL) {
        RES_MEM(m_player1);
        RES_MEM(m_player2);
    }

    bool jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
    std::string extension = jpeg ? ".jpg" : ".png";

    std::filesystem::path folder = Mod::get()->getConfigDir() / (std::to_string(PlayLayer::get()->m_level->m_levelID));
    if (!std::filesystem::exists(folder)) std::filesystem::create_directory(folder);

    int index = 1;
    while (std::filesystem::exists(folder / (std::to_string(index) + extension))) {
        index++;
    }

    std::string filename = fmt::format("{}/{}{}", folder.string(), index, extension);

    Mod::get()->getSettingValue<bool>("copy-clipboard") ? ss.intoClipboard() : ss.intoFile(filename, jpeg);
}

void ScreenshotPopup::textChanged(CCTextInputNode* p0) {
    if (p0->getString().length() < 1) return;
    std::string inputID = p0->getID();
    if (inputID == "auto-percent-input") Mod::get()->setSettingValue<int64_t>("auto-percent", std::stoi(p0->getString()));
    if (inputID == "resolution-width-input") Mod::get()->setSettingValue<int64_t>("resolution-width", std::stoi(p0->getString()));
    if (inputID == "resolution-height-input") Mod::get()->setSettingValue<int64_t>("resolution-height", std::stoi(p0->getString()));
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