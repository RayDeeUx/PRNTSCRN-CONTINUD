#pragma once

using namespace geode;

class Manager {
protected:
	static Manager* instance;
public:
	int width = 1980;
	int height = 1080;
	bool trackAndroidUI = false;
	bool isAddingToQOLMod = false;
	bool fuckVibecodersAndExitscammers = false;
	geode::Mod* waveFix = nullptr;
	geode::Mod* playerSnake = nullptr;
	geode::Ref<FLAlertLayer> screenshotPopupForQOLMod = nullptr;
	static Manager* get() {
		if (!instance) instance = new Manager();
		return instance;
	}
	static std::string fetchKeybindsStrings() {
		std::string bodyText = fmt::format("Screenshot the level: {}", Manager::getBindsStringFor("screenshot"));
		bodyText += "\n";
		bodyText += fmt::format("Screenshot the screen: {}", Manager::getBindsStringFor("plain-screenshot"));
		return bodyText;
	}
	static std::string getBindsStringFor(const std::string& key, bool hasColor = true) {
		std::string toReturn;
		for (auto bind : Mod::get()->getSettingValue<std::vector<Keybind>>(key)) {
			if (hasColor) toReturn += fmt::format("<cl>{}</c>, ", bind.toString());
			else toReturn += fmt::format("{}, ", bind.toString());
		}
		if (toReturn.empty()) {
			if (hasColor) return "<c_>N/A</c>";
			return "N/A";
		}
		if (toReturn.size() > 2 && geode::utils::string::endsWith(toReturn, ", ")) {
			toReturn.pop_back();
			toReturn.pop_back();
		}
		return toReturn;
	}
	static std::string getColorlessBindsStringFor(const std::string& key) {
		return Manager::getBindsStringFor(key, false);
	}
};