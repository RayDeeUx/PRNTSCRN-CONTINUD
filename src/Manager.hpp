#pragma once

/*
#ifndef GEODE_IS_IOS
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif
*/

using namespace geode;
/*
#ifndef GEODE_IS_IOS
using namespace keybinds;
#endif
*/

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
		std::string bodyText = fmt::format("Screenshot the level: {}", getBindsStringFor("screenshot"));
		bodyText += "\n";
		bodyText += fmt::format("Screenshot the screen: {}", getBindsStringFor("plain-screenshot"));
		return bodyText;
	}
	static std::string getBindsStringFor(const std::string& key, bool hasColor = true) {
		/*
		#ifdef GEODE_IS_IOS
		if (hasColor) return "<c_>N/A</c>";
		return "N/A";
		#else
		std::vector<Ref<keybinds::Bind>> binds = BindManager::get()->getBindsFor(key);
		if (!binds.empty()) {
			const std::string& keybindString = binds.at(0)->toString();
			if (!keybindString.empty()) {
				if (hasColor) return fmt::format("<cl>{}</c>", keybindString);
				return keybindString;
			}
			if (hasColor) return "<co>[Unknown]</c>";
			return "[Unknown]";
		}
		if (hasColor) return "<c_>NONE SET</c>";
		return "NONE SET";
		#endif
		*/
		std::string toReturn;
		for (auto bind : Mod::get()->getSettingValue<std::vector<Keybind>>(key)) {
			if (hasColor) toReturn += bind.toString();
			else toReturn += bind.toString();
		}
		if (toReturn.empty()) {
			if (hasColor) return "<c_>N/A</c>";
			else return "N/A";
		}
		return toReturn;
	}
	static std::string getColorlessBindsStringFor(const std::string& key) {
		return Manager::getBindsStringFor(key, false);
	}
};