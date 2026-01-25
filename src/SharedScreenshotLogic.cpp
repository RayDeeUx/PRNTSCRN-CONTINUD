// ReSharper disable CppTooWideScope
// ReSharper disable CppDFAUnusedValue
// ReSharper disable CppDFAUnreachableCode
// ReSharper disable CppDFAConstantConditions
// ReSharper disable CppLocalVariableMayBeConst
// yeah that's right clion stop shitting on my code >:(
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

namespace status {
	// dummy declarations because WHY DOESNT ABSOLUTE PUT HIS LABELS INSIDE UILAYER LIKE A NORMAL PERSON --raydeeux
	class Manager final : public CCNode {
		// AND WHY IS THIS SHIT AT ANCHORPOINT 0, 0 AND POSITION 0, 0 ????? --raydeeux
	};
	class Label final : public CCNode {
		// WAS CCLABELMBFONT NOT ENOUGH BRO ????? --raydeeux
	};
}

class NoclipTint final : public CCNode {
	// dummy declarations because megahack --raydeeux
};

class LevelProgressionLives final : public CCNode {
	// dummy declaration because dogotrigger never gave an _spr suffix to the node and i do not want to tempt fate --raydeeux
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

void SharedScreenshotLogic::findSorkosShaderNodesAndScaleThemToSixSeven(CCNode* node, std::unordered_map<CCNode*, CCSize>& sorkosShaderNodesAndTheirContentSizes) {
	if (!node) return;
	const float ratio = CCDirector::get()->getWinSizeInPixels().width / CCScene::get()->getContentSize().width;
	for (CCNode* child : node->getChildrenExt<CCNode*>()) {
		if (!child) continue;
		if (geode::cocos::getObjectName(child) == std::string_view("ShaderNode")) {
			CCSize origSize = child->getContentSize();
			sorkosShaderNodesAndTheirContentSizes[child] = origSize;
			child->setContentSize(node->getContentSize() * ratio);
			child->draw(); // necessary so the new scale is reflected properly
		}
		if (child->getChildrenCount() < 1) continue;
		SharedScreenshotLogic::findSorkosShaderNodesAndScaleThemToSixSeven(child, sorkosShaderNodesAndTheirContentSizes);
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
	std::unordered_map<CCNode*, bool> otherPlayerVisibilities = {};
	std::unordered_map<CCNode*, float> otherPlayerPointerScales = {};
	std::unordered_map<CCNode*, bool> otherPlayerUIVisibilities = {};
	std::unordered_map<GameObject*, GLubyte> checkpointOpacities = {};
	std::unordered_map<CCNode*, CCSize> sorkosShaderNodesAndTheirContentSizes = {};

	std::unordered_map<const char*, bool> gameVariablesAndTheirFormerValues = {};

	bool hideUI = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-player");
	bool hideAL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-attempts");
	bool hideCK = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-checkpoints");
	bool hideOT = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-multiplayers");
	bool hideTN = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-noclip-tints");

	bool hideDD = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-debug-draw");
	bool hideDG = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-draw-grid-layer");
	bool hidePT = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-playtest-path-clicks");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);
	LevelEditorLayer* lel = typeinfo_cast<LevelEditorLayer*>(node);
	bool originalEclipsePopupVisibility = false;
	bool originalEclipseCountdownVisibility = false;
	bool originalHeartsContainerVisibility = false;
	bool originalDifficultyProgressionVisibility = false;
	bool originalGDHNoclipTintVisibility = false;
	bool originalPrismMenuTintVisibility = false;

	// i dont trust absolute and i dont have the tools to load megahack on a macbook and i need this update out quick, leave me alone --raydeeux
	bool originalMegaHackLabelContainerVisibility = false;
	CCPoint originalMegaHackLabelContainerPosition = {};
	float originalMegaHackLabelContainerScale = 0;
	bool originalMegaHackLabelNoclipTintVisibility = false;
	CCPoint originalMegaHackLabelNoclipTintPosition = {};
	float originalMegaHackLabelNoclipTintScale = 0;

	bool robtopIsAFuckingDumbass = false; // YOU HAVE NO FUCKING IDEA HOW MUCH PAIN AND SUFFERING I HAD TO GO THROUGH TO JUSTIFY ADDING THIS FUCKING LOCAL VARIABLE. WHY THE ***__FUCK__*** DID ROBTOP DECIDED TO STORE EDITOR OBJECT HITBOXES AND THE EDITOR PLAYTEST PATH AND THE EDITOR PLAYTEST CLICK INDICATORS IN THE SAME FUCKING CCDRAWNODE WHY THE FUCK BRO HOW THE FUCK DOES THAT SHIT MAKE ANY FUCKING SENSE I WILL FUCKING THROW MYSELF INTO NIAGRA FALLS IF I HAVE TO PUT UP WITH EQUALLY ASININE BULLSHIT AS THIS I SWEAR TO FUCKING GOD ROBTOP

	SharedScreenshotLogic::findSorkosShaderNodesAndScaleThemToSixSeven(node, sorkosShaderNodesAndTheirContentSizes);

	if (CCNode* eclipsePopup = CCScene::get()->getChildByType<eclipse::gui::cocos::Popup>(0); eclipsePopup) {
		originalEclipsePopupVisibility = eclipsePopup->isVisible();
		eclipsePopup->setVisible(false);
	}
	if (isCtrl) ADD_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (pl) ADD_NODE(pl, sawblade.dim_mode/dimOverlay);
	if (hideUI && pl) {
		ADD_MEM(pl, m_uiLayer);
		ADD_MEM(pl, m_infoLabel);
		ADD_MEM(pl, m_percentageLabel);
		ADD_MEM(pl, m_progressBar);
		ADD_MEM(pl, m_progressFill);
		ADD_MEM(pl, m_statusLabel);

		ADD_NODE(pl, mat.run-info/RunInfoWidget);
		ADD_NODE(pl, cheeseworks.speedruntimer/timer);
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
		if (CCNode* difficultyProgression = pl->getChildByType<LevelProgressionLives>(0); difficultyProgression) {
			originalDifficultyProgressionVisibility = difficultyProgression->isVisible();
			difficultyProgression->setVisible(false);
		}

		// i dont trust absolute and i dont have the tools to load megahack on a macbook and i need this update out quick, leave me alone --raydeeux
		if (CCNode* megaHackLabelContainer = pl->getChildByType<status::Manager>(0); megaHackLabelContainer) {
			originalMegaHackLabelContainerPosition = megaHackLabelContainer->getPosition();
			originalMegaHackLabelContainerScale = megaHackLabelContainer->getScale();
			originalMegaHackLabelContainerVisibility = megaHackLabelContainer->isVisible();
			megaHackLabelContainer->setPosition({-999999, -999999});
			megaHackLabelContainer->setScale(0);
			megaHackLabelContainer->setVisible(false);
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
		for (CheckpointObject* ckpt : CCArrayExt<CheckpointObject*>(pl->m_checkpointArray)) {
			if (ckpt && ckpt->m_physicalCheckpointObject) {
				checkpointOpacities[ckpt->m_physicalCheckpointObject] = ckpt->m_physicalCheckpointObject->getOpacity();
				ckpt->m_physicalCheckpointObject->setOpacity(0);
			}
		}
	}
	if (hideTN && pl) {
		if (pl->m_uiLayer) ADD_NODE(pl->m_uiLayer, eclipse.eclipse-menu/nocliptint);

		// funny story: i was supposed to add this far earlier, but forgot. oops! --raydeeux
		ADD_NODE(pl, thesillydoggo.qolmod/noclip-tint-popup); // QOLMod v1
		ADD_NODE(pl, thesillydoggo.qolmod/noclip-tint-overlay); // QOLMod v2

		// toby toby toby why do you make me do this --raydeeux
		if (CCNode* tobyAddGDHNoclipTint = geode::cocos::getChildBySpriteName(pl, "game_bg_13_001.png"); tobyAddGDHNoclipTint && tobyAddGDHNoclipTint->getZOrder() == 999) {
			originalGDHNoclipTintVisibility = tobyAddGDHNoclipTint->isVisible();
			tobyAddGDHNoclipTint->setVisible(false);
		}

		// oh my fucking god are eclipse menu and qolmod the only mod menus to do this shit correctly holy fuck --raydeeux
		if (CCNode* prismNode = pl->getChildByTag(10420); prismNode && prismNode->getZOrder() == 1) {
			// that conditional was so goddamn brittle it's not even funny at this rate --raydeeux
			if (CCNode* prismMenuNoclipTint = geode::cocos::getChildBySpriteName(prismNode, "square02b_001.png"); prismMenuNoclipTint && prismMenuNoclipTint->getTag() == 10420 && prismMenuNoclipTint->getZOrder() == 999) {
				originalPrismMenuTintVisibility = prismMenuNoclipTint->isVisible();
				prismMenuNoclipTint->setVisible(false);
			}
		}

		// god motherfucking damn it you've got to be fucking kidding me right --raydeeux
		if (CCNode* megahackNoclipTint = pl->getChildByType<NoclipTint>(0); megahackNoclipTint) {
			originalMegaHackLabelNoclipTintVisibility = megahackNoclipTint->isVisible();
			originalMegaHackLabelNoclipTintPosition = megahackNoclipTint->getPosition();
			originalMegaHackLabelNoclipTintScale = megahackNoclipTint->getScale();
			megahackNoclipTint->setVisible(false);
			megahackNoclipTint->setPosition({-99999, -99999});
			megahackNoclipTint->setScale(0.f);
		}
	}
	if (hideUI && lel) {
		ADD_NODE(lel, UILayer);
		ADD_NODE(lel, EditorUI);
		ADD_NODE(lel, EditorPauseLayer);
		ADD_NODE(lel, dankmeme.globed2/game-overlay);
	}
	if (hideDD && lel) {
		if (lel->m_debugDrawNode) ADD_NODE(lel->m_debugDrawNode->getParent(), eclipse.eclipse-menu/hitboxes);
		ADD_GAMEVARIABLE("0045", false);
		robtopIsAFuckingDumbass = lel->m_isDebugDrawEnabled;
		lel->m_isDebugDrawEnabled = false;
		lel->updateOptions();
		lel->m_isDebugDrawEnabled = false;
		lel->updateDebugDraw();
		lel->m_isDebugDrawEnabled = false;
	}
	if (hideDG && lel) {
		ADD_MEM(lel, m_drawGridLayer);
	}
	if (hidePT && lel) {
		ADD_GAMEVARIABLE("0152", true);
		ADD_GAMEVARIABLE("0149", false);
		lel->updateOptions();
		lel->updateDebugDraw();
		if (lel->m_debugDrawNode) {
			ADD_NODE(lel->m_debugDrawNode->getParent(), nwo5.better_editor_trail/better-trail-trail);
			ADD_NODE(lel->m_debugDrawNode->getParent(), nwo5.better_editor_trail/better-trail-indicators);
		}
		ADD_MEM(lel, m_playtestDeathSprite);
	}
	if (hideTN && lel) {
		// funny story: i was supposed to add this far earlier, but forgot. oops! --raydeeux
		ADD_NODE(lel, thesillydoggo.qolmod/noclip-tint-popup); // QOLMod v1
		ADD_NODE(lel, thesillydoggo.qolmod/noclip-tint-overlay); // QOLMod v2
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
			ADD_NODE(gjbgl->m_objectLayer, dankmeme.globed2/player-node)
			SharedScreenshotLogic::hideOtherPlayersIn(gjbgl, gjbgl->m_objectLayer, otherPlayerVisibilities, otherPlayerPointerScales);
		}
	}

	CCSize selectedSize = CCSize(Manager::get()->width, Manager::get()->height);
	if (!screenshotterIsSelf) selectedSize = CCDirector::get()->getWinSizeInPixels();
	Screenshot ss = Screenshot(selectedSize, node);

	for (auto& [node, formerContentSize] : sorkosShaderNodesAndTheirContentSizes) {
		if (!node) continue;
		node->setContentSize(formerContentSize);
		node->draw();
	}

	if (CCNode* eclipsePopup = CCScene::get()->getChildByType<eclipse::gui::cocos::Popup>(0); eclipsePopup) {
		eclipsePopup->setVisible(originalEclipsePopupVisibility);
	}
	if (isCtrl) RES_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (pl) RES_NODE(pl, sawblade.dim_mode/dimOverlay);
	if (hideUI && pl) {
		RES_MEM(pl, m_uiLayer);
		RES_MEM(pl, m_infoLabel);
		RES_MEM(pl, m_percentageLabel);
		RES_MEM(pl, m_progressBar);
		RES_MEM(pl, m_progressFill);
		RES_MEM(pl, m_statusLabel);

		RES_NODE(pl, mat.run-info/RunInfoWidget);
		RES_NODE(pl, cheeseworks.speedruntimer/timer);
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
		if (CCNode* difficultyProgression = pl->getChildByType<LevelProgressionLives>(0); difficultyProgression) {
			difficultyProgression->setVisible(originalDifficultyProgressionVisibility);
		}

		// i dont trust absolute and i dont have the tools to load megahack on a macbook and i need this update out quick, leave me alone --raydeeux
		if (CCNode* megaHackLabelContainer = pl->getChildByType<status::Manager>(0); megaHackLabelContainer) {
			megaHackLabelContainer->setPosition(originalMegaHackLabelContainerPosition);
			megaHackLabelContainer->setScale(originalMegaHackLabelContainerScale);
			megaHackLabelContainer->setVisible(originalMegaHackLabelContainerVisibility);
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
		for (auto [ckptObject, opacity] : checkpointOpacities) {
			if (ckptObject) ckptObject->setOpacity(opacity);
		}
	}
	if (hideTN && pl) {
		if (pl->m_uiLayer) RES_NODE(pl->m_uiLayer, eclipse.eclipse-menu/nocliptint);

		// funny story: i was supposed to add this far earlier, but forgot. oops! --raydeeux
		RES_NODE(pl, thesillydoggo.qolmod/noclip-tint-popup); // QOLMod v1
		RES_NODE(pl, thesillydoggo.qolmod/noclip-tint-overlay); // QOLMod v2

		// toby toby toby why do you make me do this --raydeeux
		if (CCNode* tobyAddGDHNoclipTint = geode::cocos::getChildBySpriteName(pl, "game_bg_13_001.png"); tobyAddGDHNoclipTint && tobyAddGDHNoclipTint->getZOrder() == 999) {
			tobyAddGDHNoclipTint->setVisible(originalGDHNoclipTintVisibility);
		}

		// oh my fucking god are eclipse menu and qolmod the only mod menus to do this shit correctly holy fuck --raydeeux
		if (CCNode* prismNode = pl->getChildByTag(10420); prismNode && prismNode->getZOrder() == 1) {
			// that conditional was so goddamn brittle it's not even funny at this rate --raydeeux
			if (CCNode* prismMenuNoclipTint = geode::cocos::getChildBySpriteName(prismNode, "square02b_001.png"); prismMenuNoclipTint && prismMenuNoclipTint->getTag() == 10420 && prismMenuNoclipTint->getZOrder() == 999) {
				prismMenuNoclipTint->setVisible(originalPrismMenuTintVisibility);
			}
		}

		// god motherfucking damn it you've got to be fucking kidding me right --raydeeux
		if (CCNode* megahackNoclipTint = pl->getChildByType<NoclipTint>(0); megahackNoclipTint) {
			megahackNoclipTint->setVisible(originalMegaHackLabelNoclipTintVisibility);
			megahackNoclipTint->setPosition(originalMegaHackLabelNoclipTintPosition);
			megahackNoclipTint->setScale(originalMegaHackLabelNoclipTintScale);
		}
	}
	if (hideUI && lel) {
		RES_NODE(lel, UILayer);
		RES_NODE(lel, EditorUI);
		RES_NODE(lel, EditorPauseLayer);
		RES_NODE(lel, dankmeme.globed2/game-overlay);
	}
	if (hideDD && lel) {
		if (lel->m_debugDrawNode) RES_NODE(lel->m_debugDrawNode->getParent(), eclipse.eclipse-menu/hitboxes);
		RES_GAMEVARIABLE("0045");
		lel->m_isDebugDrawEnabled = robtopIsAFuckingDumbass;
		lel->updateOptions();
		lel->m_isDebugDrawEnabled = robtopIsAFuckingDumbass;
		lel->updateDebugDraw();
		lel->m_isDebugDrawEnabled = robtopIsAFuckingDumbass;
	}
	if (hideDG && lel) {
		RES_MEM(lel, m_drawGridLayer);
	}
	if (hidePT && lel) {
		RES_GAMEVARIABLE("0152");
		RES_GAMEVARIABLE("0149");
		lel->updateOptions();
		lel->updateDebugDraw();
		if (lel->m_debugDrawNode) {
			RES_NODE(lel->m_debugDrawNode->getParent(), nwo5.better_editor_trail/better-trail-trail);
			RES_NODE(lel->m_debugDrawNode->getParent(), nwo5.better_editor_trail/better-trail-indicators);
		}
		RES_MEM(lel, m_playtestDeathSprite);
	}
	if (hideTN && lel) {
		// funny story: i was supposed to add this far earlier, but forgot. oops! --raydeeux
		RES_NODE(lel, thesillydoggo.qolmod/noclip-tint-popup); // QOLMod v1
		RES_NODE(lel, thesillydoggo.qolmod/noclip-tint-overlay); // QOLMod v2
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
			RES_NODE(gjbgl->m_objectLayer, dankmeme.globed2/player-node)
			SharedScreenshotLogic::unhideOtherPlayersIn(gjbgl, gjbgl->m_objectLayer, otherPlayerVisibilities, otherPlayerPointerScales);
		}
	}

	// log::info("modIDAskingForScreenshot: {}", modIDAskingForScreenshot);
	// log::info("pauseMenuTypeForSetting: {}", pauseMenuTypeForSetting);

	bool jpeg = false;
	#ifdef GEODE_IS_DESKTOP
	jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia") && screenshotterIsSelf;
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
	// log::info("filename: {}", filename);
	ss.intoFile(filename, shouldPlaySFX, jpeg);

	if (node->getUserObject("has-custom-nodes-to-hide"_spr)) node->setUserObject("has-custom-nodes-to-hide"_spr, nullptr);
	if (node->getUserObject("mod-asking-for-screenshot"_spr)) node->setUserObject("mod-asking-for-screenshot"_spr, nullptr);
	if (node->getUserObject("is-plain-ss-from-popup"_spr)) node->setUserObject("is-plain-ss-from-popup"_spr, nullptr);
	if (node->getUserObject("pause-menu-type"_spr)) node->setUserObject("pause-menu-type"_spr, nullptr);

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