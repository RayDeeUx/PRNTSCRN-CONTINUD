#include "Screenshot.hpp"
#include "Manager.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define ADD_NODE(val)\
	if (auto node = pl->getChildByID(#val)) { \
		uiNodes[#val] = node->isVisible(); \
		node->setVisible(false); \
	}

#define ADD_MEM(val) uiNodes[#val] = pl->val->isVisible(); \
pl->val->setVisible(false);

#define RES_NODE(val) if (auto node = pl->getChildByID(#val)) node->setVisible(uiNodes[#val]);

#define RES_MEM(val) pl->val->setVisible(uiNodes[#val]);

#define SET_WIDTH(value)\
	int& width = Manager::get()->width;\
	width = value;\
	if (width < 1) width = CCDirector::get()->getWinSizeInPixels().width;

#define SET_HEIGHT(value)\
	int& height = Manager::get()->height;\
	height = value;\
	if (height < 1) height = CCDirector::get()->getWinSizeInPixels().height;

constexpr std::array monthNames = {
	"Unknown",
	"January", "February", "March", "April", "May",
	"June", "July", "August", "September", "October", "November", "December",
	"Unknown"
};

void screenshot(CCNode* node) {
	std::unordered_map<const char*, bool> uiNodes = {};
	
	bool hideUI = Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = Mod::get()->getSettingValue<bool>("hide-player");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);

	if (hideUI && pl) {
		ADD_NODE(UILayer);
		ADD_NODE(debug-text);
		ADD_NODE(testmode-label);
		ADD_NODE(percentage-label);
		ADD_NODE(mat.run-info/RunInfoWidget);
		ADD_NODE(progress-bar);
	}
	if (hidePL && pl) {
		ADD_MEM(m_player1);
		ADD_MEM(m_player2);
	}
	Screenshot ss = Screenshot(Manager::get()->width, Manager::get()->height, node);
	if (hideUI && pl) {
		RES_NODE(UILayer);
		RES_NODE(percentage-label);
		RES_NODE(progress-bar);
	}
	if (hidePL && pl) {
		RES_MEM(m_player1);
		RES_MEM(m_player2);
	}

	bool jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
	std::string extension = jpeg ? ".jpg" : ".png";

	auto now = std::chrono::system_clock::now();
	auto floored = std::chrono::floor<std::chrono::days>(now);
	std::chrono::year_month_day ymd = {floored};
	auto humanReadableMonth = monthNames[static_cast<unsigned int>(ymd.month())];
	auto day = static_cast<unsigned int>(ymd.day());
	auto year = static_cast<int>(ymd.year());
	auto formattedDate = fmt::format("{} {}, {}", humanReadableMonth, day, year);

	auto level = pl ? pl->m_level : nullptr;
	std::string suffix = level ? fmt::format("{} - {} ({})", numToString(level->m_levelID.value()), level->m_levelName, formattedDate) : formattedDate;
	std::filesystem::path folder = Mod::get()->getConfigDir() / suffix;

	if (!std::filesystem::exists(folder)) std::filesystem::create_directory(folder);

	int index = 1;
	while (std::filesystem::exists(folder / (std::to_string(index) + extension))) {
		index++;
	}

	std::string filename = fmt::format("{}/{}{}", folder.string(), index, extension);

	ss.intoFile(filename, jpeg);
	if (Mod::get()->getSettingValue<bool>("copy-clipboard")) ss.intoClipboard();
}

#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
$on_mod(Loaded) {
	BindManager::get()->registerBindable({
		"screenshot"_spr,
		"Take a Screenshot!", "Take a screenshot.",
		{ Keybind::create(KEY_F2, Modifier::None) },
		"PRNTSCRN", false
	});
	new EventListener([=](InvokeBindEvent* event) {
		if (!event->isDown()) return ListenerResult::Propagate;
		if (auto pl = PlayLayer::get()) screenshot(pl);
		else screenshot(CCScene::get());
		return ListenerResult::Propagate;
	}, InvokeBindFilter(nullptr, "screenshot"_spr));
	SET_WIDTH(Mod::get()->getSettingValue<int64_t>("resolution-width"));
	SET_HEIGHT(Mod::get()->getSettingValue<int64_t>("resolution-height"));
	listenForSettingChanges("resolution-width", [](int64_t newWidth) {
		SET_WIDTH(newWidth);
	});
	listenForSettingChanges("resolution-height", [](int64_t newHeight) {
		SET_HEIGHT(newHeight);
	});
}

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
		Fields* fields = m_fields.self();

		if (fields->autoScreenshot && currentPercent % fields->autoPercent == 0 && fields->lastScreenshot != currentPercent) {
			screenshot(this);
			fields->lastScreenshot = currentPercent;
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
		leftMenu->addChild(btn);
		leftMenu->updateLayout();
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};