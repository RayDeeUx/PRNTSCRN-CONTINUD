#pragma once

using namespace geode::prelude;

/*
* Node IDs hidden in PlayLayer:
* UILayer
* debug-text
* testmode-label
* percentage-label
* mat.run-info/RunInfoWidget
* cheeseworks.speedruntimer/timer
* progress-bar
* sawblade.dim_mode/opacityLabel
* zilko.xdbot/state-label
* zilko.xdbot/frame-label
* zilko.xdbot/recording-audio-label
* zilko.xdbot/button-menu
* dankmeme.globed2/game-overlay
* thesillydoggo.qolmod/noclip-tint-popup [OUTDATED] [because no one should be using QOLMod v1.x]
* thesillydoggo.qolmod/noclip-tint-overlay [QOLMod v2.x]
* eclipse.eclipse-menu/nocliptint (child of UILayer)
* tobyadd.gdh/labels_top_left
* tobyadd.gdh/labels_top_right
* tobyadd.gdh/labels_bottom_left
* tobyadd.gdh/labels_bottom_right
* tobyadd.gdh/labels_bottom
* tobyadd.gdh/labels_top
* zilko.grayscale/grayscale-mode-sprite
* zilko.editor_trail_in_game/drawy-node (THIS IS PART OF THE "HIDE PLAYER" FILTER. ITS PARENT NODE IS THE PLAYLAYER'S BATCH NODE.)
* ANY NODE ID PREFIXED WITH "dankmeme.globed2/remote-player-progress-" IN PLATFORMER LEVELS SPECIFICALLY
*/

/*
* Node IDs hidden in LevelEditorLayer:
* UILayer
* EditorUI
* EditorPauseLayer
* dankmeme.globed2/game-overlay
*/

/*
* Node IDs hidden when hiding the attempt label:
* raydeeux.attemptlabeltweaks/custom-attempt-label
*/

/*
* Node ID prefixes when hiding players from multiplayer mods (Globed, Champions) [parent node containing players is "GJBaseGameLayer::m_objectLayer"]:
* "dankmeme.globed2/remote-player-" (make sure to filter out node ID "dankmeme.globed2/remote-player-progress-") (then querySelector("dankmeme.globed2/visual-player1 > PlayerObject") or querySelector("dankmeme.globed2/visual-player2 > PlayerObject") for the actual player object)
* "ninxout.champions/player1-"
* "ninxout.champions/player2-"
* "dankmeme.globed2/player-node" [FOR "GLOBED V3" ONLY] (this node contains all PlayerObject nodes of remote players; hiding the whole thing is enough) (THIS IS FOR GLOBED'S NEXT MAJOR VERSION. YOU CAN ADD IT NOW, BUT GLOBED DOES NOT ASSIGN ANY NODE WITH THIS NODE ID YET [as of January 12, 2025])
*/

/*
* PlayerObject member variables whose scales are reduced to 0 then restored after a screenshot:
* m_waveTrail
* m_ghostTrail
* m_shipStreak
* m_regularTrail
* m_dashParticles
* m_dashFireSprite
* m_landParticles0
* m_landParticles1
* m_ufoClickParticles
* m_trailingParticles
* m_shipClickParticles
* m_robotBurstParticles
* m_swingBurstParticles1
* m_swingBurstParticles2
* m_dashSpritesContainer
* m_playerGroundParticles
* m_vehicleGroundParticles
*/

/*
* Node typeinfos of certain interest [the mod + version the node typeinfo comes from] {additional commentary if applicable} (AKA, people who dont add their stuff to UILayer for whatever reason)
* status::Manager + status::Label [MegaHack v9.x] {Come on, Absolute. You did things correctly the first time around with v8.x. Was this change really necessary?}
* NoclipTint [MegaHack v9.x]
* eclipse::gui::cocos::cocos [Eclipse v1.x] {This is actually a child of CCScene::get(), but is included here so PRNTSCRN can screenshot the scene without Eclipse's mobile UI getting in the way.}
* eclipse::hacks::Level::PauseCountdown [Eclipse v1.x] {Part of the "Hide UI" filter}
* HeartsContainer [Lives in GD v1.x]
* LevelProgressionLives [Difficulty Progression v1.x] {dogotrigger actually gives this node the ID "lp-overlay", but there is no mod ID prefix. hiding by typeinfo name for consistency.}
*/

/// @brief PRNTSCRN: Pretty Rad, Nifty Tool; Screen Capture Right Now [v1.0.0]
/// @note - API by Erymanthus | RayDeeUx (Discord: @erymanthus, GitHub/Codeberg: @RayDeeUx), with advice from Prevter
/// @note - scroll down for a TL;DR!
/// @note - this namespace is not designed to screenshot any specific pointer variables belonging to the GJBaseGameLayer (GJBGL), PlayLayer (PL), or LevelEditorLayer (LEL) classes.
/// @note - anyone found attempting to screenshot node pointers in GJBGL, PL, or LEL will most likely be met with severe disappointment.
/// @note - anyone found using this API to screenshot any node whose content size does not match the content size of the current running scene will also most likely be met with severe disappointment.
/// @note - screenshots of nodes with transparent backgrounds and/or content sizes smaller than the content size of the current running scene will have black backgrounds, not transparent backgrounds. this is because OpenGL is the kind of language with a higher barrier of entry that rarely takes kindly to noobs. if anyone has any suggestions on how to improve the OpenGL calls in this function, good luck.
/// @note - all screenshots using this API will use the window size.
/// @note - all screenshots using this API will be saved within (a subfolder of) the config directory for the mod `ninxout.prntscrn`. the subfolder will be prefixed with your mod ID.
/// @note - all screenshots using this API will NOT (READ: NOT!!!) be copied to the user's clipboard.
/// @note - all screenshots using this API will NOT (READ: NOT!!!) play any SFX.
/// @note - try your absolute darndest to space out your screenshots. this is a technical limitation known as "computers are not physically capable of saving raw byte data to image files on their disks instantaneously", and by using this API you *must* assume that PRNTSCRN has not found a workaround for this issue internally; even if PRNTSCRN's changelogs indicate otherwise.
/// @note - you shouldn't be requesting screenshots incredibly frequently. if you are, please consider implementing a screen recorder instead.
/// @note - TL;DR: even though any CCNode* is theoretically supported, the PRNTSCRN namespace is best used to screenshot nodes with non-transparent backgrounds whose contents fill the whole screen.
namespace PRNTSCRN {
	class ScreenshotEvent final : public geode::Event<ScreenshotEvent, bool(CCNode*, std::vector<CCNode*>, std::vector<std::string>, std::string)> {
		public:
			using Event::Event;
			CCNode* nodeToScreenshot {};
			std::vector<CCNode*> nodePointersToHide {};
			std::vector<std::string> querySelectorsToHide {};
			std::string senderModID;

			/*
			/// @note base ScreenshotEvent constructor
			explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {
				nodePointersToHide = {};
				querySelectorsToHide = {};
				senderModID = geode::getMod()->getID();
			}
			/// @note this constructor is available for convenience. use with caution!
			explicit ScreenshotEvent(CCNode* node, const std::vector<CCNode*>& pointers) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = {};
				senderModID = geode::getMod()->getID();
			}
			/// @note this constructor is available for convenience. use with caution!
			explicit ScreenshotEvent(CCNode* node, const std::vector<std::string>& querySelectors) : nodeToScreenshot(node) {
				nodePointersToHide = {};
				querySelectorsToHide = querySelectors;
				senderModID = geode::getMod()->getID();
			}
			/// @note complex ScreenshotEvent constructor (variant one) [used by API]
			explicit ScreenshotEvent(CCNode* node, const std::vector<CCNode*>& pointers, const std::vector<std::string>& querySelectors) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = querySelectors;
				senderModID = geode::getMod()->getID();
			}
			/// @note complex ScreenshotEvent constructor (variant one) [available for convenience, use with caution!]
			explicit ScreenshotEvent(CCNode* node, const std::vector<std::string>& querySelectors, const std::vector<CCNode*>& pointers) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = querySelectors;
				senderModID = geode::getMod()->getID();
			}
			[[nodiscard]] CCNode* getNode() const { return nodeToScreenshot; }
			[[nodiscard]] std::vector<CCNode*> getPointersToHide() const { return nodePointersToHide; }
			[[nodiscard]] std::vector<std::string> getQuerysToHide() const { return querySelectorsToHide; }
			[[nodiscard]] std::string getSenderModID() const { return senderModID; }
			*/
	};

	enum ReferenceType {
		ByID = 0,
		ByIDRecursive = 1,
		ByQuerySelector = 2
	};

	/// @note - THIS FUNCTION IS FOR LOGGING PURPOSES WHEN CALLING PRNTSCRN::screenshotNodeByTypeFrom() ONLY.
	/// @note - YOU SHOULD NOT NEED TO CALL THIS FUNCTION IN 99.9999999999999999999999999999999999% OF CASES.
	/// @note - code happily reused from geode-sdk/DevTools.
	inline std::string __demangle__(const char* typeName) {
		#ifdef GEODE_IS_WINDOWS
		return typeName + 6;
		#else
		std::string ret;

		int status = 0;
		auto demangle = abi::__cxa_demangle(typeName, 0, 0, &status);
		if (status == 0) {
			ret = demangle;
		}
		free(demangle);

		return ret;
		#endif
	}

	/// @brief screenshot a node as seen on the screen, but with more advanced options in choosing the nodes you want to hide for the screenshot (by pointer and by querySelector).
	/// @param node CCNode* pointer of the node being screenshotted, *AND* whose children nodes you want to make invisible by calling node->querySelector(). (yes, the querySelectors you want to hide must descend from the `node` CCNode* pointer.
	/// @param pointersToHide vector of CCNode* pointers that should be made invisible for the screenshot. can be left empty.
	/// @param querySelectorsToHide vector of `std::string`s that should be made invisible for the screenshot. these nodes will be accessed via CCNode::querySelector() from the `node` param. can be left empty.
	/// @returns a Result of Err if something is wrong with the node (null), otherwise returns Ok().
	/// @note - unlike most other function calls in this namespace, PRNTSCRN::screenshotNodeAdvanced() ignores the user's personal preferences for hiding the UI, the player, the attempt text, and the visible checkpoints as per their settings for PRNTSCRN.
	/// @note - this function is designed for screenshotting PlayLayer or LevelEditorLayer with your own set of node pointers or querySelector nodes to hide. you should have already decided the contents of the two std::vector params before calling this.
	/// @note - if you do not feel comfortable using this option to hide nodes by pointer or by querySelector, you are responsible for hiding specific nodes on your own, using PRNTSCRN::screenshotNode instead, and manually restoring the visibility states of those nodes you chose to hide.
	/// @note - if you want to hide Practice Mode checkpoints, YOU (YES, YOU) are responsible for hiding Practice Mode checkpoints on your own BEFORE calling this function. this is because Practice Mode checkpoints are stored in PlayLayer::m_checkpointArray, and you need to edit each individual CheckpointObject's m_physicalCheckpointObject variable (i recommend setting opacity to 0).
	/// @note - if a node fails to hide, double check your querySelectorsToHide vector and your pointersToHide vector. they could exist anywhere else on the node tree during your screenshot, which is an edge case PRNTSCRN is not designed for, nor is PRNTSCRN responsible for.
	/// @note - for more info on (how to use) querySelector, see https://docs.geode-sdk.org/classes/cocos2d/CCNode#querySelector.
	inline geode::Result<> screenshotNodeAdvanced(CCNode* node, const std::vector<CCNode*>& pointersToHide, const std::vector<std::string>& querySelectorsToHide) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced"));
		}
		ScreenshotEvent().send(node, pointersToHide, querySelectorsToHide, geode::getMod()->getID());
		return Ok();
	}

	/// @brief screenshot PlayLayer without any node visibility filters, since, yknow, yall are hella lazy sometimes
	/// @note - this tells PRNTSCRN to take a screenshot of PlayLayer WHILE completely ignoring the user's node visibility filters in their PRNTSCRN settings.
	/// @note - the length of this function name is intentionally long in order for you to perform your own cost/benefit analyses from calling this function.
	inline geode::Result<> screenshotPlayLayerWithoutAnyVisibilityFilters() {
		PlayLayer* pl = PlayLayer::get();
		if (!pl) {
			log::error("[PRNTSCRN API] screenshotPlayLayerWithoutAnyVisibilityFilters failed, NO PLAYLAYER FOUND!");
			return Err(fmt::format("[PRNTSCRN API] screenshotPlayLayerWithoutAnyVisibilityFilters failed, NO PLAYLAYER FOUND!"));
		}
		return PRNTSCRN::screenshotNodeAdvanced(pl, {FMODAudioEngine::get()}, {""_spr}); // neither of these nodes would exist on the node tree anyway so it gets the job done
	}

	/// @brief screenshot LevelEditorLayer without any node visibility filters, since, yknow, yall are hella lazy sometimes
	/// @note - this tells PRNTSCRN to take a screenshot of LevelEditorLayer WHILE completely ignoring the user's node visibility filters in their PRNTSCRN settings.
	/// @note - the length of this function name is intentionally long in order for you to perform your own cost/benefit analyses from calling this function.
	inline geode::Result<> screenshotLevelEditorLayerWithoutAnyVisibilityFilters() {
		LevelEditorLayer* lel = LevelEditorLayer::get();
		if (!lel) {
			log::error("[PRNTSCRN API] screenshotLevelEditorLayerWithoutAnyVisibilityFilters failed, NO LEVELEDITORLAYER FOUND!");
			return Err(fmt::format("[PRNTSCRN API] screenshotLevelEditorLayerWithoutAnyVisibilityFilters failed, NO LEVELEDITORLAYER FOUND!"));
		}
		return PRNTSCRN::screenshotNodeAdvanced(lel, {FMODAudioEngine::get()}, {""_spr}); // neither of these nodes would exist on the node tree anyway so it gets the job done
	}

	/// @brief screenshot a node as seen on the screen.
	/// @param node CCNode* being screenshotted. you are responsible for filling in the CCNode* parameter.
	/// @returns a Result of Err if something is wrong with the node (null), otherwise returns Ok().
	/// @note - if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI, the player, the attempt label (from PlayLayer), or the practice mode checkpoints (from PlayLayer) in PRNTSCRN's settings will apply.
	inline geode::Result<> screenshotNode(CCNode* node) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNode");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNode"));
		}
		ScreenshotEvent().send(node, {}, {}, geode::getMod()->getID());
		return Ok();
	}

	/// @brief screenshot a node (accessed by CCNode::getChildByID(), CCNode::getChildByIDRecursive(), or CCNode::querySelector()) as seen on the screen.
	/// @param parent CCNode* whose children you want to access and take a screenshot of.
	/// @param querySelectorOrIDOrRecursive the string to be used. could be a node ID for CCNode::getChildByID(), could be a node ID for CCNode::getChildByIDRecursive(), or a querySelector for CCNode::querySelector().
	/// @param fetchType an enum of type PRNTSCRN::ReferenceType to determine if the `querySelectorOrIDOrRecursive` parameter is used for CCNode::getChildByID(), CCNode::getChildByIDRecursive(), or CCNode::querySelector().
	/// @returns a Result of Err if something is wrong with the parent node (null, lacks children nodes, querySelectorOrIDOrRecursive was empty, or node was not found), otherwise returns Result from calling PRNTSCRN::screenshotNode().
	/// @note if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI, the player, the attempt label (from PlayLayer), or the practice mode checkpoints (from PlayLayer) in PRNTSCRN's settings will apply.
	/// @note - for more info on (how to use) querySelector, see https://docs.geode-sdk.org/classes/cocos2d/CCNode#querySelector.
	inline geode::Result<> screenshotNodeUsingStringFrom(CCNode* parent, const std::string_view querySelectorOrIDOrRecursive, ReferenceType fetchType) {
		if (!parent) {
			log::error("[PRNTSCRN API] unable to reference parent node");
			return Err(fmt::format("[PRNTSCRN API] unable to reference parent node"));
		}
		if (parent->getChildrenCount() < 1) {
			log::error("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead");
			return Err(fmt::format("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead"));
		}
		if (querySelectorOrIDOrRecursive.empty()) {
			log::error("[PRNTSCRN API] querySelectorOrIDOrRecursive parameter was empty");
			return Err(fmt::format("[PRNTSCRN API] querySelectorOrIDOrRecursive parameter was empty"));
		}
		CCNode* node = nullptr;
		switch (fetchType) {
			default: break;
			case ByID:
				node = parent->getChildByID(querySelectorOrIDOrRecursive);
				break;
			case ByIDRecursive:
				node = parent->getChildByIDRecursive(querySelectorOrIDOrRecursive);
				break;
			case ByQuerySelector:
				node = parent->querySelector(querySelectorOrIDOrRecursive);
				break;
		}
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node with string {} using ReferenceType mode {}", querySelectorOrIDOrRecursive, static_cast<int>(fetchType));
			return Err(fmt::format("[PRNTSCRN API] unable to reference node with string {} using ReferenceType mode {}", querySelectorOrIDOrRecursive, static_cast<int>(fetchType)));
		}
		return PRNTSCRN::screenshotNode(node);
	}

	/// @brief screenshot a node as seen on the screen. the syntax is similar to geode's CCNode::getChildByTag(tag).
	/// @param parent CCNode* pointer whose child node you want to access by tag, and take a screenshot of.
	/// @param tag the tag of the child you want to screenshot. behavior relies entirely on Cocos-2dx's original CCNode::getChildByTag() function.
	/// @returns a Result of Err if something is wrong with the parent node (null, lacks children nodes, tag param is less than 0), otherwise returns Result from calling PRNTSCRN::screenshotNode().
	/// @note - if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI, the player, the attempt label (from PlayLayer), or the practice mode checkpoints (from PlayLayer) in PRNTSCRN's settings will apply.
	/// @note - the node being accessed by its tag MUST be a direct descendant of the `parent` parameter!
	/// @note - this function returns early if the `tag` param is less than 0, as most CCNode* variables lack tags and use -1 as a fallback value.
	inline geode::Result<> screenshotNodeUsingTagFrom(CCNode* parent, int tag) {
		if (!parent) {
			log::error("[PRNTSCRN API] unable to reference parent node");
			return Err(fmt::format("[PRNTSCRN API] unable to reference parent node"));
		}
		if (parent->getChildrenCount() < 1) {
			log::error("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead");
			return Err(fmt::format("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead"));
		}
		if (tag < 0) {
			log::error("[PRNTSCRN API] unable to fetch child by tag. tag {} is less than 0, and CCNodes without a specific tag are assigned -1 by default which makes PRNTSCRN::screenshotNodeUsingTagFrom() useless", tag);
			return Err(fmt::format("[PRNTSCRN API] unable to fetch child by tag. tag {} is less than 0, and CCNodes without a specific tag are assigned -1 by default which makes PRNTSCRN::screenshotNodeUsingTagFrom() useless", tag));
		}
		CCNode* node = parent->getChildByTag(tag);
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node using screenshotNodeUsingTagFrom with tag {}", tag);
			return Err(fmt::format("[PRNTSCRN API] unable to reference node using screenshotNodeUsingTagFrom with tag {}", tag));
		}
		return PRNTSCRN::screenshotNode(node);
	}

	/// @brief screenshot a node as seen on the screen. the syntax is similar to geode's CCNode::getChildByType<T>(index). T is the type of node you want to screenshot.
	/// @param parent CCNode* whose children you want to access and take a screenshot of.
	/// @param index the nth child of type T that you want to screenshot, if there are multiple children of type T from param parent.
	/// @returns a Result of Err if something is wrong with the parent node (null or lacks children nodes), otherwise returns Result from calling PRNTSCRN::screenshotNode().
	/// @note if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI, the player, the attempt label (from PlayLayer), or the practice mode checkpoints (from PlayLayer) in PRNTSCRN's settings will apply.
	template<class T>
	geode::Result<> screenshotNodeByTypeFrom(CCNode* parent, int index) {
		if (!parent) {
			log::error("[PRNTSCRN API] unable to reference parent node using screenshotNodeByTypeFrom");
			return Err(fmt::format("[PRNTSCRN API] unable to reference parent node using screenshotNodeByTypeFrom"));
		}
		if (parent->getChildrenCount() < 1) {
			log::error("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead");
			return Err(fmt::format("[PRNTSCRN API] parent node lacks children, call PRNTSCRN::screenshotNode() on the parent node instead"));
		}
		CCNode* node = parent->getChildByType<T>(index);
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", PRNTSCRN::__demangle__(typeid(T).name()), index);
			return Err(fmt::format("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", PRNTSCRN::__demangle__(typeid(T).name()), index));
		}
		return PRNTSCRN::screenshotNode(node);
	}

	/// @brief screenshot PlayLayer using the user's own PRNTSCRN visibility settings, since, yknow, yall are hella lazy sometimes
	/// @note - this tells PRNTSCRN to take a screenshot of PlayLayer USING the user's node visibility filters in their PRNTSCRN settings.
	/// @note - the length of this function name is intentionally long in order for you to perform your own cost/benefit analyses from calling this function.
	inline geode::Result<> screenshotPlayLayerUsingUsersOwnPRNTSCRNPreferences() {
		PlayLayer* pl = PlayLayer::get();
		if (!pl) {
			log::error("[PRNTSCRN API] screenshotPlayLayerUsingUsersOwnPRNTSCRNPreferences failed, NO PLAYLAYER FOUND!");
			return Err(fmt::format("[PRNTSCRN API] screenshotPlayLayerUsingUsersOwnPRNTSCRNPreferences failed, NO PLAYLAYER FOUND!"));
		}
		return PRNTSCRN::screenshotNode(pl);
	}

	/// @brief screenshot LevelEditorLayer using the user's own PRNTSCRN visibility settings, since, yknow, yall are hella lazy sometimes
	/// @note - this tells PRNTSCRN to take a screenshot of LevelEditorLayer USING the user's node visibility filters in their PRNTSCRN settings.
	/// @note - the length of this function name is intentionally long in order for you to perform your own cost/benefit analyses from calling this function.
	inline geode::Result<> screenshotLevelEditorLayerUsingUsersOwnPRNTSCRNPreferences() {
		LevelEditorLayer* lel = LevelEditorLayer::get();
		if (!lel) {
			log::error("[PRNTSCRN API] screenshotLevelEditorLayerUsingUsersOwnPRNTSCRNPreferences failed, NO LEVELEDITORLAYER FOUND!");
			return Err(fmt::format("[PRNTSCRN API] screenshotLevelEditorLayerUsingUsersOwnPRNTSCRNPreferences failed, NO LEVELEDITORLAYER FOUND!"));
		}
		return PRNTSCRN::screenshotNode(lel);
	}
}