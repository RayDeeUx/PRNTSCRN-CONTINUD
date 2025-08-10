#pragma once

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

class Manager {
protected:
	static Manager* instance;
public:
	int width = 1980;
	int height = 1080;
	static Manager* get() {
		if (!instance) instance = new Manager();
		return instance;
	}
};