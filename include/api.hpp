#pragma once

using namespace geode::prelude;

namespace PRNTSCRN { // Pretty Rad (and) Nifty Tool (to) Screen Capture Right Now
    class ScreenshotEvent final : public Event {
	    protected:
	        CCNode* nodeToScreenshot {};
	    public:
	        explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {}
	        [[nodiscard]] CCNode* getNode() const { return nodeToScreenshot; }
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

	inline void screenshotNode(CCNode* node) {
		if (!node) return log::error("[PRNTSCRN API] unable to reference node from screenshotNode");
		ScreenshotEvent(node).post();
	}

	inline void screenshotNodeUsingStringFrom(CCNode* parent, const std::string_view querySelectorOrIDOrRecursive, ReferenceType fetchType) {
		if (!parent) return log::error("[PRNTSCRN API] unable to reference parent node");
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
		if (!node) return log::error("[PRNTSCRN API] unable to reference node with string {} using ReferenceType mode {}", querySelectorOrIDOrRecursive, static_cast<int>(fetchType));
		PRNTSCRN::screenshotNode(node);
	}

	inline void screenshotNodeUsingTagFrom(CCNode* parent, int tag) {
		if (!parent) return log::error("[PRNTSCRN API] unable to reference parent node");
		if (tag < 0) return log::error("[PRNTSCRN API] unable to fetch child by tag, tag {} is less than 0", tag);
		CCNode* node = parent->getChildByTag(tag);
		if (!node) return log::error("[PRNTSCRN API] unable to reference node using screenshotNodeUsingTagFrom with tag {}", tag);
		PRNTSCRN::screenshotNode(node);
	}

	template<class T>
	void screenshotNodeByTypeFrom(CCNode* parent, int index) {
		if (!parent) return log::error("[PRNTSCRN API] unable to reference parent node using screenshotNodeByTypeFrom");
		CCNode* node = parent->getChildByType<T>(index);
		if (!node) return log::error("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", PRNTSCRN::__demangle__(typeid(T).name()), index);
		PRNTSCRN::screenshotNode(node);
	}
}