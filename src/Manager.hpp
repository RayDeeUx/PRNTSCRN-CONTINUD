#pragma once

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