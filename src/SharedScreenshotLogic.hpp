#pragma once

using namespace geode::prelude;

#define ADD_NODE(parent, val) \
	if (auto node = parent->getChildByID(#val)) { \
		uiNodes[#val] = node->isVisible(); \
		node->setVisible(false); \
	}

#define ADD_MEM(parent, val) \
	if (auto node = parent->val) { \
		uiNodes[#val] = node->isVisible(); \
		node->setVisible(false); \
	}

#define ADD_SCALE(parent, val, unorderedMap) \
	if (auto node = parent->val) { \
		unorderedMap[node] = node->getScale(); \
		node->setScale(0.f); \
	}

#define RES_NODE(parent, val) if (auto node = parent->getChildByID(#val)) parent->getChildByID(#val)->setVisible(uiNodes[#val]);

#define RES_MEM(parent, val) if (auto node = parent->val) node->setVisible(uiNodes[#val]);

#define RES_SCALE(parent, val, unorderedMap) if (auto node = parent->val) node->setScale(unorderedMap[node]);

namespace SharedScreenshotLogic {

	std::string getFormattedDate();
	void hideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void hideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void unhideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void unhideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl);
	void screenshot(CCNode* node);

	constexpr std::array monthNames = {
		"Unknown",
		"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December",
		"Unknown"
	};

}