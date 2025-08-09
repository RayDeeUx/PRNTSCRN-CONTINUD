#include "Screenshot.hpp"
#include "Manager.hpp"
#include <Geode/Geode.hpp>

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
		RES_NODE(debug-text);
		RES_NODE(testmode-label);
		RES_NODE(percentage-label);
		RES_NODE(mat.run-info/RunInfoWidget);
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
			screenshot(this);
			fields->lastScreenshot = currentPercent;
		} else if (!levelIsClassic && canAutoScreenshot(currentTime)) {
			screenshot(this);
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