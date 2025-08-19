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

	// querySelectorsToHide are node IDs that are direct children of the node chosen as the screenshot's target.
	// PRNTSCRN will call node->querySelector() to find the node you want to hide.
	// if you do not feel comfortable using this, use PRNTSCRN::screenshotNode instead.
	// for more info, see https://docs.geode-sdk.org/classes/cocos2d/CCNode#querySelector.
	inline geode::Result<> screenshotNodeAdvanced(CCNode* node, std::vector<CCNode*> pointersToHide, std::vector<std::string> querySelectorsToHide) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNodeAdvanced"));
		}
		ScreenshotEvent(node, pointersToHide, querySelectorsToHide).post();
		return Ok();
	}

	inline geode::Result<> screenshotNode(CCNode* node) {
		if (!node) {
			log::error("[PRNTSCRN API] unable to reference node from screenshotNode");
			return Err(fmt::format("[PRNTSCRN API] unable to reference node from screenshotNode"));
		}
		ScreenshotEvent(node).post();
		return Ok();
	}

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