#include "SharedScreenshotLogic.hpp"
#include "ScreenshotPopup.hpp"
#include "Manager.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

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
	this->setID("ScreenshotPopup"_spr);

	CCMenu* resolutionMenu = CCMenu::create();
	resolutionMenu->setPosition(ccp(75.f, 185.f));
	resolutionMenu->setContentWidth(240.f);
	resolutionMenu->setLayout(RowLayout::create()->setAutoScale(false));

	resolutionWidthInput = Build<TextInput>::create(55.f, "Width", "bigFont.fnt")
		.scale(0.75f)
		.parent(resolutionMenu)
		.collect();

	resolutionWidthInput->setString(numToString(Manager::get()->width));
	resolutionWidthInput->getInputNode()->setID("resolution-width-input");
	resolutionWidthInput->getInputNode()->setDelegate(this);

	CCLabelBMFont* xLabel = CCLabelBMFont::create("x", "bigFont.fnt");
	xLabel->setScale(0.7f);
	resolutionMenu->addChild(xLabel);

	resolutionHeightInput = Build<TextInput>::create(55.f, "Height", "bigFont.fnt")
		.scale(0.75f)
		.parent(resolutionMenu)
		.collect();

	resolutionHeightInput->setString(numToString(Manager::get()->height));
	resolutionHeightInput->getInputNode()->setID("resolution-height-input"_spr);
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

	autoPercentInput->setString(numToString(Mod::get()->getSettingValue<int64_t>("auto-percent")));
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

	CCMenuItemSpriteExtra* screenshotButton = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("Screenshot!"),
		this, menu_selector(ScreenshotPopup::onScreenshot)
	);
	screenshotButton->setPosition(ccp(170, 30));
	m_buttonMenu->addChild(screenshotButton);

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
	if (inputID == "auto-percent-input") Mod::get()->setSettingValue<int64_t>("auto-percent", parsedInteger);
	if (inputID == "resolution-width-input") Mod::get()->setSettingValue<int64_t>("resolution-width", parsedInteger);
	if (inputID == "resolution-height-input") Mod::get()->setSettingValue<int64_t>("resolution-height", parsedInteger);
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