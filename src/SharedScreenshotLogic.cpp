// ReSharper disable CppDFAUnreachableCode
// ReSharper disable CppDFAConstantConditions
// yeah that's right clion stop saying my code is unreachable >:(
#include "SharedScreenshotLogic.hpp"
#include "Screenshot.hpp"
#include "Manager.hpp"
#include <api.hpp>
#include <chrono>

using namespace geode::prelude;

std::string normalizePath(std::filesystem::path thePath) {
	#ifdef GEODE_IS_WINDOWS
	return geode::utils::string::wideToUtf8(thePath.wstring());
	#else
	return thePath.string();
	#endif
}

std::string SharedScreenshotLogic::getFormattedDate() {
	auto nowTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto now = std::localtime(&nowTimeT);
	auto humanReadableMonth = SharedScreenshotLogic::monthNames[static_cast<unsigned int>(now->tm_mon + 1)];
	return fmt::format("{} {}, {}", humanReadableMonth, now->tm_mday, now->tm_year + 1900);
}

void SharedScreenshotLogic::hideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_regularTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashFireSprite, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_landParticles0, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_landParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_ufoClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_trailingParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_shipClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_robotBurstParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_swingBurstParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_swingBurstParticles2, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_dashSpritesContainer, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_playerGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player1->m_vehicleGroundParticles, unorderedMapStoringScales);
	// ADD_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
}

void SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	ADD_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_regularTrail, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashFireSprite, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_landParticles0, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_landParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_ufoClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_trailingParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_shipClickParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_robotBurstParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_swingBurstParticles1, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_swingBurstParticles2, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_dashSpritesContainer, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_playerGroundParticles, unorderedMapStoringScales);
	ADD_SCALE(gjbgl, m_player2->m_vehicleGroundParticles, unorderedMapStoringScales);
	// ADD_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
}

void SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer* gjbgl) {
	RES_SCALE(gjbgl, m_player1->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_shipStreak, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_regularTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashFireSprite, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_landParticles0, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_landParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_ufoClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_trailingParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_shipClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_robotBurstParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_swingBurstParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_swingBurstParticles2, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_dashSpritesContainer, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_playerGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player1->m_vehicleGroundParticles, unorderedMapStoringScales);
	// RES_SCALE(gjbgl, m_player1->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
}

void SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(std::unordered_map<CCNode*, float>& unorderedMapStoringScales, GJBaseGameLayer *gjbgl) {
	RES_SCALE(gjbgl, m_player2->m_waveTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_ghostTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_shipStreak, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_regularTrail, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashFireSprite, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_landParticles0, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_landParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_ufoClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_trailingParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_shipClickParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_robotBurstParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_swingBurstParticles1, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_swingBurstParticles2, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_dashSpritesContainer, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_playerGroundParticles, unorderedMapStoringScales);
	RES_SCALE(gjbgl, m_player2->m_vehicleGroundParticles, unorderedMapStoringScales);
	// RES_SCALE(gjbgl, m_player2->m_dashFireSprite->getChildByType<CCSprite>(0), unorderedMapStoringScales); // this line won't even work anyway because you need to constantly set its scale to 0.f which is simply not happening
}

void SharedScreenshotLogic::screenshot(CCNode* node) {
	if (!node) return log::error("invalid node!");
	bool hasCustomNodesToHide = false;
	bool isCtrl = false;
	if (CCObject* hasNodeFilters = node->getUserObject("has-custom-nodes-to-hide"_spr); hasNodeFilters) {
		hasCustomNodesToHide = static_cast<CCBool*>(hasNodeFilters)->getValue();
	}
	if (CCObject* isControlFromPopup = node->getUserObject("is-plain-ss-from-popup"_spr); isControlFromPopup) {
		isCtrl = static_cast<CCBool*>(isControlFromPopup)->getValue();
	}
	std::string modIDAskingForScreenshot;
	std::string pauseMenuTypeForSetting;
	if (CCObject* modID = node->getUserObject("mod-asking-for-screenshot"_spr); modID) {
		auto cString = static_cast<std::string>(static_cast<CCString*>(modID)->getCString());
		if (!cString.empty()) modIDAskingForScreenshot = fmt::format("{} - ", cString);
	}
	if (CCObject* pauseMenuType = node->getUserObject("pause-menu-type"_spr); pauseMenuType) {
		auto cString = static_cast<std::string>(static_cast<CCString*>(pauseMenuType)->getCString());
		if (!cString.empty()) pauseMenuTypeForSetting = cString;
	}
	bool modRequestingScreenshotIsSelf = modIDAskingForScreenshot.empty() || modIDAskingForScreenshot == Mod::get()->getID();

	// event filter from main.cpp will have already hidden the nodes by this point
	std::unordered_map<const char*, bool> uiNodes = {};
	std::unordered_map<CCNode*, float> playerPointerScales = {};
	bool hideUI = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-ui");
	bool hidePL = hasCustomNodesToHide ? false : Mod::get()->getSettingValue<bool>("hide-player");

	PlayLayer* pl = typeinfo_cast<PlayLayer*>(node);
	LevelEditorLayer* lel = typeinfo_cast<LevelEditorLayer*>(node);

	if (isCtrl) ADD_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (hideUI && pl) {
		ADD_NODE(pl, UILayer);
		ADD_NODE(pl, debug-text);
		ADD_NODE(pl, testmode-label);
		ADD_NODE(pl, percentage-label);
		ADD_NODE(pl, mat.run-info/RunInfoWidget);
		ADD_NODE(pl, cheeseworks.speedruntimer/timer);
		ADD_NODE(pl, progress-bar);
	}
	if (hideUI && lel) {
		ADD_NODE(lel, UILayer);
		ADD_NODE(lel, EditorUI);
		ADD_NODE(lel, EditorPauseLayer);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		ADD_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::hideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		ADD_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::hideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}
	CCSize selectedSize = CCSize(Manager::get()->width, Manager::get()->height);
	if (!modRequestingScreenshotIsSelf) selectedSize = CCDirector::get()->getWinSizeInPixels();
	Screenshot ss = Screenshot(selectedSize, node);
	if (isCtrl) RES_NODE(CCScene::get(), ninxout.prntscrn/ScreenshotPopup);
	if (hideUI && pl) {
		RES_NODE(pl, UILayer);
		RES_NODE(pl, debug-text);
		RES_NODE(pl, testmode-label);
		RES_NODE(pl, percentage-label);
		RES_NODE(pl, mat.run-info/RunInfoWidget);
		RES_NODE(pl, cheeseworks.speedruntimer/timer);
		RES_NODE(pl, progress-bar);
	}
	if (hideUI && lel) {
		RES_NODE(lel, UILayer);
		RES_NODE(lel, EditorUI);
		RES_NODE(lel, EditorPauseLayer);
	}
	if (hidePL && (pl || lel)) {
		GJBaseGameLayer* gjbgl = static_cast<GJBaseGameLayer*>(node);
		RES_MEM(gjbgl, m_player1);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerOne(playerPointerScales, gjbgl);

		RES_MEM(gjbgl, m_player2);
		SharedScreenshotLogic::unhideOtherPartsOfPlayerTwo(playerPointerScales, gjbgl);
	}

	bool jpeg = Mod::get()->getSettingValue<bool>("jpeg-mafia");
	std::string extension = jpeg ? ".jpg" : ".png";

	std::string formattedDate = SharedScreenshotLogic::getFormattedDate();
	std::string targetFolderName = modIDAskingForScreenshot.empty() ? "" : modIDAskingForScreenshot;

	GJGameLevel* level = nullptr;
	if (pl || lel) level = static_cast<GJBaseGameLayer*>(node)->m_level;
	if (level) {
		const std::string& lvlIDString = level->m_levelType == GJLevelType::Editor ? "Editor level" : numToString(level->m_levelID);
		targetFolderName += fmt::format("{} - {} ({})", lvlIDString, level->m_levelName, formattedDate);
	} else targetFolderName += formattedDate;

	std::filesystem::path folder = Mod::get()->getConfigDir() / targetFolderName;

	if (!std::filesystem::exists(folder)) std::filesystem::create_directory(folder);

	int index = 1;
	while (std::filesystem::exists(folder / (numToString(index) + extension))) {
		index++;
	}

	std::string filename = normalizePath(folder / (numToString(index) + extension));
	ss.intoFile(filename, jpeg);
	if (Mod::get()->getSettingValue<bool>("copy-clipboard") && modRequestingScreenshotIsSelf) {
		bool shouldCopy = true;
		if (!pauseMenuTypeForSetting.empty()) {
			shouldCopy = false;
			const std::string& pauseMenuPreference = utils::string::toLower(Mod::get()->getSettingValue<std::string>("copy-screenshot-with-pause-menu-on"));
			// "Never", "Regular levels only", "Editor levels only", "All levels"
			if (pauseMenuPreference == "all levels") shouldCopy = true;
			else if (pauseMenuPreference == "never") shouldCopy = false;
			else if (pauseMenuPreference == "editor levels only" && pauseMenuTypeForSetting == "EditorPauseLayer") shouldCopy = true;
			else if (pauseMenuPreference == "regular levels only" && pauseMenuTypeForSetting == "PauseLayer") shouldCopy = true;
		}
		if (shouldCopy) ss.intoClipboard();
	}
}

void SharedScreenshotLogic::screenshotLevelOrScene() {
	CCNode* nodeToScreenshot = CCScene::get();
	PlayLayer* pl = PlayLayer::get();
	LevelEditorLayer* lel = LevelEditorLayer::get();
	if (pl && pl->getParent() == CCScene::get()) {
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
	if (lel && lel->getParent() == CCScene::get()) {
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
}