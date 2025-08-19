// also you can keep this mod i dont want it
// keep me and mat in the dev field somewhere and its all urs
// -- ninxout to me (raydeeux), in discord DMs
#include "SharedScreenshotLogic.hpp"
#include "Manager.hpp"
#include <api.hpp>

using namespace geode::prelude;

void setWidth() {
	int& width = Manager::get()->width;
	int widthPixel = static_cast<int>(CCDirector::get()->getWinSizeInPixels().width);
	if (Mod::get()->getSettingValue<bool>("use-window-width")) {
		width = widthPixel;
	} else {
		int widthSetting = static_cast<int>(Mod::get()->getSettingValue<int64_t>("resolution-width"));
		width = std::clamp(widthSetting, 1, widthPixel * 4); // if anyone wants to do higher resolutions than 4x their window dimensions wnat are you doing with your life --raydeeux
	}
}

void setHeight() {
	int &height = Manager::get()->height;
	int heightPixel = static_cast<int>(CCDirector::get()->getWinSizeInPixels().height);
	if (Mod::get()->getSettingValue<bool>("use-window-height")) {
		height = heightPixel;
	} else {
		int heightSetting = static_cast<int>(Mod::get()->getSettingValue<int64_t>("resolution-height"));
		height = std::clamp(heightSetting, 1, heightPixel * 4); // if anyone wants to do higher resolutions than 4x their window dimensions wnat are you doing with your life --raydeeux
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
		"Take a Plain Screenshot!", "Takes a screenshot of the screen itself, even if there's a level on screen. Useful for taking a screenshot of the pause menu.",
		{ Keybind::create(KEY_F2, Modifier::Control), Keybind::create(KEY_GraveAccent, Modifier::Control) }, // added GraveAccent as a default for any macOS users with the Touch Bar --raydeeux
		"PRNTSCRN", false
	}); // y'know, in case anyone wants to take a screenshot of the pause layer --raydeeux
	new EventListener([=](InvokeBindEvent* event) {
		if (!event->isDown()) return ListenerResult::Propagate;
		CCNode* nodeToScreenshot = CCScene::get();
		if (PlayLayer* pl = PlayLayer::get(); pl) nodeToScreenshot = pl;
		else if (LevelEditorLayer* lel = LevelEditorLayer::get(); lel) nodeToScreenshot = lel;
		if (nodeToScreenshot) SharedScreenshotLogic::screenshot(nodeToScreenshot);
		return ListenerResult::Propagate;
	}, InvokeBindFilter(nullptr, "screenshot"_spr));
	new EventListener([=](InvokeBindEvent* event) {
		if (event->isDown() && CCScene::get()) SharedScreenshotLogic::screenshot(CCScene::get());
		return ListenerResult::Propagate;
	}, InvokeBindFilter(nullptr, "plain-screenshot"_spr));
	setWidth();
	setHeight();
	listenForSettingChanges("resolution-width", [](int64_t unused) { setWidth(); });
	listenForSettingChanges("resolution-height", [](int64_t unused) { setHeight(); });
	listenForSettingChanges("use-window-width", [](bool unused) { setWidth(); });
	listenForSettingChanges("use-window-height", [](bool unused) { setHeight(); });
	new EventListener<EventFilter<PRNTSCRN::ScreenshotEvent>>(+[](PRNTSCRN::ScreenshotEvent* ev) {
		if (!ev) {
			log::error("[PRNTSCRN API] THE EVENT WAS NULLPTR.");
			return ListenerResult::Stop;
		}
		if (!ev->getNode()) {
			log::error("[PRNTSCRN API] THE NODE WAS NULLPTR.");
			return ListenerResult::Stop;
		}
		std::unordered_map<std::string, bool> formerNodeIDsVisibilityStates = {};
		std::unordered_map<CCNode*, bool> formerNodePointersVisibilityStates = {};
		std::unordered_map<CCNode*, float> formerPlayerOnePointersScaleStates = {};
		std::unordered_map<CCNode*, float> formerPlayerTwoPointersScaleStates = {};

		CCNode* nodeBeingScreenshotted = ev->getNode();
		nodeBeingScreenshotted->setUserObject("has-custom-nodes-to-hide"_spr, CCBool::create(false));

		std::vector<CCNode*> hideThesePointers = ev->getPointersToHide();
		std::vector<std::string> hideTheseQuerySelectors = ev->getQuerysToHide();
		if (!hideThesePointers.empty() || !hideTheseQuerySelectors.empty()) {
			static_cast<CCBool*>(nodeBeingScreenshotted->getUserObject("has-custom-nodes-to-hide"_spr))->setValue(true);
		}
		if (!hideThesePointers.empty()) {
			for (CCNode* nodeToHide : hideThesePointers) {
				if (!nodeToHide) continue;
				if (nodeToHide == nodeBeingScreenshotted) continue;
				formerNodePointersVisibilityStates[nodeToHide] = nodeToHide->isVisible();
				nodeToHide->setVisible(false);
				auto* gjbgl = GJBaseGameLayer::get();
				if (!gjbgl) continue;
				if (nodeToHide == gjbgl->m_player1) SharedScreenshotLogic::hideOtherPartsOfPlayerOne(formerPlayerOnePointersScaleStates, gjbgl);
				if (nodeToHide == gjbgl->m_player2) SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(formerPlayerTwoPointersScaleStates, gjbgl);
			}
		}
		if (!hideTheseQuerySelectors.empty()) {
			for (std::string querySelectorToHide : hideTheseQuerySelectors) {
				if (querySelectorToHide.empty()) continue;
				CCNode* theNodeToHide = nodeBeingScreenshotted->querySelector(querySelectorToHide);
				if (!theNodeToHide) continue;
				if (theNodeToHide == nodeBeingScreenshotted) continue;
				formerNodeIDsVisibilityStates[querySelectorToHide] = theNodeToHide->isVisible();
				theNodeToHide->setVisible(false);
			}
		}
		SharedScreenshotLogic::screenshot(nodeBeingScreenshotted);
		if (!hideThesePointers.empty() && !formerNodePointersVisibilityStates.empty()) {
			for (auto [nodeToRestore, formerVisibility] : formerNodePointersVisibilityStates) {
				if (!nodeToRestore) continue;
				nodeToRestore->setVisible(formerVisibility);
				auto* gjbgl = GJBaseGameLayer::get();
				if (!gjbgl) continue;
				if (nodeToRestore == gjbgl->m_player1) SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(formerPlayerOnePointersScaleStates, gjbgl);
				if (nodeToRestore == gjbgl->m_player2) SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(formerPlayerTwoPointersScaleStates, gjbgl);
			}
		}
		if (!hideTheseQuerySelectors.empty() && !formerNodeIDsVisibilityStates.empty()) {
			for (auto [querySelectorToRestore, formerVisibility] : formerNodeIDsVisibilityStates) {
				CCNode* theNodeToRestore = nodeBeingScreenshotted->querySelector(querySelectorToRestore);
				if (!theNodeToRestore) continue;
				theNodeToRestore->setVisible(formerVisibility);
			}
		}
		if (!hideThesePointers.empty() || !hideTheseQuerySelectors.empty()) {
			static_cast<CCBool*>(nodeBeingScreenshotted->getUserObject("has-custom-nodes-to-hide"_spr))->setValue(false);
		}
		return ListenerResult::Stop;
	});
}

#include <Geode/modify/PlayLayer.hpp>
class $modify(AutoScreenshotPlayLsoayer, PlayLayer) {
	struct Fields {
		int lastScreenshot = 0;
		bool autoScreenshot = false;
		int autoPercent = 10;
		int autoSeconds = 10;
	};

	bool canAutoScreenshot(int currentPercentOrTime) {
		Fields* fields = m_fields.self();
		if (!m_level || !fields->autoScreenshot) return false;
		if (fields->lastScreenshot == currentPercentOrTime) return false;
		if (m_level->isPlatformer()) return currentPercentOrTime % fields->autoSeconds == 0;
		if (!m_level->isPlatformer()) return currentPercentOrTime % fields->autoPercent == 0;
		return false;
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		m_fields->autoScreenshot = Mod::get()->getSettingValue<bool>("auto-screenshot");
		m_fields->autoPercent = std::clamp(static_cast<int>(Mod::get()->getSettingValue<int64_t>("auto-percent")), 5, 100);
		m_fields->autoSeconds = std::clamp(static_cast<int>(Mod::get()->getSettingValue<int64_t>("auto-seconds")), 5, 604800); // 1 week == 604800 seconds
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

#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(NewEditorPauseLayer, EditorPauseLayer) {
	bool init(LevelEditorLayer* lel) {
		if (!EditorPauseLayer::init(lel)) return false;
		auto settingsMenu = getChildByID("settings-menu");
		if (!settingsMenu) return true;

		auto btn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("screenshot.png"_spr, 1, CircleBaseColor::Green, CircleBaseSize::Tiny),
			this, menu_selector(NewEditorPauseLayer::onScreenshotPopup)
		);
		btn->setID("screenshot-button"_spr);
		settingsMenu->addChild(btn);
		settingsMenu->updateLayout();
		return true;
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};