#include <Geode/ui/GeodeUI.hpp>
#include "eclipse.hpp"
#include "../Manager.hpp"
#include "../ScreenshotPopup.hpp"
#include "../SharedScreenshotLogic.hpp"
#include "../include/api.hpp"

using namespace eclipse;
using namespace geode::prelude;

$on_mod(Loaded) {
	Loader::get()->queueInMainThread([] {
		Mod* mod = Mod::get();
		auto tab = MenuTab::find("PRNTSCRN");

		tab.addButton("Screenshot", [](){
			CCNode* nodeToScreenshot = CCScene::get();
			PlayLayer* pl = PlayLayer::get();
			LevelEditorLayer* lel = LevelEditorLayer::get();
			if (pl) {
				SharedScreenshotLogic::screenshot(pl);
				if (CCNode* ell = pl->getChildByID("EndLevelLayer"); ell) {
					bool hideUISetting = Loader::get()->getLoadedMod("ninxout.prntscrn")->getSettingValue<bool>("hide-ui"); // guaranteed to get the Mod* pointer
					UILayer* uiLayer = hideUISetting ? pl->m_uiLayer : nullptr;
					std::vector<std::string> nodeIDsToHide = {};
					if (uiLayer) nodeIDsToHide = {"debug-text", "testmode-label", "percentage-label", "mat.run-info/RunInfoWidget", "cheeseworks.speedruntimer/timer", "progress-bar"};
					Result res = PRNTSCRN::screenshotNodeAdvanced(pl, {ell, uiLayer}, nodeIDsToHide);
					if (res.isErr()) log::error("[PRNTSCRN] Something went wrong! ({})", res.unwrapErr());
				} else if (CCScene::get()->getChildByID("PauseLayer")) {
					CCScene* baseScene = CCScene::get();
					baseScene->setUserObject("pause-menu-type"_spr, CCString::create("PauseLayer"));
					SharedScreenshotLogic::screenshot(baseScene);
					baseScene->setUserObject("pause-menu-type"_spr, CCString::create(""));
				}
				return;
			}
			if (lel) {
				SharedScreenshotLogic::screenshot(lel);
				if (lel->getChildByID("EditorPauseLayer")) {
					CCScene* baseScene = CCScene::get();
					baseScene->setUserObject("pause-menu-type"_spr, CCString::create("EditorPauseLayer"));
					SharedScreenshotLogic::screenshot(baseScene);
					baseScene->setUserObject("pause-menu-type"_spr, CCString::create(""));
				}
				return;
			}
			if (nodeToScreenshot) SharedScreenshotLogic::screenshot(nodeToScreenshot);
		}).setDescription("Screenshot the level (either from gameplay or in the editor, if there is one active). Otherwise, screenshot the contents of the screen.");

		if (std::shared_ptr<SettingV3> clipboardSetting = mod->getSetting("copy-clipboard"); clipboardSetting) {
			tab.addModSettingToggle(clipboardSetting).setDescription("Copy screenshots to the clipboard.");
		}
		if (std::shared_ptr<SettingV3> setting = mod->getSetting("copy-screenshot-with-pause-menu-on"); setting) {
			tab.addLabel(fmt::format("{} {}", setting->getDisplayName(), mod->getSettingValue<std::string>("copy-screenshot-with-pause-menu-on")));
		}

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

		tab.addLabel(Manager::getBindsStringFor("screenshot"_spr));
		tab.addLabel(Manager::getBindsStringFor("plain-screenshot"_spr));
	});
}