#include "Screenshot.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define ADD_NODE(val) uiNodes[#val] = pl->getChildByID(#val)->isVisible(); \
pl->getChildByID(#val)->setVisible(false);

#define ADD_MEM(val) uiNodes[#val] = pl->val->isVisible(); \
pl->val->setVisible(false);

#define RES_NODE(val) pl->getChildByID(#val)->setVisible(uiNodes[#val]);

#define RES_MEM(val) pl->val->setVisible(uiNodes[#val]);

void screenshot(PlayLayer* pl) {
	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	auto scene = director->getRunningScene();

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

#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat) {
		PlayLayer* pl = PlayLayer::get();
		if (down && key == enumKeyCodes::KEY_F2 && pl) {
			screenshot(pl);
		}
		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
	}
};

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayLayer) {
	struct Fields {
		int lastScreenshot = 0;
		bool autoScreenshot = false;
		int autoPercent = 10;
		bool hideUI = false;
	};

	void resetLevel() {
		PlayLayer::resetLevel();
		m_fields->autoScreenshot = Mod::get()->getSettingValue<bool>("auto-screenshot");
		m_fields->autoPercent = Mod::get()->getSettingValue<int64_t>("auto-percent");
		m_fields->hideUI = Mod::get()->getSettingValue<bool>("hide-ui");
		m_fields->lastScreenshot = 0;
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);
		int currentPercent = getCurrentPercentInt();

		if (m_fields->autoScreenshot && currentPercent % m_fields->autoPercent == 0 && m_fields->lastScreenshot != currentPercent) {
			screenshot(this);
			m_fields->lastScreenshot = currentPercent;
		}
	}
};

#include <Geode/modify/PauseLayer.hpp>
#include "ScreenshotPopup.hpp"
class $modify(NewPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto btn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("screenshot.png"_spr),
			this,
			menu_selector(NewPauseLayer::onScreenshotPopup)
		);
		static_cast<CCMenu*>(getChildByID("left-button-menu"))->addChild(btn);
		static_cast<CCMenu*>(getChildByID("left-button-menu"))->updateLayout();
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};