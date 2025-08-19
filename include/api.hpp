#pragma once

using namespace geode::prelude;

/// @note - PRNTSCRN: Pretty Rad, Nifty Tool; Screen Capture Right Now
/// @note - this namespace is not designed to screenshot any specific pointers belong to the GJBaseGameLayer, PlayLayer, or LevelEditorLayer classes. anyone found attempting to screenshot such pointers will be met with severe disappointment.
/// @note - API by Erymanthus | RayDeeUx
namespace PRNTSCRN {
	class ScreenshotEvent final : public Event {
		protected:
			CCNode* nodeToScreenshot {};
			std::vector<CCNode*> nodePointersToHide {};
			std::vector<std::string> querySelectorsToHide  {};
		public:
			/// @note base ScreenshotEvent constructor
			explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {
				nodePointersToHide = {};
				querySelectorsToHide = {};
			}
			/// @note this constructor is available for convenience. use with caution!
			explicit ScreenshotEvent(CCNode* node, const std::vector<CCNode*>& pointers) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = {};
			}
			/// @note this constructor is available for convenience. use with caution!
			explicit ScreenshotEvent(CCNode* node, const std::vector<std::string>& querySelectors) : nodeToScreenshot(node) {
				nodePointersToHide = {};
				querySelectorsToHide = querySelectors;
			}
			/// @note complex ScreenshotEvent constructor (variant one) [used by API]
			explicit ScreenshotEvent(CCNode* node, const std::vector<CCNode*>& pointers, const std::vector<std::string>& querySelectors) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = querySelectors;
			}
			/// @note complex ScreenshotEvent constructor (variant one) [available for convenience, use with caution!]
			explicit ScreenshotEvent(CCNode* node, const std::vector<std::string>& querySelectors, const std::vector<CCNode*>& pointers) : nodeToScreenshot(node) {
				nodePointersToHide = pointers;
				querySelectorsToHide = querySelectors;
			}
			[[nodiscard]] CCNode* getNode() const { return nodeToScreenshot; }
			[[nodiscard]] std::vector<CCNode*> getPointersToHide() const { return nodePointersToHide; }
			[[nodiscard]] std::vector<std::string> getQuerysToHide() const { return querySelectorsToHide; }
	};

	enum ReferenceType {
		ByID = 0,
		ByIDRecursive = 1,
		ByQuerySelector = 2
	};

	/// @note - THIS FUNCTION IS FOR LOGGING PURPOSES WHEN CALLING PRNTSCRN::screenshotNodeByTypeFrom() ONLY.
	/// @note - YOU SHOULD NOT NEED TO CALL THIS FUNCTION IN 99.999999% OF CASES.
	/// @note - code happily reused from geode-sdk/DevTools
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
	/// @note - unlike most other function calls in this namespace, PRNTSCRN::screenshotNodeAdvanced() ignores the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings.
	/// @note - this function is designed for screenshotting PlayLayer or LevelEditorLayer with your own set of node pointers or querySelector nodes to hide. you should have already decided the contents of the two std::vector params before calling this.
	/// @note - if you do not feel comfortable using this option to hide nodes by pointer or by querySelector, you are responsible for hiding specific nodes on your own, using PRNTSCRN::screenshotNode instead, and manually restoring the visibility states of those nodes you chose to hide.
	/// @note - if a node fails to hide, double check your querySelectorsToHide vector and your pointersToHide vector. they could exist anywhere else on the node tree during your screenshot, which PRNTSCRN is not responsible for.
	/// @note - for more info on querySelector, see https://docs.geode-sdk.org/classes/cocos2d/CCNode#querySelector.
	inline geode::Result<> screenshotNodeAdvanced(CCNode* node, const std::vector<CCNode*>& pointersToHide, const std::vector<std::string>& querySelectorsToHide) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced"));
		}
		ScreenshotEvent(node, pointersToHide, querySelectorsToHide).post();
		return Ok();
	}

	/// @brief screenshot a node as seen on the screen.
	/// @param node CCNode* being screenshotted. you are responsible for filling in the CCNode* parameter.
	/// @returns a Result of Err if something is wrong with the node (null), otherwise returns Ok().
	/// @note if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
	inline geode::Result<> screenshotNode(CCNode* node) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNode");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNode"));
		}
		ScreenshotEvent(node).post();
		return Ok();
	}

	/// @brief screenshot a node (accessed by CCNode::getChildByID(), CCNode::getChildByIDRecursive(), or CCNode::querySelector()) as seen on the screen.
	/// @param parent CCNode* whose children you want to access and take a screenshot of.
	/// @param querySelectorOrIDOrRecursive the string to be used. could be a node ID for CCNode::getChildByID(), could be a node ID for CCNode::getChildByIDRecursive(), or a querySelector for CCNode::querySelector().
	/// @param fetchType an enum of type PRNTSCRN::ReferenceType to determine if the `querySelectorOrIDOrRecursive` parameter is used for CCNode::getChildByID(), CCNode::getChildByIDRecursive(), or CCNode::querySelector().
	/// @returns a Result of Err if something is wrong with the parent node (null, lacks children nodes, querySelectorOrIDOrRecursive was empty, or node was not found), otherwise returns Result from calling PRNTSCRN::screenshotNode().
	/// @note if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
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
	/// @note - if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
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
	/// @note if the node being screenshotted is a PlayLayer or LevelEditorLayer node, the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
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
}