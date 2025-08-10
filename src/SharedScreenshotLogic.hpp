#pragma once

using namespace geode::prelude;

namespace SharedScreenshotLogic {

	void screenshot(CCNode* node);

	constexpr std::array monthNames = {
		"Unknown",
		"January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December",
		"Unknown"
	};

}