#include "ScreenshotGalleryPopup.hpp"
#include <asp/fs/std.hpp>
#include <UIBuilder.hpp>
#include <asp/fs/fs.hpp>

using namespace geode::prelude;

bool ScreenshotGalleryPopup::init() {
	constexpr float popupWidth = 360.f;
	if (!geode::Popup::init(popupWidth, 270.f, "GJ_square02.png")) return false;

	this->setTitle("PRNTSCRN Gallery");
	this->setID("ScreenshotGalleryPopup"_spr);
	this->m_title->setID("title"_spr);

	this->m_scrollLayer = Build<geode::ScrollLayer>::create(CCSize{popupWidth - 40.f, 225.f}).id("scroll-layer"_spr).ignoreAnchorPointForPos(false);
	this->m_mainLayer->addChildAtPosition(this->m_scrollLayer, geode::Anchor::Center, {0.f, -15.f});

	std::vector<ScreenshotInfo> images = {};
	for (auto fakeEntry : asp::fs::DirectoryIterator::create(Mod::get()->getConfigDir()).unwrap()) {
		std::error_code ea;
		if (!fakeEntry.isErr()) continue;
		auto entry = fakeEntry.unwrap();
		if (!entry.isDir()) continue;
		for (auto fakeNestedEntry : asp::fs::DirectoryIterator::create(entry.path).unwrap()) {
			std::error_code eb, ec, ed, ee, ef;
			if (!fakeNestedEntry) continue;
			auto nestedEntry = fakeNestedEntry.unwrap();
			if (!nestedEntry.path.exists() || !nestedEntry.isFile() || nestedEntry.isDir()) continue;
			asp::fs::Path& path = nestedEntry.path;
			if (path.extension().value() != ".png" && path.extension().value() != ".jpg") continue;
			const std::filesystem::path& stdPath = path.toStdPath().unwrapOr(std::filesystem::path{});
			images.push_back({
				.path = stdPath,
				.lastWriteTime = geode::localtime(std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(GEODE_WINDOWS(std::chrono::clock_cast<std::chrono::system_clock>)GEODE_MOBILE(std::filesystem::file_time_type::clock::to_sys)GEODE_MACOS(std::filesystem::file_time_type::clock::to_sys)(std::filesystem::last_write_time(stdPath, ee))))),
				.lastWriteTimeInRawForm = std::filesystem::last_write_time(stdPath, ef)
			});
		}
	}

	std::sort(images.begin(), images.end(), [](ScreenshotInfo& a, ScreenshotInfo& b) {
		return a.lastWriteTimeInRawForm > b.lastWriteTimeInRawForm;
	});

	int i = 0, rowCount = 0, totalImageCount = 0;
	CCNode* row = CCNode::create();
	row->setContentSize({320.f, 105.f});

	for (const ScreenshotInfo& image : images) {
		CCNode* theScreenshotNode = reinterpret_cast<CCNode*>(PRNTSCRNScreenshot::create(image));
		if (!theScreenshotNode) continue;

		theScreenshotNode->setID(fmt::format("screenshot-{}"_spr, i + 1));
		theScreenshotNode->setTag(totalImageCount + 1);
		row->addChild(theScreenshotNode);

		i++;
		totalImageCount++;
		if (i < 5) continue;

		row->setLayout(RowLayout::create()->setGap(10.f)->setAxisReverse(false)->setGrowCrossAxis(false));
		row->setID(fmt::format("screenshots-row-{}"_spr, rowCount + 1));

		this->m_scrollLayer->m_contentLayer->addChild(row);

		row = CCNode::create();
		row->setContentSize({320.f, 105.f});

		i = 0;
		rowCount++;
	}

	if (!row->getParent() && row->getChildrenCount() > 0) {
		for (int j = row->getChildrenCount(); j < 5; j++) {
			CCNode* dummy = CCNode::create();
			dummy->setContentSize({85.f, 105.f});
			row->addChild(dummy, 10);
		}
		row->setLayout(RowLayout::create()->setGap(20.f)->setAxisReverse(false)->setGrowCrossAxis(false));
		row->setID(fmt::format("screenshots-row-{}"_spr, rowCount + 1));
		this->m_scrollLayer->m_contentLayer->addChild(row);
	}

	this->m_scrollLayer->m_contentLayer->setLayout(Build<geode::ColumnLayout>::create()->setGap(10.f)->ignoreInvisibleChildren(false)->setAxisReverse(true)->setAutoGrowAxis(std::make_optional<float>(0.f)));

	this->m_scrollLayer->scrollToTop();

	return true;
}

ScreenshotGalleryPopup* ScreenshotGalleryPopup::create() {
	auto ret = new ScreenshotGalleryPopup;
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}