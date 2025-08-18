#pragma once

using namespace geode::prelude;

#define ADD_NODE(parent, val) \
	if (auto node = parent->getChildByID(#val)) { \
		uiNodes[#val] = node->isVisible(); \
		node->setVisible(false); \
	}

#define ADD_MEM(parent, val) \
	uiNodes[#val] = parent->val->isVisible(); \
	parent->val->setVisible(false);

#define RES_NODE(parent, val) if (auto node = parent->getChildByID(#val)) parent->getChildByID(#val)->setVisible(uiNodes[#val]);

#define RES_MEM(parent, val) parent->val->setVisible(uiNodes[#val]);

namespace SharedScreenshotLogic {

	std::string getFormattedDate();
	void screenshot(CCNode* node);

	constexpr std::array monthNames = {
		"Unknown",
		"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December",
		"Unknown"
	};

}