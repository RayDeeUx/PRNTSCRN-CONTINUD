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

void SharedScreenshotLogic::hideOtherPartsOfPlayerOne(std::unordered_map<const char*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
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

void SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(std::unordered_map<const char*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
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

void SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(std::unordered_map<const char*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	RES_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
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

void SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(std::unordered_map<const char *, float>& unorderedMapStoringScales, GJBaseGameLayer *gjbgl) {
	RES_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
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
	std::unordered_map<const char*, float> playerPointerScales = {};
	bool hideUI = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-player");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);
	LevelEditorLayer* lel = typeinfo_cast<LevelEditorLayer*>(node);
	CCScene* scene = typeinfo_cast<CCScene*>(node);

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
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		ADD_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::hideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		ADD_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}
	CCSize size = CCSize{static_cast<float>(Manager::get()->width), static_cast<float>(Manager::get()->height)};
	if (!pl && !lel && !scene) {
		const CCSize sceneSize = CCScene::get()->getContentSize();
		const CCSize nodeSize = node->getContentSize();
		size = node->getContentSize() * geode::utils::getDisplayFactor() * geode::utils::getDisplayFactor() * std::max(nodeSize.width / sceneSize.width, nodeSize.height / sceneSize.height);
	}
	Screenshot ss = Screenshot(size, node);
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
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		RES_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		RES_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}

	bool jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
	std::string extension = jpeg ? ".jpg" : ".png";

	std::string formattedDate = SharedScreenshotLogic::getFormattedDate();

	GJGameLevel* level = nullptr;
	if (pl) level = pl->m_level;
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