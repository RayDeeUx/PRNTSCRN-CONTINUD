// ReSharper disable CppDFAUnreachableCode
// yeah that's right clion stop saying my code is unreachable >:(
#include "SharedScreenshotLogic.hpp"
#include "Screenshot.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

std::string SharedScreenshotLogic::getFormattedDate() {
	auto now = std::chrono::system_clock::now();
	auto floored = std::chrono::floor<std::chrono::days>(now);
	std::chrono::year_month_day ymd = {floored};
	auto humanReadableMonth = SharedScreenshotLogic::monthNames[static_cast<unsigned int>(ymd.month())];
	auto day = static_cast<unsigned int>(ymd.day());
	auto year = static_cast<int>(ymd.year());
	return fmt::format("{} {}, {}", humanReadableMonth, day, year);
}

void SharedScreenshotLogic::hideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_regularTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashFireSprite, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_landParticles0, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_landParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_ufoClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_trailingParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_shipClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_robotBurstParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_swingBurstParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_swingBurstParticles2, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashSpritesContainer, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_playerGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_vehicleGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales);
}

void SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_regularTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashFireSprite, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_landParticles0, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_landParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_ufoClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_trailingParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_shipClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_robotBurstParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_swingBurstParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_swingBurstParticles2, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashSpritesContainer, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_playerGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_vehicleGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales);
}

void SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	RES_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_regularTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashFireSprite, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_landParticles0, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_landParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_ufoClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_trailingParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_shipClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_robotBurstParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_swingBurstParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_swingBurstParticles2, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashSpritesContainer, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_playerGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_vehicleGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales);
}

void SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer *gjbgl) {
	RES_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_regularTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashFireSprite, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_landParticles0, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_landParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_ufoClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_trailingParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_shipClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_robotBurstParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_swingBurstParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_swingBurstParticles2, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashSpritesContainer, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_playerGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_vehicleGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales);
}

void SharedScreenshotLogic::screenshot(CCNode* node) {
	if (!node) return log::error("invalid node!");
	bool hasCustomNodesToHide = false;
	if (CCBool* obj = typeinfo_cast<CCBool*>(node->getUserObject("has-custom-nodes-to-hide"_spr)); obj) {
		hasCustomNodesToHide = obj->getValue();
	}

	// event filter from main.cpp will have already hidden the nodes by this point
	std::unordered_map<const char*, bool> uiNodes = {};
	std::unordered_map<CCNode*, float> playerPointerScales = {};
	bool hideUI = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-player");

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
		ADD_NODE(lel, EditorPauseLayer);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		ADD_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::hideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		ADD_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
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
		RES_NODE(lel, EditorPauseLayer);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		RES_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		RES_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}

	std::string modIDAskingForScreenshot;
	std::string pauseMenuTypeForSetting;

	if (CCString* modID = typeinfo_cast<CCString*>(node->getUserObject("mod-asking-for-screenshot"_spr)); modID) {
		auto cString = static_cast<std::string>(modID->getCString());
		if (!cString.empty()) modIDAskingForScreenshot = fmt::format("{} - ", cString);
	}

	if (CCString* pauseMenuType = typeinfo_cast<CCString*>(node->getUserObject("pause-menu-type"_spr)); pauseMenuType) {
		auto cString = static_cast<std::string>(pauseMenuType->getCString());
		if (!cString.empty()) pauseMenuTypeForSetting = cString;
	}

	bool jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
	std::string extension = jpeg ? ".jpg" : ".png";

	std::string formattedDate = SharedScreenshotLogic::getFormattedDate();
	std::string targetFolderName = modIDAskingForScreenshot.empty() ? "" : modIDAskingForScreenshot;

	GJGameLevel* level = nullptr;
	if (pl || lel) level = static_cast<GJBaseGameLayer*>(node)->m_level;
	if (level) {
		const std::string& lvlIDString = level->m_levelType == GJLevelType::Editor ? "Editor level" : numToString(level->m_levelID);
		targetFolderName += fmt::format("{} - {} ({})", lvlIDString, level->m_levelName, formattedDate);
	} else targetFolderName += formattedDate;

	std::filesystem::path folder = Mod::get()->getConfigDir() / targetFolderName;

	if (!std::filesystem::exists(folder)) std::filesystem::create_directory(folder);

	int index = 1;
	while (std::filesystem::exists(folder / (numToString(index) + extension))) {
		index++;
	}

	std::string filename = folder / (numToString(index) + extension);
	ss.intoFile(filename, jpeg);
	if (Mod::get()->getSettingValue<bool>("copy-clipboard") && (modIDAskingForScreenshot.empty() || modIDAskingForScreenshot == Mod::get()->getID())) {
		bool shouldCopy = true;
		if (!pauseMenuTypeForSetting.empty()) {
			shouldCopy = false;
			const std::string& pauseMenuPreference = utils::string::toLower(Mod::get()->getSettingValue<std::string>("copy-screenshot-with-pause-menu-on"));
			// "Never", "Regular levels only", "Editor levels only", "All levels"
			if (pauseMenuPreference == "all levels") shouldCopy = true;
			else if (pauseMenuPreference == "never") shouldCopy = false;
			else if (pauseMenuPreference == "editor levels only" && pauseMenuTypeForSetting == "EditorPauseLayer") shouldCopy = true;
			else if (pauseMenuPreference == "regular levels only" && pauseMenuTypeForSetting == "PauseLayer") shouldCopy = true;
		}
		if (shouldCopy) ss.intoClipboard();
	}
}