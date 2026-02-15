#include "SharedScreenshotLogic.hpp"
#include "ScreenshotPopup.hpp"
#include "Manager.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
	Mod* mod = Mod::get();
	CCMenu* quickToggleMenu = Build<CCMenu>(CCMenu::create())
		.id(fmt::format("{}-quick-toggle-setting"_spr, key))
		.layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(5.f))
		.width(240.f)
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
		InfoAlertButton* infoBtn = Build<InfoAlertButton>::create(fmt::format("PRNTSCRN - {}", title), trueDesc, .5f)
			.id(fmt::format("{}-info"_spr, key))
			.parent(quickToggleMenu)
			.collect();
		const std::string parsedDesc = string::replace(string::replace(string::replace(string::replace(trueDesc, "</c>", ""), "<cl>", ""), "<cy>", ""), "<cp>", "");
		infoBtn->setUserObject("alphalaneous.tooltips/tooltip", CCString::create(parsedDesc));
	}

	quickToggleMenu->updateLayout();

	toggler->toggle(Mod::get()->getSettingValue<bool>(key)); // this line needs to be called AFTER updatelayout to avoid x-position inconsistencies --raydeeux
	return quickToggleMenu;
}

bool ScreenshotPopup::init() {
	constexpr float popupWidth = 360.f;
	if (!geode::Popup::init(popupWidth, 270.f, "GJ_square02.png")) return false;

	this->setTitle("Screenshot");
	this->setID("ScreenshotPopup"_spr);
	this->m_title->setID("title"_spr);

	PlayLayer* pl = PlayLayer::get();
	LevelEditorLayer* lel = LevelEditorLayer::get();
	bool isPlatformerLevel = pl && pl->m_level && pl->m_level->isPlatformer();

	CCMenu* resolutionMenu = CCMenu::create();
	resolutionMenu->setPosition(ccp(75.f, 205.f));
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
	settingsMenu->setPosition(ccp(245.f, 142.5f));
	settingsMenu->setContentSize({180.f, 185.f});
	settingsMenu->setLayout(ColumnLayout::create()->setAutoScale(true)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::Center));

	#ifdef GEODE_IS_DESKTOP
	settingsMenu->addChild(createSetting("Copy To Clipboard", "copy-clipboard"));
	#endif
	settingsMenu->addChild(createSetting("Hide UI", "hide-ui"));
	settingsMenu->addChild(createSetting("Hide Player", "hide-player"));
	settingsMenu->addChild(createSetting("Hide Other Players", "hide-multiplayers"));
	settingsMenu->addChild(createSetting("Hide Noclip Tints", "hide-noclip-tints"));
	if (pl) {
		settingsMenu->addChild(createSetting("Hide Attempt Label", "hide-attempts"));
		settingsMenu->addChild(createSetting("Hide Checkpoints", "hide-checkpoints"));
	} else if (lel) {
		settingsMenu->addChild(createSetting("Hide Grid", "hide-draw-grid-layer"));
		settingsMenu->addChild(createSetting("Hide Hitboxes/Debug Draw", "hide-debug-draw"));
		settingsMenu->addChild(createSetting("Hide Playtest Path + Clicks", "hide-playtest-path-clicks"));
	}
	#ifdef GEODE_IS_DESKTOP
	settingsMenu->addChild(createSetting("JPEG", "jpeg-mafia"));
	#endif
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
			.pos(75, 170)
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
	screenshotButton->setPosition(ccp(popupWidth / 2.f, 30));
	m_buttonMenu->addChild(screenshotButton);

	CCMenuItemSpriteExtra* configDirButton = CCMenuItemSpriteExtra::create(
		CircleButtonSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
		this, menu_selector(ScreenshotPopup::onConfigDir)
	);
	configDirButton->setID("screenshots-folder-button"_spr);
	configDirButton->setPosition(ccp(80, 80));
	m_buttonMenu->addChild(configDirButton);

	#ifndef GEODE_IS_IOS
	InfoAlertButton* infoButton = InfoAlertButton::create("PRNTSCRN - Keybinds", Manager::fetchKeybindsStrings(), .7f);
	infoButton->setID("keybinds-help-button"_spr);
	infoButton->setPosition(m_mainLayer->getContentSize() - 3.f);
	m_buttonMenu->addChild(infoButton);
	#endif

	m_buttonMenu->setID("button-menu"_spr);
	m_closeBtn->setID("close-button"_spr);
	m_mainLayer->setID("main-layer"_spr);
	m_bgSprite->setID("background"_spr);

	xLabel->setID("finicky-multiplication-sign"_spr);
	xLabel->setAnchorPoint({.5f, .4f}); // to avoid visual off-center illusion on the y-axis

	return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
	bool isControl = CCKeyboardDispatcher::get()->getControlKeyPressed();
	if (PlayLayer* pl = PlayLayer::get(); pl && !isControl) SharedScreenshotLogic::screenshot(pl);
	else if (LevelEditorLayer* lel = LevelEditorLayer::get(); lel && !isControl) SharedScreenshotLogic::screenshot(lel);
	else if (CCScene* scene = CCScene::get(); scene) {
		scene->setUserObject("is-plain-ss-from-popup"_spr, CCBool::create(true));
		SharedScreenshotLogic::screenshot(scene);
		scene->setUserObject("is-plain-ss-from-popup"_spr, CCBool::create(false));
	}
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
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}
