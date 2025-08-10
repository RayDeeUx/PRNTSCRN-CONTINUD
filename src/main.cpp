#include "SharedScreenshotLogic.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

void setWidth() {
	int& width = Manager::get()->width;
	width = static_cast<int>(Mod::get()->getSettingValue<int64_t>("resolution-width"));
	if (width < 1 || Mod::get()->getSettingValue<bool>("use-window-width")) {
		width = static_cast<int>(CCDirector::get()->getWinSizeInPixels().width);
	}
}

void setHeight() {
	int &height = Manager::get()->height;
	height = static_cast<int>(Mod::get()->getSettingValue<int64_t>("resolution-height"));
	if (height < 1 || Mod::get()->getSettingValue<bool>("use-window-height")) {
		height = static_cast<int>(CCDirector::get()->getWinSizeInPixels().height);
	}
}

#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
$on_mod(Loaded) {
	BindManager::get()->registerBindable({
		"screenshot"_spr,
		"Take a Screenshot!", "Takes a screenshot of the level on screen, if there is one. Otherwise, takes a screenshot of the screen itself.",
		{ Keybind::create(KEY_F2, Modifier::None), Keybind::create(KEY_GraveAccent, Modifier::None) }, // added GraveAccent as a default for any macOS users with the Touch Bar --raydeeux
		"PRNTSCRN", false
	});
	BindManager::get()->registerBindable({
		"plain-screenshot"_spr,
		"Take a Plain Screenshot!", "Takes a screenshot of the screen itself, even if there's a level on screen.",
		{ Keybind::create(KEY_F2, Modifier::Shift), Keybind::create(KEY_GraveAccent, Modifier::Shift) }, // added GraveAccent as a default for any macOS users with the Touch Bar --raydeeux
		"PRNTSCRN", false
	}); // y'know, in case anyone wants to take a screenshot of the pause layer --raydeeux
	new EventListener([=](InvokeBindEvent* event) {
		if (!event->isDown()) return ListenerResult::Propagate;
		CCNode* nodeToScreenshot = CCScene::get();
		if (PlayLayer* pl = PlayLayer::get(); pl) nodeToScreenshot = pl;
		SharedScreenshotLogic::screenshot(nodeToScreenshot);
		return ListenerResult::Propagate;
	}, InvokeBindFilter(nullptr, "screenshot"_spr));
	new EventListener([=](InvokeBindEvent* event) {
		if (event->isDown()) SharedScreenshotLogic::screenshot(CCScene::get());
		return ListenerResult::Propagate;
	}, InvokeBindFilter(nullptr, "plain-screenshot"_spr));
	setWidth();
	setHeight();
	listenForSettingChanges("resolution-width", [](int64_t unsused) { setWidth(); });
	listenForSettingChanges("resolution-height", [](int64_t unsused) { setHeight(); });
	listenForSettingChanges("use-window-width", [](bool unsused) { setWidth(); });
	listenForSettingChanges("use-window-height", [](bool unsused) { setHeight(); });
}

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayLayer) {
	struct Fields {
		int lastScreenshot = 0;
		bool autoScreenshot = false;
		int autoPercent = 10;
		int autoSeconds = 10;
		bool hideUI = false;
	};

	bool canAutoScreenshot(int currentPercentOrTime) {
		Fields* fields = m_fields.self();
		if (!m_level || !fields->autoScreenshot) return false;
		return currentPercentOrTime % fields->lastScreenshot == 0 && fields->lastScreenshot != currentPercentOrTime;
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		m_fields->autoScreenshot = Mod::get()->getSettingValue<bool>("auto-screenshot");
		m_fields->autoPercent = std::clamp(static_cast<int>(Mod::get()->getSettingValue<int64_t>("auto-percent")), 5, 100);
		m_fields->autoSeconds = std::clamp(static_cast<int>(Mod::get()->getSettingValue<int64_t>("auto-seconds")), 5, 604800); // 1 week == 604800 seconds
		m_fields->hideUI = Mod::get()->getSettingValue<bool>("hide-ui");
		m_fields->lastScreenshot = 0;
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);
		Fields* fields = m_fields.self();
		if (!m_level || !fields->autoScreenshot) return;
		int currentPercent = getCurrentPercentInt(); // in % as integer
		int currentTime = static_cast<int>(m_timePlayed); // in seconds
		bool levelIsClassic = !m_level->isPlatformer();

		if (levelIsClassic && canAutoScreenshot(currentPercent)) {
			SharedScreenshotLogic::screenshot(this);
			fields->lastScreenshot = currentPercent;
		} else if (!levelIsClassic && canAutoScreenshot(currentTime)) {
			SharedScreenshotLogic::screenshot(this);
			fields->lastScreenshot = currentTime;
		}
	}
};

#include <Geode/modify/PauseLayer.hpp>
#include "ScreenshotPopup.hpp"
class $modify(NewPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto leftMenu = getChildByID("left-button-menu");
		if (!leftMenu) return;

		auto btn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("screenshot.png"_spr),
			this, menu_selector(NewPauseLayer::onScreenshotPopup)
		);
		btn->setID("screenshot-button"_spr);
		leftMenu->addChild(btn);
		leftMenu->updateLayout();
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};