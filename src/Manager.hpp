#pragma once

#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace geode;
using namespace keybinds;

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
	static std::string fetchKeybindsStrings() {
		std::string bodyText = fmt::format("Screenshot the level: {}", getBindsStringFor("screenshot"_spr));
		bodyText += "\n";
		bodyText += fmt::format("Screenshot the screen: {}", getBindsStringFor("plain-screenshot"_spr));
		return bodyText;
	}
	static std::string getBindsStringFor(const std::string& key, bool hasColor = true) {
		std::vector<Ref<keybinds::Bind>> binds = BindManager::get()->getBindsFor(key);
		if (!binds.empty()) {
			const std::string& keybindString = binds.at(0)->toString();
			if (!keybindString.empty()) {
				if (hasColor) return fmt::format("<cl>{}</c>", keybindString);
				return keybindString;
			}
			{
				if (hasColor) return "<co>[Unknown]</c>";
				return "[Unknown]";
			}
		} {
			if (hasColor) return "<c_>NONE SET</c>";
			return "NONE SET";
		}
	}
	static std::string getColorlessBindsStringFor(const std::string& key) {
		return Manager::getBindsStringFor(key, false);
	}
};