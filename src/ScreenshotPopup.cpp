#include "SharedScreenshotLogic.hpp"
#include "ScreenshotPopup.hpp"
#include "Manager.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
	CCMenu* thing = Build<CCMenu>(CCMenu::create())
		.id(fmt::format("{}-quick-toggle-setting"_spr, key))
		.layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(5.f))
		.width(180.f)
		.collect();

	CCMenuItemToggler* toggler = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
	[this, key, mod = Mod::get()](CCMenuItemToggler* toggler) {
		toggler->toggle(mod->getSettingValue<bool>(key));
		mod->setSettingValue<bool>(key, !mod->getSettingValue<bool>(key));
		if (key == "use-window-width") {
			int pixelWinSize = static_cast<int>(CCDirector::get()->getWinSizeInPixels().width);
			bool isKeyEnabled = mod->getSettingValue<bool>(key);
			std::string numericString = isKeyEnabled ? numToString(pixelWinSize) : numToString(mod->getSettingValue<int64_t>("resolution-width"));
			resolutionWidthInput->setString(numericString);
			resolutionWidthInput->setEnabled(!isKeyEnabled);
		}
		if (key == "use-window-height") {
			int pixelWinSize = static_cast<int>(CCDirector::get()->getWinSizeInPixels().height);
			bool isKeyEnabled = mod->getSettingValue<bool>(key);
			std::string numericString = isKeyEnabled ? numToString(pixelWinSize) : numToString(mod->getSettingValue<int64_t>("resolution-height"));
			resolutionHeightInput->setString(numericString);
			resolutionHeightInput->setEnabled(!isKeyEnabled);
		}
	})
		.id(fmt::format("{}-toggler"_spr, key))
		.scale(0.75f)
		.parent(thing)
		.collect();

	Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
		.id(fmt::format("{}-label"_spr, key))
		.scale(0.4f)
		.parent(thing);

	thing->updateLayout();

	toggler->toggle(Mod::get()->getSettingValue<bool>(key)); // this line needs to be called AFTER updatelayout to avoid x-position inconsistencies --raydeeux
	return thing;
}

bool ScreenshotPopup::setup() {
	this->setTitle("Screenshot");
	this->setID("ScreenshotPopup"_spr);
	this->m_title->setID("title"_spr);

	PlayLayer* pl = PlayLayer::get();
	bool isPlatformerLevel = pl && pl->m_level && pl->m_level->isPlatformer();

	CCMenu* resolutionMenu = CCMenu::create();
	resolutionMenu->setPosition(ccp(75.f, 185.f));
	resolutionMenu->setContentWidth(110.f);
	resolutionMenu->setLayout(RowLayout::create()->setAutoScale(false));
	resolutionMenu->setID("resolution-menu"_spr);

	resolutionWidthInput = Build<TextInput>::create(55.f, "Width", "bigFont.fnt")
		.id("width-geode-input"_spr)
		.scale(0.75f)
		.parent(resolutionMenu)
		.collect();

	resolutionWidthInput->setString(numToString(Manager::get()->width));
	resolutionWidthInput->getInputNode()->setID("resolution-width-input"_spr);
	resolutionWidthInput->getInputNode()->setDelegate(this);
	resolutionWidthInput->setEnabled(!Mod::get()->getSettingValue<bool>("use-window-width"));

	CCLabelBMFont* xLabel = CCLabelBMFont::create("x", "bigFont.fnt");
	xLabel->setScale(0.7f);
	resolutionMenu->addChild(xLabel);

	resolutionHeightInput = Build<TextInput>::create(55.f, "Height", "bigFont.fnt")
		.id("height-geode-input"_spr)
		.scale(0.75f)
		.parent(resolutionMenu)
		.collect();

	resolutionHeightInput->setString(numToString(Manager::get()->height));
	resolutionHeightInput->getInputNode()->setID("resolution-height-input"_spr);
	resolutionHeightInput->getInputNode()->setDelegate(this);
	resolutionHeightInput->setEnabled(!Mod::get()->getSettingValue<bool>("use-window-height"));

	CCMenu* settingsMenu = CCMenu::create();
	settingsMenu->setID("quick-settings"_spr);
	settingsMenu->setPosition(ccp(235.f, 125.f));
	settingsMenu->setContentSize({180.f, 145.f});
	settingsMenu->setLayout(ColumnLayout::create()->setAutoScale(true)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End));

	settingsMenu->addChild(createSetting("Copy To Clipboard", "copy-clipboard"));
	settingsMenu->addChild(createSetting("Hide UI Layer", "hide-ui"));
	settingsMenu->addChild(createSetting("Hide Player", "hide-player"));
	settingsMenu->addChild(createSetting("JPEG", "jpeg-mafia"));
	settingsMenu->addChild(createSetting("Auto Screenshot", "auto-screenshot"));
	settingsMenu->addChild(createSetting("Use Window Width", "use-window-width"));
	settingsMenu->addChild(createSetting("Use Window Height", "use-window-height"));

	int64_t selectedSetting = isPlatformerLevel ? Mod::get()->getSettingValue<int64_t>("auto-seconds") : Mod::get()->getSettingValue<int64_t>("auto-percent");
	std::string selectedPlaceholderString = isPlatformerLevel ? "s" : "%";
	std::string selectedInputNodeID = isPlatformerLevel ? "auto-seconds-input"_spr : "auto-percent-input"_spr;
	std::string selectedGeodeInputNodeID = isPlatformerLevel ? "seconds-geode-input"_spr : "percent-geode-input"_spr;
	std::string selectedLabel = isPlatformerLevel ? "Auto Seconds" : "Auto Percent";

	CCMenu* autoPercent = Build<CCMenu>::create()
		.layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(4.f)->setAxisReverse(true))
		.id("auto-screenshot"_spr)
		.pos(75, 150)
		.width(125.f)
		.collect();

	autoPercentInput = Build<TextInput>::create(35.f, selectedPlaceholderString, "bigFont.fnt")
		.id(selectedGeodeInputNodeID)
		.scale(0.75f)
		.parent(autoPercent)
		.collect();

	autoPercentInput->setString(numToString(selectedSetting));
	autoPercentInput->getInputNode()->setID(selectedInputNodeID);
	autoPercentInput->getInputNode()->setDelegate(this);

	Build<CCLabelBMFont>::create(selectedLabel.c_str(), "bigFont.fnt")
		.id("auto-screenshot-label"_spr)
		.scale(0.4f)
		.parent(autoPercent);

	m_mainLayer->addChild(settingsMenu);
	m_mainLayer->addChild(resolutionMenu);
	m_mainLayer->addChild(autoPercent);
	settingsMenu->updateLayout();
	resolutionMenu->updateLayout();
	autoPercent->updateLayout();

	CCMenuItemSpriteExtra* screenshotButton = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("Screenshot!"),
		this, menu_selector(ScreenshotPopup::onScreenshot)
	);
	screenshotButton->setID("screenshot-button"_spr);
	screenshotButton->setPosition(ccp(170, 30));
	m_buttonMenu->addChild(screenshotButton);

	m_buttonMenu->setID("button-menu"_spr);
	m_closeBtn->setID("close-button"_spr);
	m_mainLayer->setID("main-layer"_spr);
	m_bgSprite->setID("background"_spr);

	xLabel->setAnchorPoint({.5f, .4f}); // to avoid visual off-center illusion on the y-axis

	return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
	if (PlayLayer* pl = PlayLayer::get()) SharedScreenshotLogic::screenshot(pl);
}

void ScreenshotPopup::textChanged(CCTextInputNode* p0) {
	const std::string& inputString = p0->getString();
	if (inputString.empty()) return;

	const std::string& inputID = p0->getID();
	int parsedInteger = geode::utils::numFromString<int>(inputString).unwrapOr(0);
	if (inputID == "auto-percent-input"_spr) Mod::get()->setSettingValue<int64_t>("auto-percent", parsedInteger);
	if (inputID == "auto-seconds-input"_spr) Mod::get()->setSettingValue<int64_t>("auto-seconds", parsedInteger);
	if (inputID == "resolution-width-input"_spr) Mod::get()->setSettingValue<int64_t>("resolution-width", parsedInteger);
	if (inputID == "resolution-height-input"_spr) Mod::get()->setSettingValue<int64_t>("resolution-height", parsedInteger);
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