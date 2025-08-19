#pragma once

using namespace geode::prelude;

namespace PRNTSCRN { // Pretty Rad (and) Nifty Tool (to) Screen Capture Right Now
    class ScreenshotEvent final : public Event {
	    protected:
	        CCNode* nodeToScreenshot {};
    		std::vector<CCNode*> nodePointersToHide {};
    		std::vector<std::string> querySelectorsToHide  {};
	    public:
    		explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {
    			nodePointersToHide = {};
    			querySelectorsToHide = {};
    		}
    	    explicit ScreenshotEvent(CCNode* node, std::vector<CCNode*> pointers) : nodeToScreenshot(node) {
    			nodePointersToHide = pointers;
    			querySelectorsToHide = {};
    		}
    	    explicit ScreenshotEvent(CCNode* node, std::vector<std::string> querySelectors) : nodeToScreenshot(node) {
    			nodePointersToHide = {};
    			querySelectorsToHide = querySelectors;
    		}
    		explicit ScreenshotEvent(CCNode* node, std::vector<CCNode*> pointers, std::vector<std::string> querySelectors) : nodeToScreenshot(node) {
    			nodePointersToHide = pointers;
    			querySelectorsToHide = querySelectors;
    		}
    		explicit ScreenshotEvent(CCNode* node, std::vector<std::string> querySelectors, std::vector<CCNode*> pointers) : nodeToScreenshot(node) {
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

	// THIS FUNCTION IS FOR LOGGING PURPOSES ONLY.
	// YOU SHOULD NOT NEED THIS IN 99.9% OF CASES.
	// THE DOUBLE UNDERSCORES SHLD ILLUSTRATE THAT
	// code happily reused from geode-sdk/DevTools
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

	// screenshot a node of your choice with custom visibility filters.
	// if the node being screenshotted is a PlayLayer or LevelEditorLayer node,
	// you WILL need to specify your own nodes to hide--the user's preferences in PRNTSCRN's settings
	// for hiding PlayLayer/LevelEditor UI or the player will be ignored--that is the whole purpose of this function!
	// pointersToHide are pointers to any variable that inherits CCNode*. one example is PlayLayer::get()->m_player1.
	// querySelectorsToHide are node IDs that are direct decendants of the node chosen as the screenshot's target.
	// PRNTSCRN will call node->querySelector() to find the node you want to hide.
	// if you do not feel comfortable using this option to hide nodes by pointer or by querySelector,
	// you are responsible for hiding specific nodes on your own, using PRNTSCRN::screenshotNode instead,
	// and manually restoring the visibility states of those nodes you chose to hide.
	// for more info on querySelector, see https://docs.geode-sdk.org/classes/cocos2d/CCNode#querySelector.
	inline geode::Result<> screenshotNodeAdvanced(CCNode* node, std::vector<CCNode*> pointersToHide, std::vector<std::string> querySelectorsToHide) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced"));
		}
		ScreenshotEvent(node, pointersToHide, querySelectorsToHide).post();
		return Ok();
	}

	// screenshot a node as seen on the screen. when screenshotting a PlayLayer or LevelEditorLayer node,
	// the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
	// Examples:
	/*
	PRNTSCRN::screenshotNode(PlayLayer::get()->m_uiLayer);
	PRNTSCRN::screenshotNode(LevelEditorLayer::get()->getChildByIDRecursive("main-node"));
	PRNTSCRN::screenshotNode(CCScene::get()->getChildByType<LevelInfoLayer>(0)->getChildByID("right-side-menu"));
	*/
	inline geode::Result<> screenshotNode(CCNode* node) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNode");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNode"));
		}
		ScreenshotEvent(node).post();
		return Ok();
	}

	// screenshot a node as seen on the screen. use the func's third parameter (PRNTSCRN::ReferenceType enum type)
	// to choose how you want to fetch the node. possible enum values are ByID, ByIDRecursive, or ByQuerySelector.
	// if the node being screenshotted is a PlayLayer or LevelEditorLayer node,
	// the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
	// Examples:
	/*
	PRNTSCRN::screenshotNodeUsingStringFrom(PlayLayer::get(), "UILayer", PRNTSCRN::ReferenceType::ByID);
	PRNTSCRN::screenshotNodeUsingStringFrom(LevelEditorLayer::get(), "main-node", PRNTSCRN::ReferenceType::ByIDRecursive);
	PRNTSCRN::screenshotNodeUsingStringFrom(CCScene::get(), "LevelInfoLayer > right-side-menu", PRNTSCRN::ReferenceType::ByQuerySelector);
	*/
	inline geode::Result<> screenshotNodeUsingStringFrom(CCNode* parent, const std::string_view querySelectorOrIDOrRecursive, ReferenceType fetchType) {
		if (!parent) {
		    log::error("[PRNTSCRN API] unable to reference parent node");
		    return Err(fmt::format("[PRNTSCRN API] unable to reference parent node"));
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

	// screenshot a node as seen on the screen. per cocos spec, the `tag` parameter *must* be greater than -1.
	// there is no recursive variant of this!!! the node you want to screenshot with the specified tag must be
	// a direct child of the `parent` parameter.
	// if the node being screenshotted is a PlayLayer or LevelEditorLayer node,
	// the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
	// Examples:
	/*
	PRNTSCRN::screenshotNodeUsingStringFrom(PlayLayer::get(), 32);
	PRNTSCRN::screenshotNodeUsingStringFrom(LevelEditorLayer::get(), 32);
	PRNTSCRN::screenshotNodeUsingStringFrom(CCScene::get(), 32);
	*/
	inline geode::Result<> screenshotNodeUsingTagFrom(CCNode* parent, int tag) {
		if (!parent) {
		    log::error("[PRNTSCRN API] unable to reference parent node");
		    return Err(fmt::format("[PRNTSCRN API] unable to reference parent node"));
		}
		if (tag < 0) {
		    log::error("[PRNTSCRN API] unable to fetch child by tag, tag {} is less than 0", tag);
		    return Err(fmt::format("[PRNTSCRN API] unable to fetch child by tag, tag {} is less than 0", tag));
		}
		CCNode* node = parent->getChildByTag(tag);
		if (!node) {
		    log::error("[PRNTSCRN API] unable to reference node using screenshotNodeUsingTagFrom with tag {}", tag);
		    return Err(fmt::format("[PRNTSCRN API] unable to reference node using screenshotNodeUsingTagFrom with tag {}", tag));
		}
		return PRNTSCRN::screenshotNode(node);
	}

	// screenshot a node as seen on the screen. the syntax is similar to geode's CCNode::getChildByType<T>(index).
	// if the node being screenshotted is a PlayLayer or LevelEditorLayer node,
	// the user's personal preferences for hiding the UI or the player in PRNTSCRN's settings will apply.
	// Examples:
	/*
	PRNTSCRN::screenshotNodeUsingStringFrom(PlayLayer::get(), 32);
	PRNTSCRN::screenshotNodeUsingStringFrom(LevelEditorLayer::get(), 32);
	PRNTSCRN::screenshotNodeUsingStringFrom(CCScene::get(), 32);
	*/
	template<class T>
	geode::Result<> screenshotNodeByTypeFrom(CCNode* parent, int index) {
		if (!parent) {
		    log::error("[PRNTSCRN API] unable to reference parent node using screenshotNodeByTypeFrom");
		    return Err(fmt::format("[PRNTSCRN API] unable to reference parent node using screenshotNodeByTypeFrom"));
		}
		CCNode* node = parent->getChildByType<T>(index);
		if (!node) {
		    log::error("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", PRNTSCRN::__demangle__(typeid(T).name()), index);
		    return Err(fmt::format("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", PRNTSCRN::__demangle__(typeid(T).name()), index));
		}
		return PRNTSCRN::screenshotNode(node);
	}
}