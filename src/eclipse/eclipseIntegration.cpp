#include "eclipse.hpp"
#include "../SharedScreenshotLogic.hpp"
#include "../include/api.hpp"

using namespace eclipse;
using namespace geode::prelude;

$on_mod(Loaded) {
	Loader::get()->queueInMainThread([] {
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
		});

		(void) tab.addModSettingToggle(Mod::get()->getSetting("copy-clipboard"));
	});
}
