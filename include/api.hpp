#pragma once

using namespace geode::prelude;

namespace PRNTSCRN { // Pretty Rad (and) Nifty Tool (to) Screen Capture Right Now
	enum ReferenceType {
		ByID = 0,
		ByIDRecursive = 1,
		ByQuerySelector = 2
	};

    class ScreenshotEvent final : public Event {
	    protected:
	        CCNode* nodeToScreenshot {};
	    public:
	        explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {}
	        [[nodiscard]] CCNode* getNode() const { return nodeToScreenshot; }
    };

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
		if (!node) return log::error("[PRNTSCRN API] unable to reference node of type {} and index {} using screenshotNodeByTypeFrom", typeid(T).name(), index);
		PRNTSCRN::screenshotNode(node);
	}
}