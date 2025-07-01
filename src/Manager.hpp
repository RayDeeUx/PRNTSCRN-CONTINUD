#pragma once

#define ADD_NODE(val) \
	if (auto node = pl->getChildByID(#val)) { \
		uiNodes[#val] = node->isVisible(); \
		node->setVisible(false); \
	}

#define ADD_MEM(val) \
	uiNodes[#val] = pl->val->isVisible(); \
	pl->val->setVisible(false);

#define RES_NODE(val) pl->getChildByID(#val)->setVisible(uiNodes[#val]);

#define RES_MEM(val) pl->val->setVisible(uiNodes[#val]);

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