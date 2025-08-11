#include "SharedScreenshotLogic.hpp"
#include "Screenshot.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

void SharedScreenshotLogic::screenshot(CCNode* node) {
	std::unordered_map<const char*, bool> uiNodes = {};

	bool hideUI = Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = Mod::get()->getSettingValue<bool>("hide-player");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);
	LevelEditorLayer* lel = typeinfo_cast<LevelEditorLayer*>(node);

	if (hideUI && pl) {
		ADD_NODE(pl, UILayer);
		ADD_NODE(pl, debug-text);
		ADD_NODE(pl, testmode-label);
		ADD_NODE(pl, percentage-label);
		ADD_NODE(pl, mat.run-info/RunInfoWidget);
		ADD_NODE(pl, cheeseworks.speedruntimer/timer);
		ADD_NODE(pl, progress-bar);
	}
	if (hideUI && lel) {
		ADD_NODE(lel, UILayer);
		ADD_NODE(lel, EditorUI);
	}
	if (hidePL && pl) {
		ADD_MEM(pl, m_player1);
		ADD_MEM(pl, m_player2);
	}
	if (hidePL && lel) {
		ADD_MEM(lel, m_player1);
		ADD_MEM(lel, m_player2);
	}
	Screenshot ss = Screenshot(Manager::get()->width, Manager::get()->height, node);
	if (hideUI && pl) {
		RES_NODE(pl, UILayer);
		RES_NODE(pl, debug-text);
		RES_NODE(pl, testmode-label);
		RES_NODE(pl, percentage-label);
		RES_NODE(pl, mat.run-info/RunInfoWidget);
		RES_NODE(pl, cheeseworks.speedruntimer/timer);
		RES_NODE(pl, progress-bar);
	}
	if (hideUI && lel) {
		RES_NODE(lel, UILayer);
		RES_NODE(lel, EditorUI);
	}
	if (hidePL && pl) {
		RES_MEM(pl, m_player1);
		RES_MEM(pl, m_player2);
	}
	if (hidePL && lel) {
		RES_MEM(lel, m_player1);
		RES_MEM(lel, m_player2);
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