#include "RenderTexture.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// #include <CCGL.h>
#include <thread>

#ifdef GEODE_IS_WINDOWS

using uint = unsigned int;

void copy_screenshot(std::unique_ptr<uint8_t[]> data, const CCSize& size, bool copy, uint x, uint y, uint a, uint b) {
    const auto src_width = static_cast<uint>(size.width);
    const auto src_height = static_cast<uint>(size.height);
    a = a ? a : src_width;
    b = b ? b : src_height;
    std::thread([=, data = std::move(data)]() {
        if (copy) {
			auto bitmap = CreateBitmap((int)size.width, (int)size.height, 1, 32, data.get());

			if (OpenClipboard(NULL)) {
				if (EmptyClipboard()) {
					SetClipboardData(CF_BITMAP, bitmap);
					CloseClipboard();
				}
			}
		} else {
			CCImage* image = new CCImage();
			std::string filepath = (geode::Mod::get()->getConfigDir() / "test.png").string();
			image->initWithImageData((void*)data, (int)size.width * (int)size.height * 4, CCImage::EImageFormat::kFmtPng, (int)size.width, (int)size.height, 8);
			image->saveToFile(filepath.c_str(), true);
		}
    }).detach();
}

#endif

#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat) {
		if (down && key == enumKeyCodes::KEY_F2) {
			auto director = CCDirector::sharedDirector();
			auto winSize = director->getWinSize();
			auto scene = director->getRunningScene();

			auto captureSize = CCSize(1920, 1080);
			RenderTexture texture(captureSize.width, captureSize.height);
			auto data = texture.capture(scene);

			auto* ctexture = texture.intoTexture();

			screenshot(std::move(data), captureSize, false);
		}
		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
	}
};

#include <Geode/modify/PauseLayer.hpp>
#include "ScreenshotPopup.hpp"
class $modify(NewPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		
		auto btn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("screenshot.png"_spr),
			this,
			menu_selector(NewPauseLayer::onScreenshotPopup)
		);
		static_cast<CCMenu*>(getChildByID("left-button-menu"))->addChild(btn);
		static_cast<CCMenu*>(getChildByID("left-button-menu"))->updateLayout();
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};