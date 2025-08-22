#include <Geode/ui/GeodeUI.hpp>
#include "eclipse.hpp"
#include "../Manager.hpp"
#include "../ScreenshotPopup.hpp"
#include "../SharedScreenshotLogic.hpp"

using namespace eclipse;
using namespace geode::prelude;

void addSetting(MenuTab& tab, const std::string& settingsKey) {
	if (std::shared_ptr<SettingV3> setting = Mod::get()->getSetting(settingsKey); setting) {
		const std::string& colorlessDesc = string::replace(string::replace(string::replace(setting->getDescription().value_or("Copy screenshots to the clipboard."), "</c>", ""), "<cl>", ""), "<cy>", "");
		tab.addModSettingToggle(setting).setDescription(colorlessDesc);
	}
}

$on_mod(Loaded) {
	Loader::get()->queueInMainThread([] {
		Mod* mod = Mod::get();
		auto tab = MenuTab::find("PRNTSCRN");

		tab.addButton("Screenshot", [](){
			SharedScreenshotLogic::screenshotLevelOrScene();
		}).setDescription("Screenshot the level (either from gameplay or in the editor, if there is one active).\nOtherwise, screenshot the contents of the screen.");

		addSetting(tab, "copy-clipboard");
		addSetting(tab, "hide-ui");
		addSetting(tab, "hide-player");
		addSetting(tab, "jpeg-mafia");
		addSetting(tab, "auto-screenshot");

		tab.addButton("Open Screenshots Folder", [mod]() {
			geode::utils::file::openFolder(mod->getConfigDir());
		});

		tab.addButton("Open PRNTSCRN Settings", [mod]() {
			CCScene* scene = CCScene::get();
			if (!scene) return;
			if (CCNode* ssPopup = scene->getChildByID("ScreenshotPopup"_spr); ssPopup) {
				if (CCNode* alert = scene->getChildByID("FLAlertLayer"); alert) {
					std::string titleString;
					CCNode* title = static_cast<FLAlertLayer*>(alert)->m_mainLayer->getChildByID("title");
					if (title) titleString = static_cast<std::string>(static_cast<CCLabelBMFont*>(title)->getString());
					if (utils::string::startsWith(titleString, "PRNTSCRN - ")) {
						static_cast<FLAlertLayer*>(alert)->keyBackClicked();
					}
				}
				static_cast<ScreenshotPopup*>(ssPopup)->keyBackClicked();
			}
			geode::openSettingsPopup(mod);
		});

		if (mod->getSettingValue<bool>("show-more-details")) {
			(void) tab.addLabel("Most recent settings:");
			tab.addLabel(fmt::format("Screenshot size: {} x {}", Manager::get()->width, Manager::get()->height));
			if (std::shared_ptr<SettingV3> setting = mod->getSetting("copy-screenshot-with-pause-menu-on"); setting) {
				tab.addLabel(fmt::format("{} {}", setting->getDisplayName(), mod->getSettingValue<std::string>("copy-screenshot-with-pause-menu-on")));
			}
			if (std::shared_ptr<SettingV3> setting = mod->getSetting("auto-percent"); setting) {
				tab.addLabel(fmt::format("Auto Screenshot % Interval (Classic levels): {}%", mod->getSettingValue<int64_t>("auto-percent")));
			}
			if (std::shared_ptr<SettingV3> setting = mod->getSetting("auto-seconds"); setting) {
				tab.addLabel(fmt::format("Auto Screenshot Seconds Interval (Plat. levels): {} seconds", mod->getSettingValue<int64_t>("auto-seconds")));
			}
			tab.addLabel(fmt::format("Screenshot the level: {}", Manager::getColorlessBindsStringFor("screenshot"_spr)));
			tab.addLabel(fmt::format("Screenshot the screen: {}", Manager::getColorlessBindsStringFor("plain-screenshot"_spr)));
		}
	});
}