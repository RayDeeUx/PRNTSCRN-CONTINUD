#pragma once

using namespace geode::prelude;

class ScreenshotEvent : public Event {
protected:
	CCNode* nodeToScreenshot {};
public:
	explicit ScreenshotEvent(CCNode* node) : nodeToScreenshot(node) {}
	[[nodiscard]] CCNode* getNode() const { return nodeToScreenshot; }
};

namespace PRNTSCRN { // Pretty Rad (and) Nifty Tool (to) Screen Capture Right Now
	inline void screenshotNode(CCNode* node) {
		ScreenshotEvent(node).post();
	}
}