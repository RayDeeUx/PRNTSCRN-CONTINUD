#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

struct ScreenshotInfo {
	std::filesystem::path path {};
	std::tm lastWriteTime {};
	std::filesystem::file_time_type lastWriteTimeInRawForm {};
};

class PRNTSCRNScreenshot final : cocos2d::CCNode {
public:
	static PRNTSCRNScreenshot* create(const ScreenshotInfo& screenshotInfo) {
		auto* ret = new PRNTSCRNScreenshot();
		if (ret->init(screenshotInfo)) {
			ret->autorelease();
			return ret;
		}
		return ret;
	}

	bool init(const ScreenshotInfo& screenshotInfo) {
		if (!CCNode::init()) return false;

		this->m_screenshotInfo = screenshotInfo;

		this->m_screenshot = geode::Ref<geode::LazySprite>(geode::LazySprite::create({85.f, 85.f}));
		this->m_screenshot->loadFromFile(screenshotInfo.path);

		this->m_screenshot->setLoadCallback([screenshot = geode::Ref(this->m_screenshot)](const geode::Result<>& result) {
			if (!result.isErr()) {
				screenshot->setScale(85.f / screenshot->getContentWidth());
			}
		});
		this->m_screenshot->addOnEnterCallback([screenshot = geode::Ref(this->m_screenshot)]() {
			screenshot->setScale(85.f / screenshot->getContentWidth());
		});

		this->m_screenshot->setID("screenshot-sprite"_spr);

		this->setContentSize({85.f, 105.f});
		this->addChildAtPosition(this->m_screenshot, geode::Anchor::Center);

		this->m_fileNameLabel = geode::Label::create(fmt::format("{} - {}", geode::utils::string::pathToString(screenshotInfo.path.parent_path().stem()), geode::utils::string::pathToString(screenshotInfo.path.stem())), "bigFont.fnt");
		this->m_fileNameLabel->setLimitLabelSize({80.f, 32.f});
		this->m_fileNameLabel->setID("file-name-label"_spr);
		this->addChildAtPosition(this->m_fileNameLabel, geode::Anchor::Top, {0.f, -12.5f});

		this->m_fileDateLabel = geode::Label::create(fmt::format("{:%A, %B} {}, {:%Y %H:%M}", screenshotInfo.lastWriteTime, screenshotInfo.lastWriteTime.tm_mday, screenshotInfo.lastWriteTime), "chatFont.fnt");
		this->m_fileDateLabel->setLimitLabelSize({80.f, 32.f});
		this->m_fileDateLabel->setID("file-date-label"_spr);
		this->addChildAtPosition(this->m_fileDateLabel, geode::Anchor::Bottom, {0.f, 12.5f});

		return true;
	}
protected:
	ScreenshotInfo m_screenshotInfo {};
	geode::Ref<geode::LazySprite> m_screenshot {};
	geode::Label* m_fileNameLabel {};
	geode::Label* m_fileDateLabel {};
};

class ScreenshotGalleryPopup final : public geode::Popup {
public:
	static ScreenshotGalleryPopup* create();
protected:
	geode::ScrollLayer* m_scrollLayer {};

	bool init() override;
};