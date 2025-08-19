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

void SharedScreenshotLogic::screenshot(CCNode* node) {
	if (!node) return log::error("invalid node!");
	bool hasCustomNodesToHide = false;
	if (CCBool* obj = typeinfo_cast<CCBool*>(node->getUserObject("has-custom-nodes-to-hide"_spr)); obj) {
		hasCustomNodesToHide = obj->getValue();
	}

	// event filter from main.cpp will have already hidden the nodes by this point
	std::unordered_map<const char*, bool> uiNodes = {};
	std::unordered_map<const char*, float> playerPointerOpacities = {};
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
		if (CCSprite* fuckYou = gjbgl->m_player1->m_dashSpritesContainer) {
			fuckYou->setCascadeOpacityEnabled(true);
		}
		ADD_MEM(gjbgl, m_player1);

		ADD_OPACITY(gjbgl, m_player1->m_waveTrail);
		ADD_OPACITY(gjbgl, m_player1->m_ghostTrail);
		ADD_OPACITY(gjbgl, m_player1->m_shipStreak);
		ADD_OPACITY(gjbgl, m_player1->m_dashParticles);
		ADD_OPACITY(gjbgl, m_player1->m_dashFireSprite);
		ADD_OPACITY(gjbgl, m_player1->m_landParticles0);
		ADD_OPACITY(gjbgl, m_player1->m_landParticles1);
		ADD_OPACITY(gjbgl, m_player1->m_ufoClickParticles);
		ADD_OPACITY(gjbgl, m_player1->m_trailingParticles);
		ADD_OPACITY(gjbgl, m_player1->m_shipClickParticles);
		ADD_OPACITY(gjbgl, m_player1->m_robotBurstParticles);
		ADD_OPACITY(gjbgl, m_player1->m_swingBurstParticles1);
		ADD_OPACITY(gjbgl, m_player1->m_swingBurstParticles2);
		ADD_OPACITY(gjbgl, m_player1->m_dashSpritesContainer);
		ADD_OPACITY(gjbgl, m_player1->m_playerGroundParticles);
		ADD_OPACITY(gjbgl, m_player1->m_vehicleGroundParticles);
		ADD_OPACITY(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0));

		if (CCSprite* fuckYou = gjbgl->m_player2->m_dashSpritesContainer) {
			fuckYou->setCascadeOpacityEnabled(true);
		}
		ADD_MEM(gjbgl, m_player2);

		ADD_OPACITY(gjbgl, m_player2->m_waveTrail);
		ADD_OPACITY(gjbgl, m_player2->m_ghostTrail);
		ADD_OPACITY(gjbgl, m_player2->m_shipStreak);
		ADD_OPACITY(gjbgl, m_player2->m_dashParticles);
		ADD_OPACITY(gjbgl, m_player2->m_dashFireSprite);
		ADD_OPACITY(gjbgl, m_player2->m_landParticles0);
		ADD_OPACITY(gjbgl, m_player2->m_landParticles1);
		ADD_OPACITY(gjbgl, m_player2->m_ufoClickParticles);
		ADD_OPACITY(gjbgl, m_player2->m_trailingParticles);
		ADD_OPACITY(gjbgl, m_player2->m_shipClickParticles);
		ADD_OPACITY(gjbgl, m_player2->m_robotBurstParticles);
		ADD_OPACITY(gjbgl, m_player2->m_swingBurstParticles1);
		ADD_OPACITY(gjbgl, m_player2->m_swingBurstParticles2);
		ADD_OPACITY(gjbgl, m_player2->m_dashSpritesContainer);
		ADD_OPACITY(gjbgl, m_player2->m_playerGroundParticles);
		ADD_OPACITY(gjbgl, m_player2->m_vehicleGroundParticles);
		ADD_OPACITY(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0));
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
		if (CCSprite* fuckYou = gjbgl->m_player1->m_dashSpritesContainer) {
			fuckYou->setCascadeOpacityEnabled(false);
		}
		RES_MEM(gjbgl, m_player1);

		RES_OPACITY(gjbgl, m_player1->m_waveTrail);
		RES_OPACITY(gjbgl, m_player1->m_ghostTrail);
		RES_OPACITY(gjbgl, m_player1->m_shipStreak);
		RES_OPACITY(gjbgl, m_player1->m_dashParticles);
		RES_OPACITY(gjbgl, m_player1->m_dashFireSprite);
		RES_OPACITY(gjbgl, m_player1->m_landParticles0);
		RES_OPACITY(gjbgl, m_player1->m_landParticles1);
		RES_OPACITY(gjbgl, m_player1->m_ufoClickParticles);
		RES_OPACITY(gjbgl, m_player1->m_trailingParticles);
		RES_OPACITY(gjbgl, m_player1->m_shipClickParticles);
		RES_OPACITY(gjbgl, m_player1->m_robotBurstParticles);
		RES_OPACITY(gjbgl, m_player1->m_swingBurstParticles1);
		RES_OPACITY(gjbgl, m_player1->m_swingBurstParticles2);
		RES_OPACITY(gjbgl, m_player1->m_dashSpritesContainer);
		RES_OPACITY(gjbgl, m_player1->m_playerGroundParticles);
		RES_OPACITY(gjbgl, m_player1->m_vehicleGroundParticles);
		RES_OPACITY(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0));

		if (CCSprite* fuckYou = gjbgl->m_player2->m_dashSpritesContainer) {
			fuckYou->setCascadeOpacityEnabled(false);
		}
		RES_MEM(gjbgl, m_player2);

		RES_OPACITY(gjbgl, m_player2->m_waveTrail);
		RES_OPACITY(gjbgl, m_player2->m_ghostTrail);
		RES_OPACITY(gjbgl, m_player2->m_shipStreak);
		RES_OPACITY(gjbgl, m_player2->m_dashParticles);
		RES_OPACITY(gjbgl, m_player2->m_dashFireSprite);
		RES_OPACITY(gjbgl, m_player2->m_landParticles0);
		RES_OPACITY(gjbgl, m_player2->m_landParticles1);
		RES_OPACITY(gjbgl, m_player2->m_ufoClickParticles);
		RES_OPACITY(gjbgl, m_player2->m_trailingParticles);
		RES_OPACITY(gjbgl, m_player2->m_shipClickParticles);
		RES_OPACITY(gjbgl, m_player2->m_robotBurstParticles);
		RES_OPACITY(gjbgl, m_player2->m_swingBurstParticles1);
		RES_OPACITY(gjbgl, m_player2->m_swingBurstParticles2);
		RES_OPACITY(gjbgl, m_player2->m_dashSpritesContainer);
		RES_OPACITY(gjbgl, m_player2->m_playerGroundParticles);
		RES_OPACITY(gjbgl, m_player2->m_vehicleGroundParticles);
		RES_OPACITY(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0));
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