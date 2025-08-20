#include "SharedScreenshotLogic.hpp"
#include "ScreenshotPopup.hpp"
#include "Manager.hpp"
#include <UIBuilder.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace geode::prelude;

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
	Mod* mod = Mod::get();
	CCMenu* quickToggleMenu = Build<CCMenu>(CCMenu::create())
		.id(fmt::format("{}-quick-toggle-setting"_spr, key))
		.layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(5.f))
		.width(180.f)
		.collect();

	CCMenuItemToggler* toggler = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
		[this, key, mod](CCMenuItemToggler* toggler) {
			toggler->toggle(mod->getSettingValue<bool>(key));
			mod->setSettingValue<bool>(key, !mod->getSettingValue<bool>(key));
			if (key == "use-window-width") {
				resolutionWidthInput->setString(numToString(Manager::get()->width));
				resolutionWidthInput->setEnabled(!mod->getSettingValue<bool>(key));
			}
			if (key == "use-window-height") {
				resolutionHeightInput->setString(numToString(Manager::get()->height));
				resolutionHeightInput->setEnabled(!mod->getSettingValue<bool>(key));
			}
		})
		.id(fmt::format("{}-toggler"_spr, key))
		.scale(0.75f)
		.parent(quickToggleMenu)
		.collect();

	Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
		.id(fmt::format("{}-label"_spr, key))
		.scale(0.4f)
		.parent(quickToggleMenu);

	if (auto desc = mod->getSetting(key)->getDescription(); desc.has_value()) {
		const std::string& trueDesc = desc.value();
		InfoAlertButton* infoBtn = Build<InfoAlertButton>::create(title, trueDesc, .5f)
			.id(fmt::format("{}-info"_spr, key))
			.parent(quickToggleMenu)
			.collect();
		const std::string parsedDesc = string::replace(string::replace(string::replace(trueDesc, "</c>", ""), "<cl>", ""), "<cy>", "");
		infoBtn->setUserObject("alphalaneous.tooltips/tooltip", CCString::create(parsedDesc));
	}

	quickToggleMenu->updateLayout();

	toggler->toggle(Mod::get()->getSettingValue<bool>(key)); // this line needs to be called AFTER updatelayout to avoid x-position inconsistencies --raydeeux
	return quickToggleMenu;
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
	// settingsMenu->addChild(createSetting("Use Window Width", "use-window-width")); // ninxout insisted that this stay separate
	// settingsMenu->addChild(createSetting("Use Window Height", "use-window-height")); // ninxout insisted that this stay separate

	if (pl) {
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

		m_mainLayer->addChild(autoPercent);
		autoPercent->updateLayout();
	}

	m_mainLayer->addChild(settingsMenu);
	m_mainLayer->addChild(resolutionMenu);
	settingsMenu->updateLayout();
	resolutionMenu->updateLayout();

	CCMenuItemSpriteExtra* screenshotButton = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("Screenshot!"),
		this, menu_selector(ScreenshotPopup::onScreenshot)
	);
	screenshotButton->setID("screenshot-button"_spr);
	screenshotButton->setPosition(ccp(170, 30));
	m_buttonMenu->addChild(screenshotButton);

	CCMenuItemSpriteExtra* configDirButton = CCMenuItemSpriteExtra::create(
		CircleButtonSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
		this, menu_selector(ScreenshotPopup::onConfigDir)
	);
	configDirButton->setID("screenshots-folder-button"_spr);
	configDirButton->setPosition(ccp(80, 80));
	m_buttonMenu->addChild(configDirButton);

	std::string bodyText;
	std::vector<geode::Ref<keybinds::Bind>> screenshotBinds = keybinds::BindManager::get()->getBindsFor("screenshot"_spr);
	if (!screenshotBinds.empty()) {
		const std::string& keybindString = screenshotBinds.at(0)->toString();
		if (!keybindString.empty()) bodyText += fmt::format("Screenshot the level: <cl>{}</c>", keybindString);
		else bodyText += "Screenshot the level: <co>[Unknown]</c>";
	} else bodyText += "Screenshot the level: <c_>NONE SET</c>";
	std::vector<geode::Ref<keybinds::Bind>> plainScreenshotBinds = keybinds::BindManager::get()->getBindsFor("plain-screenshot"_spr);
	if (!plainScreenshotBinds.empty()) {
		const std::string& keybindString = plainScreenshotBinds.at(0)->toString();
		if (!keybindString.empty()) bodyText += fmt::format("Screenshot the screen: <cl>{}</c>", keybindString);
		else bodyText += "Screenshot the screen: <co>[Unknown]</c>";
	} else bodyText += "Screenshot the screen: <c_>NONE SET</c>";

	InfoAlertButton* infoButton = InfoAlertButton::create(
		"PRNTSCRN Keybinds", bodyText, .7f
	);
	infoButton->setID("keybinds-help-button"_spr);
	infoButton->setPosition(m_mainLayer->getContentSize() - 3.f);
	m_buttonMenu->addChild(infoButton);

	m_buttonMenu->setID("button-menu"_spr);
	m_closeBtn->setID("close-button"_spr);
	m_mainLayer->setID("main-layer"_spr);
	m_bgSprite->setID("background"_spr);

	xLabel->setID("finicky-multiplication-sign"_spr);
	xLabel->setAnchorPoint({.5f, .4f}); // to avoid visual off-center illusion on the y-axis

	return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
	if (PlayLayer* pl = PlayLayer::get()) SharedScreenshotLogic::screenshot(pl);
}

void ScreenshotPopup::onConfigDir(CCObject*) {
	geode::utils::file::openFolder(Mod::get()->getConfigDir());
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