#pragma once

using namespace geode::prelude;

#define ADD_NODE(parent, val) \
	if (parent) { \
		if (auto node = parent->getChildByID(#val); node) { \
			uiNodes[#val] = node->isVisible(); \
			node->setVisible(false); \
		}\
	}

#define ADD_MEM(parent, val) \
	if (parent) { \
		if (auto node = parent->val; node) { \
			uiNodes[#val] = node->isVisible(); \
			node->setVisible(false); \
		}\
	}

#define ADD_SCALE(parent, val, unorderedMap) \
	if (parent) { \
		if (auto node = parent->val; node) { \
			unorderedMap[node] = node->getScale(); \
			node->setScale(0.f); \
		}\
	}

#define ADD_OTHER_PLAYER_MEM(cocosNodePointer, unorderedMap) \
	if (cocosNodePointer) { \
		unorderedMap[cocosNodePointer] = cocosNodePointer->getScale(); \
		cocosNodePointer->setScale(0.f); \
	}

#define RES_NODE(parent, val) \
	if (parent) { \
		if (auto node = parent->getChildByID(#val); node) { \
			parent->getChildByID(#val)->setVisible(uiNodes[#val]); \
		} \
	}

#define RES_MEM(parent, val) \
	if (parent) { \
		if (auto node = parent->val; node) { \
			node->setVisible(uiNodes[#val]); \
		} \
	}

#define RES_SCALE(parent, val, unorderedMap) \
	if (parent) { \
		if (auto node = parent->val; node) { \
			node->setScale(unorderedMap[node]); \
		} \
	}

#define RES_OTHER_PLAYER_MEM(cocosNodePointer, unorderedMap) \
	if (cocosNodePointer) { \
		cocosNodePointer->setScale(unorderedMap[cocosNodePointer]); \
	}

#define ADD_GAMEVARIABLE(gameVariableKey, desiredValue) \
	gameVariablesAndTheirFormerValues[gameVariableKey] = GameManager::sharedState()->getGameVariable(gameVariableKey); \
	GameManager::sharedState()->setGameVariable(gameVariableKey, desiredValue); \

#define RES_GAMEVARIABLE(gameVariableKey) \
	GameManager::sharedState()->setGameVariable(gameVariableKey, gameVariablesAndTheirFormerValues[gameVariableKey]); \

namespace SharedScreenshotLogic {

	std::string getFormattedDate();
	bool belongsToEitherPlayer(CCNode* nodeBeingChecked);
	void hideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void hideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void unhideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void unhideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void hidePartsOfPlayer(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, PlayerObject* player);
	void unhidePartsOfPlayer(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, PlayerObject* player);
	void hideOtherPlayersIn(GJBaseGameLayer* gjbgl, CCNode* parentNode, std::unordered_map<CCNode*, bool>& otherPlayerVisibilities, std::unordered_map<CCNode*, float>& otherPlayerPointerScales);
	void unhideOtherPlayersIn(GJBaseGameLayer* gjbgl, CCNode* parentNode, std::unordered_map<CCNode*, bool>& otherPlayerVisibilities, std::unordered_map<CCNode*, float>& otherPlayerPointerScales);
	void findSorkosShaderNodesAndScaleThemToSixSeven(CCNode* node, std::unordered_map<CCNode*, float>& sorkosShaderNodesAndTheirScales);
	void screenshot(CCNode* node);
	void screenshotLevelOrScene();

	constexpr std::array monthNames = {
		"Unknown",
		"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December",
		"Unknown"
	};

}