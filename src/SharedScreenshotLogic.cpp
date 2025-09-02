// ReSharper disable CppDFAUnreachableCode
// ReSharper disable CppDFAConstantConditions
// yeah that's right clion stop saying my code is unreachable >:(
#include "SharedScreenshotLogic.hpp"
#include "Screenshot.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

namespace eclipse::gui::cocos {
	class Popup final : public CCNode {
		// dummy declaration for eclipse support (when are y'all adding node IDs?) --raydeeux
	};
}

namespace eclipse::hacks::Level {
	class PauseCountdown final : public CCNode {
		// dummy declaration for eclipse support (when are y'all adding node IDs?) --raydeeux
	};
}

class HeartsContainer final : public CCNode {
	// dummy declaration because rainix is either allergic to CCNode::setID() or was never aware of it --raydeeux
};

std::string SharedScreenshotLogic::getFormattedDate() {
	auto nowTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto now = std::localtime(&nowTimeT);
	auto humanReadableMonth = SharedScreenshotLogic::monthNames[static_cast<unsigned int>(now->tm_mon + 1)];
	return fmt::format("{} {}, {}", humanReadableMonth, now->tm_mday, now->tm_year + 1900);
}

bool SharedScreenshotLogic::belongsToEitherPlayer(CCNode* nodeBeingChecked) {
	GJBaseGameLayer* gjbgl = GJBaseGameLayer::get();
	if (!gjbgl) return false;

	if (nodeBeingChecked == gjbgl->m_player1->m_waveTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_ghostTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_shipStreak) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_regularTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_dashParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_dashFireSprite) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_landParticles0) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_landParticles1) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_ufoClickParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_trailingParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_shipClickParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_robotBurstParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_swingBurstParticles1) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_swingBurstParticles2) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_dashSpritesContainer) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_playerGroundParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player1->m_vehicleGroundParticles) return true;

	if (nodeBeingChecked == gjbgl->m_player2->m_waveTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_ghostTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_shipStreak) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_regularTrail) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_dashParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_dashFireSprite) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_landParticles0) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_landParticles1) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_ufoClickParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_trailingParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_shipClickParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_robotBurstParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_swingBurstParticles1) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_swingBurstParticles2) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_dashSpritesContainer) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_playerGroundParticles) return true;
	if (nodeBeingChecked == gjbgl->m_player2->m_vehicleGroundParticles) return true;

	return false;
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
	// ADD_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
	ADD_SCALE(gjbgl->m_objectLayer, getChildByID("dankmeme.globed2/self-name"), unorderedMapStoringScales);
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
	// ADD_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
	ADD_SCALE(gjbgl->m_player2->getParent(), getChildByID("dankmeme.globed2/self-name-p2"), unorderedMapStoringScales);
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
	// RES_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
	RES_SCALE(gjbgl->m_objectLayer, getChildByID("dankmeme.globed2/self-name"), unorderedMapStoringScales);
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
	// RES_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
	RES_SCALE(gjbgl->m_player2->getParent(), getChildByID("dankmeme.globed2/self-name-p2"), unorderedMapStoringScales);
}

void SharedScreenshotLogic::hidePartsOfPlayer(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, PlayerObject* player) {
	if (!player) return;
	ADD_OTHER_PLAYER_MEM(player->m_waveTrail, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_ghostTrail, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_shipStreak, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_regularTrail, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_dashParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_dashFireSprite, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_landParticles0, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_landParticles1, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_ufoClickParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_trailingParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_shipClickParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_robotBurstParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_swingBurstParticles1, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_swingBurstParticles2, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_dashSpritesContainer, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_playerGroundParticles, unorderedMapStoringScales);
	ADD_OTHER_PLAYER_MEM(player->m_vehicleGroundParticles, unorderedMapStoringScales);
}

void SharedScreenshotLogic::unhidePartsOfPlayer(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, PlayerObject* player) {
	if (!player) return;
	RES_OTHER_PLAYER_MEM(player->m_waveTrail, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_ghostTrail, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_shipStreak, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_regularTrail, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_dashParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_dashFireSprite, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_landParticles0, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_landParticles1, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_ufoClickParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_trailingParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_shipClickParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_robotBurstParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_swingBurstParticles1, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_swingBurstParticles2, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_dashSpritesContainer, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_playerGroundParticles, unorderedMapStoringScales);
	RES_OTHER_PLAYER_MEM(player->m_vehicleGroundParticles, unorderedMapStoringScales);
}

void SharedScreenshotLogic::hideOtherPlayersIn(GJBaseGameLayer* gjbgl, CCNode* parentNode, std::unordered_map<CCNode*, bool>& otherPlayerVisibilities, std::unordered_map<CCNode*, float>& otherPlayerPointerScales) {
	// thank you so much MasterXI for being my impromptu test subject for this code snippet --raydeeux
	if (!parentNode || !gjbgl) return;
	for (CCNode* maybePossiblyPlayer : CCArrayExt<CCNode*>(parentNode->getChildren())) {
		if (!maybePossiblyPlayer) continue;
		if (maybePossiblyPlayer == gjbgl->m_player1 || maybePossiblyPlayer == gjbgl->m_player2) continue;

		if (SharedScreenshotLogic::belongsToEitherPlayer(maybePossiblyPlayer)) continue;
		const std::string& nodeID = maybePossiblyPlayer->getID();
		if (nodeID.empty() || utils::string::startsWith(nodeID, "dankmeme.globed2/remote-player-progress-")) continue;

		bool isGlobed = utils::string::startsWith(nodeID, "dankmeme.globed2/remote-player-");
		bool isChampion = utils::string::startsWith(nodeID, "ninxout.champions/player1-") || utils::string::startsWith(nodeID, "ninxout.champions/player2-");
		if (maybePossiblyPlayer && (isGlobed || isChampion)) {
			otherPlayerVisibilities[maybePossiblyPlayer] = maybePossiblyPlayer->isVisible();
			maybePossiblyPlayer->setVisible(false);
		}

		if (isChampion && maybePossiblyPlayer) {
			SharedScreenshotLogic::hidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(maybePossiblyPlayer));
		} else if (isGlobed && maybePossiblyPlayer) {
			if (CCNode* complexPlayerOne = maybePossiblyPlayer->getChildByID("dankmeme.globed2/visual-player1"); complexPlayerOne) {
				CCNode* playerObjectOne = complexPlayerOne->getChildByID("PlayerObject");
				if (playerObjectOne) SharedScreenshotLogic::hidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(playerObjectOne));
			}
			if (CCNode* complexPlayerTwo = maybePossiblyPlayer->getChildByID("dankmeme.globed2/visual-player2"); complexPlayerTwo) {
				CCNode* playerObjectTwo = complexPlayerTwo->getChildByID("PlayerObject");
				if (playerObjectTwo) SharedScreenshotLogic::hidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(playerObjectTwo));
			}
		}
	}
}

void SharedScreenshotLogic::unhideOtherPlayersIn(GJBaseGameLayer* gjbgl, CCNode* parentNode, std::unordered_map<CCNode*, bool>& otherPlayerVisibilities, std::unordered_map<CCNode*, float>& otherPlayerPointerScales) {
	// thank you so much MasterXI for being my impromptu test subject for this code snippet --raydeeux
	if (!parentNode || !gjbgl) return;
	for (CCNode* maybePossiblyPlayer : CCArrayExt<CCNode*>(parentNode->getChildren())) {
		if (!maybePossiblyPlayer) continue;
		if (maybePossiblyPlayer == gjbgl->m_player1 || maybePossiblyPlayer == gjbgl->m_player2) continue;

		if (SharedScreenshotLogic::belongsToEitherPlayer(maybePossiblyPlayer)) continue;
		const std::string& nodeID = maybePossiblyPlayer->getID();
		if (nodeID.empty() || utils::string::startsWith(nodeID, "dankmeme.globed2/remote-player-progress-")) continue;

		bool isGlobed = utils::string::startsWith(nodeID, "dankmeme.globed2/remote-player-");
		bool isChampion = utils::string::startsWith(nodeID, "ninxout.champions/player1-") || utils::string::startsWith(nodeID, "ninxout.champions/player2-");
		if (maybePossiblyPlayer && (isGlobed || isChampion)) {
			maybePossiblyPlayer->setVisible(otherPlayerVisibilities[maybePossiblyPlayer]);
		}

		if (isChampion && maybePossiblyPlayer) {
			SharedScreenshotLogic::unhidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(maybePossiblyPlayer));
		} else if (isGlobed && maybePossiblyPlayer) {
			if (CCNode* complexPlayerOne = maybePossiblyPlayer->getChildByID("dankmeme.globed2/visual-player1"); complexPlayerOne) {
				CCNode* playerObjectOne = complexPlayerOne->getChildByID("PlayerObject");
				if (playerObjectOne) SharedScreenshotLogic::unhidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(playerObjectOne));
			}
			if (CCNode* complexPlayerTwo = maybePossiblyPlayer->getChildByID("dankmeme.globed2/visual-player2"); complexPlayerTwo) {
				CCNode* playerObjectTwo = complexPlayerTwo->getChildByID("PlayerObject");
				if (playerObjectTwo) SharedScreenshotLogic::unhidePartsOfPlayer(otherPlayerPointerScales, static_cast<PlayerObject*>(playerObjectTwo));
			}
		}
	}
}

void SharedScreenshotLogic::screenshot(CCNode* node) {
	if (!node) return log::error("invalid node!");
	bool hasCustomNodesToHide = false;
	bool isCtrl = false;
	if (CCObject* hasNodeFilters = node->getUserObject("has-custom-nodes-to-hide"_spr); hasNodeFilters) {
		hasCustomNodesToHide = static_cast<CCBool*>(hasNodeFilters)->getValue();
	}
	if (CCObject* isControlFromPopup = node->getUserObject("is-plain-ss-from-popup"_spr); isControlFromPopup) {
		isCtrl = static_cast<CCBool*>(isControlFromPopup)->getValue();
	}
	std::string modIDAskingForScreenshot;
	std::string pauseMenuTypeForSetting;
	if (CCObject* modID = node->getUserObject("mod-asking-for-screenshot"_spr); modID) {
		auto cString = static_cast<std::string>(static_cast<CCString*>(modID)->getCString());
		if (!cString.empty()) modIDAskingForScreenshot = fmt::format("{} - ", cString);
	}
	if (CCObject* pauseMenuType = node->getUserObject("pause-menu-type"_spr); pauseMenuType) {
		auto cString = static_cast<std::string>(static_cast<CCString*>(pauseMenuType)->getCString());
		if (!cString.empty()) pauseMenuTypeForSetting = cString;
	}
	bool screenshotterIsSelf = modIDAskingForScreenshot.empty() || modIDAskingForScreenshot == Mod::get()->getID();

	// event filter from main.cpp will have already hidden the nodes by this point
	std::unordered_map<const char*, bool> uiNodes = {};
	std::unordered_map<CCNode*, float> playerPointerScales = {};
	std::unordered_map<std::string, GLubyte> checkpointOpacities = {};
	std::unordered_map<CCNode*, bool> otherPlayerVisibilities = {};
	std::unordered_map<CCNode*, float> otherPlayerPointerScales = {};
	std::unordered_map<CCNode*, bool> otherPlayerUIVisibilities = {};

	bool hideUI = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-player");
	bool hideAL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-attempts");
	bool hideCK = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-checkpoints");
	bool hideOT = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-multiplayers");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);
	LevelEditorLayer* lel = typeinfo_cast<LevelEditorLayer*>(node);
	bool originalEclipsePopupVisibility = false;
	bool originalEclipseCountdownVisibility = false;
	bool originalHeartsContainerVisibility = false;

	if (CCNode* eclipsePopup = CCScene::get()->getChildByType<eclipse::gui::cocos::Popup>(0); eclipsePopup) {
		originalEclipsePopupVisibility = eclipsePopup->isVisible();
		eclipsePopup->setVisible(false);
	}
	if (isCtrl) ADD_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (pl) ADD_NODE(pl, sawblade.dim_mode/dimOverlay);
	if (hideUI && pl) {
		ADD_NODE(pl, UILayer);
		ADD_NODE(pl, debug-text);
		ADD_NODE(pl, testmode-label);
		ADD_NODE(pl, percentage-label);
		ADD_NODE(pl, mat.run-info/RunInfoWidget);
		ADD_NODE(pl, cheeseworks.speedruntimer/timer);
		ADD_NODE(pl, progress-bar);
		ADD_NODE(pl, sawblade.dim_mode/opacityLabel);
		ADD_NODE(pl, zilko.xdbot/state-label);
		ADD_NODE(pl, zilko.xdbot/frame-label);
		ADD_NODE(pl, zilko.xdbot/recording-audio-label);
		ADD_NODE(pl, zilko.xdbot/button-menu);
		ADD_NODE(pl, dankmeme.globed2/game-overlay);
		ADD_NODE(pl, tobyadd.gdh/labels_top_left);
		ADD_NODE(pl, tobyadd.gdh/labels_top_right);
		ADD_NODE(pl, tobyadd.gdh/labels_bottom_left);
		ADD_NODE(pl, tobyadd.gdh/labels_bottom_right);
		ADD_NODE(pl, tobyadd.gdh/labels_bottom);
		ADD_NODE(pl, tobyadd.gdh/labels_top);

		if (CCNode* eclipseCountdown = pl->getChildByType<eclipse::hacks::Level::PauseCountdown>(0); eclipseCountdown) {
			originalEclipseCountdownVisibility = eclipseCountdown->isVisible();
			eclipseCountdown->setVisible(false);
		}
		if (CCNode* heartsContainer = pl->getChildByType<HeartsContainer>(0); heartsContainer) {
			originalHeartsContainerVisibility = heartsContainer->isVisible();
			heartsContainer->setVisible(false);
		}

		if (pl->m_level && pl->m_level->isPlatformer()) {
			for (CCNode* potentiallyPlayer : CCArrayExt<CCNode*>(pl->getChildren())) {
				if (!potentiallyPlayer) continue;
				const std::string& potentialNodeID = potentiallyPlayer->getID();
				if (potentialNodeID.empty() || !utils::string::startsWith(potentialNodeID, "dankmeme.globed2/remote-player-progress-")) continue;
				otherPlayerUIVisibilities[potentiallyPlayer] = potentiallyPlayer->isVisible();
				potentiallyPlayer->setVisible(false);
			}
		}
	}
	if (hideAL && pl && pl->m_attemptLabel) {
		ADD_MEM(pl, m_attemptLabel);
		if (pl->m_attemptLabel->getParent()) ADD_NODE(pl->m_attemptLabel->getParent(), raydeeux.attemptlabeltweaks/custom-attempt-label);
	}
	if (hideCK && pl && pl->m_checkpointArray && pl->m_checkpointArray->count() > 0) {
		int i = 0;
		for (CheckpointObject* ckpt : CCArrayExt<CheckpointObject*>(pl->m_checkpointArray)) {
			if (ckpt && ckpt->m_physicalCheckpointObject) {
				checkpointOpacities[fmt::format("checkpoint_{}", i).c_str()] = ckpt->m_physicalCheckpointObject->getOpacity();
				ckpt->m_physicalCheckpointObject->setOpacity(0);
			}
			i++;
		}
	}
	if (hideUI && lel) {
		ADD_NODE(lel, UILayer);
		ADD_NODE(lel, EditorUI);
		ADD_NODE(lel, EditorPauseLayer);
		ADD_NODE(lel, dankmeme.globed2/game-overlay);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		ADD_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::hideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		ADD_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}
	if (hideOT && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		if (gjbgl->m_objectLayer) {
			SharedScreenshotLogic::hideOtherPlayersIn(gjbgl, gjbgl->m_objectLayer, otherPlayerVisibilities, otherPlayerPointerScales);
		}
	}

	CCSize selectedSize = CCSize(Manager::get()->width, Manager::get()->height);
	if (!screenshotterIsSelf) selectedSize = CCDirector::get()->getWinSizeInPixels();
	Screenshot ss = Screenshot(selectedSize, node);

	if (CCNode* eclipsePopup = CCScene::get()->getChildByType<eclipse::gui::cocos::Popup>(0); eclipsePopup) {
		eclipsePopup->setVisible(originalEclipsePopupVisibility);
	}
	if (isCtrl) RES_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (pl) RES_NODE(pl, sawblade.dim_mode/dimOverlay);
	if (hideUI && pl) {
		RES_NODE(pl, UILayer);
		RES_NODE(pl, debug-text);
		RES_NODE(pl, testmode-label);
		RES_NODE(pl, percentage-label);
		RES_NODE(pl, mat.run-info/RunInfoWidget);
		RES_NODE(pl, cheeseworks.speedruntimer/timer);
		RES_NODE(pl, progress-bar);
		RES_NODE(pl, sawblade.dim_mode/opacityLabel);
		RES_NODE(pl, zilko.xdbot/state-label);
		RES_NODE(pl, zilko.xdbot/frame-label);
		RES_NODE(pl, zilko.xdbot/recording-audio-label);
		RES_NODE(pl, zilko.xdbot/button-menu);
		RES_NODE(pl, dankmeme.globed2/game-overlay);
		RES_NODE(pl, tobyadd.gdh/labels_top_left);
		RES_NODE(pl, tobyadd.gdh/labels_top_right);
		RES_NODE(pl, tobyadd.gdh/labels_bottom_left);
		RES_NODE(pl, tobyadd.gdh/labels_bottom_right);
		RES_NODE(pl, tobyadd.gdh/labels_bottom);
		RES_NODE(pl, tobyadd.gdh/labels_top);

		if (CCNode* eclipseCountdown = pl->getChildByType<eclipse::hacks::Level::PauseCountdown>(0); eclipseCountdown) {
			eclipseCountdown->setVisible(originalEclipseCountdownVisibility);
		}
		if (CCNode* heartsContainer = pl->getChildByType<HeartsContainer>(0); heartsContainer) {
			heartsContainer->setVisible(originalHeartsContainerVisibility);
		}

		if (pl->m_level && pl->m_level->isPlatformer() && !otherPlayerUIVisibilities.empty()) {
			for (auto [potentiallyPlayer, formerVisibility] : otherPlayerUIVisibilities) {
				if (!potentiallyPlayer) continue;
				potentiallyPlayer->setVisible(formerVisibility);
			}
		}
	}
	if (hideAL && pl && pl->m_attemptLabel) {
		RES_MEM(pl, m_attemptLabel);
		if (pl->m_attemptLabel->getParent()) RES_NODE(pl->m_attemptLabel->getParent(), raydeeux.attemptlabeltweaks/custom-attempt-label);
	}
	if (hideCK && pl && pl->m_checkpointArray && pl->m_checkpointArray->count() > 0) {
		int i = 0;
		for (CheckpointObject* ckpt : CCArrayExt<CheckpointObject*>(pl->m_checkpointArray)) {
			if (ckpt && ckpt->m_physicalCheckpointObject) {
				ckpt->m_physicalCheckpointObject->setOpacity(checkpointOpacities[fmt::format("checkpoint_{}", i).c_str()]);
			}
			i++;
		}
	}
	if (hideUI && lel) {
		RES_NODE(lel, UILayer);
		RES_NODE(lel, EditorUI);
		RES_NODE(lel, EditorPauseLayer);
		RES_NODE(lel, dankmeme.globed2/game-overlay);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		RES_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		RES_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}
	if (hideOT && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		if (gjbgl->m_objectLayer) {
			SharedScreenshotLogic::unhideOtherPlayersIn(gjbgl, gjbgl->m_objectLayer, otherPlayerVisibilities, otherPlayerPointerScales);
		}
	}

	bool jpeg = false;
	#ifdef GEODE_IS_DESKTOP
	jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
	#endif
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

	if (pl && pl->getChildByID("EndLevelLayer")) {
		if (pl->getChildByID("EndLevelLayer")->getScale() == 0.f) extension = " (without endscreen)" + extension; // prevent filename collision
	}

	std::string filename = geode::utils::string::pathToString(folder / (numToString(index) + extension));
	bool shouldPlaySFX = screenshotterIsSelf && Mod::get()->getSettingValue<bool>("play-sfx");
	ss.intoFile(filename, shouldPlaySFX, jpeg);

	#ifdef GEODE_IS_DESKTOP
	if (Mod::get()->getSettingValue<bool>("copy-clipboard") && screenshotterIsSelf) {
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
	#endif
}

void SharedScreenshotLogic::screenshotLevelOrScene() {
	CCNode* nodeToScreenshot = CCScene::get();
	PlayLayer* pl = PlayLayer::get();
	LevelEditorLayer* lel = LevelEditorLayer::get();
	if (pl && pl->getParent() == CCScene::get()) {
		SharedScreenshotLogic::screenshot(pl); // regular PlayLayer w/o "hidden" EndLevelLayer
		if (CCNode* ell = pl->getChildByID("EndLevelLayer"); ell) {
			float ellOriginalScale = ell->getScale();
			ell->setScale(0.f);
			SharedScreenshotLogic::screenshot(pl); // regular PlayLayer w/"hidden" EndLevelLayer
			ell->setScale(ellOriginalScale);
			SharedScreenshotLogic::screenshot(CCScene::get()); // baseScene
			return;
		}
		if (CCScene::get()->getChildByID("PauseLayer")) {
			CCScene* baseScene = CCScene::get();
			baseScene->setUserObject("pause-menu-type"_spr, CCString::create("PauseLayer"));
			SharedScreenshotLogic::screenshot(baseScene);
			baseScene->setUserObject("pause-menu-type"_spr, CCString::create(""));
			return;
		}
		return;
	}
	if (lel && lel->getParent() == CCScene::get()) {
		SharedScreenshotLogic::screenshot(lel);
		if (lel->getChildByID("EditorPauseLayer")) {
			CCScene* baseScene = CCScene::get();
			baseScene->setUserObject("pause-menu-type"_spr, CCString::create("EditorPauseLayer"));
			SharedScreenshotLogic::screenshot(baseScene);
			baseScene->setUserObject("pause-menu-type"_spr, CCString::create(""));
		}
		return;
	}
	if (nodeToScreenshot) SharedScreenshotLogic::screenshot(nodeToScreenshot);
}