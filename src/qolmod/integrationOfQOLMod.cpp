#include "CategoryExt.hpp"
#include "../Manager.hpp"
#include "../ScreenshotPopup.hpp"

using namespace geode::prelude;

// this sounds evil on paper but i swear i have a good reason for doing this --raydeeux
class QOLModAndroidUITrackerDummyNodeForPRNTSCRN final : public CCNode {
public:
	void trackAndroidUIScheduler(float) {
		if (!Manager::get()->trackAndroidUI) return;
		for (CCNode* child : cocos2d::CCScene::get()->getChildrenExt()) {
			if (geode::cocos::getObjectName(child) == "AndroidUI") return;
		}
		Manager::get()->trackAndroidUI = false;
		Manager::get()->screenshotPopupForQOLMod = nullptr;
	}
};

$on_mod(Loaded) {
	Loader::get()->queueInMainThread([]() {
		GameManager::get()->schedule(schedule_selector(QOLModAndroidUITrackerDummyNodeForPRNTSCRN::trackAndroidUIScheduler));
		qolmod::ext::addCustomCategory({1, Mod::get()->getName(), "qolmod.png"_spr, Mod::get()->getID(), [](cocos2d::CCMenu* layer) {
			if (!layer) return;

			if (ScreenshotPopup* foo = CCScene::get()->getChildByType<ScreenshotPopup>(0); foo) {
				geode::Popup::CloseEvent(foo).send();
				foo->setKeypadEnabled(false);
				foo->setTouchEnabled(false);
				foo->removeFromParent();
			}

			// if you're going to steal my source code then at least watch Heated Rivalry S1E6 to understand these variable name references, you cheap clankers and/or fuckwads --raydeeux
			CCLabelBMFont* iHaveThisProblem = CCLabelBMFont::create("[If you're viewing this inside the QOLMod Discord server, whoever sent this has never learned how to read.]", "chatFont.fnt");
			iHaveThisProblem->limitLabelWidth(layer->getContentWidth() * .65f, 1.f, .0001f);
			iHaveThisProblem->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
			iHaveThisProblem->setScale(.4f);

			CCLabelBMFont* iLikeWomenYesAndEverywhereIGoImSurroundedByBeautifulWomenAndTheyLoveMeAndTheseWomenTheyreSoSexyAndFun = CCLabelBMFont::create("Heads up! This menu is NOT native to QOLMod!!", "goldFont.fnt");
			iLikeWomenYesAndEverywhereIGoImSurroundedByBeautifulWomenAndTheyLoveMeAndTheseWomenTheyreSoSexyAndFun->limitLabelWidth(layer->getContentWidth() * .85f, 1.f, .0001f);
			iLikeWomenYesAndEverywhereIGoImSurroundedByBeautifulWomenAndTheyLoveMeAndTheseWomenTheyreSoSexyAndFun->setScale(.5f);

			CCLabelBMFont* butImAlwaysThinkgAboutThisSlowFuckingHockeyPlayerWithBeautifulFrecklesAndAWeakBackhandAndHesSoBoringAndHeDrivesThisTerribleCar = CCLabelBMFont::create("This menu comes from PRNTSCRN!", "bigFont.fnt");
			butImAlwaysThinkgAboutThisSlowFuckingHockeyPlayerWithBeautifulFrecklesAndAWeakBackhandAndHesSoBoringAndHeDrivesThisTerribleCar->limitLabelWidth(layer->getContentWidth() * .75f, 1.f, .0001f);
			butImAlwaysThinkgAboutThisSlowFuckingHockeyPlayerWithBeautifulFrecklesAndAWeakBackhandAndHesSoBoringAndHeDrivesThisTerribleCar->setScale(.4f);

			CCLabelBMFont* iAmAlwaysWishingThatTheseWomenWereHimItsATerribleProblemIDontEverWantThatProblemToEverGoAway = CCLabelBMFont::create("Any bugs from using this menu ***__MUST__*** be reported to RayDeeUx and ***__ONLY__*** RayDeeUx!!", "chatFont.fnt");
			iAmAlwaysWishingThatTheseWomenWereHimItsATerribleProblemIDontEverWantThatProblemToEverGoAway->limitLabelWidth(layer->getContentWidth() * .65f, 1.f, .0001f);
			iAmAlwaysWishingThatTheseWomenWereHimItsATerribleProblemIDontEverWantThatProblemToEverGoAway->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
			iAmAlwaysWishingThatTheseWomenWereHimItsATerribleProblemIDontEverWantThatProblemToEverGoAway->setScale(.45f);

			layer->addChildAtPosition(iLikeWomenYesAndEverywhereIGoImSurroundedByBeautifulWomenAndTheyLoveMeAndTheseWomenTheyreSoSexyAndFun, geode::Anchor::Top, {0.f, -10.f});
			layer->addChildAtPosition(butImAlwaysThinkgAboutThisSlowFuckingHockeyPlayerWithBeautifulFrecklesAndAWeakBackhandAndHesSoBoringAndHeDrivesThisTerribleCar, geode::Anchor::Top, {0.f, -25.f});
			layer->addChildAtPosition(iAmAlwaysWishingThatTheseWomenWereHimItsATerribleProblemIDontEverWantThatProblemToEverGoAway, geode::Anchor::Top, {0.f, -37.5f});
			layer->addChildAtPosition(iHaveThisProblem, geode::Anchor::Top, {0.f, -45.f});

			Manager::get()->isAddingToQOLMod = true;
			Manager::get()->screenshotPopupForQOLMod = nullptr;
			ScreenshotPopup* fuck = ScreenshotPopup::create();
			fuck->show();
			Manager::get()->screenshotPopupForQOLMod = geode::Ref(fuck);
			Manager::get()->isAddingToQOLMod = false;

			fuck->setKeypadEnabled(false);
			fuck->setTouchEnabled(false);
			fuck->removeFromParentAndCleanup(true);

			// so what you need to be doing is unregistering force prio since it isn't being used as a popup --alphalaneous --raydeeux
			if (!geode::Loader::get()->isModLoaded("alk.better-touch-prio")) {
				CCTouchDispatcher::get()->unregisterForcePrio(Manager::get()->screenshotPopupForQOLMod);
			}

			layer->addChildAtPosition(Manager::get()->screenshotPopupForQOLMod, geode::Anchor::Center, {0.f, -25.f});

			Manager::get()->screenshotPopupForQOLMod->ignoreAnchorPointForPosition(false);
			Manager::get()->screenshotPopupForQOLMod->setScale(7.25f);

			Manager::get()->trackAndroidUI = true;
		}});
	});
}