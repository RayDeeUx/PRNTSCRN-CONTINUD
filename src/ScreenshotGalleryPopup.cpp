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
	auto iterDir = asp::fs::iterDir(Mod::get()->getConfigDir());
	// if (iterDir.isErr()) log::info("FUCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
	for (auto fakeEntry : iterDir.unwrap()) {
		if (fakeEntry.isErr()) continue;
		auto entry = fakeEntry.unwrap();
		// geode::log::info("!entry.isDir(): {}", !entry.isDir());
		if (!entry.isDir()) continue;
		// geode::log::info("entry.path: {}", entry.path);
		auto entryIterDir = asp::fs::iterDir(entry.path);
		// if (entryIterDir.isErr()) log::info("SHITTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT");
		for (auto fakeNestedEntry : entryIterDir.unwrap()) {
			std::error_code ee, ef;
			if (fakeNestedEntry.isErr()) continue;
			auto nestedEntry = fakeNestedEntry.unwrap();
			// geode::log::info("nestedEntry.path: {}", nestedEntry.path);
			// geode::log::info("!nestedEntry.path.exists(): {}", !nestedEntry.path.exists());
			// geode::log::info("!nestedEntry.isFile(): {}", !nestedEntry.isFile());
			// geode::log::info("nestedEntry.isDir(): {}", nestedEntry.isDir());
			// geode::log::info("!nestedEntry.path.exists() || !nestedEntry.isFile() || nestedEntry.isDir(): {}", !nestedEntry.path.exists() || !nestedEntry.isFile() || nestedEntry.isDir());
			if (!nestedEntry.path.exists() || !nestedEntry.isFile() || nestedEntry.isDir()) continue;
			const asp::fs::Path& path = nestedEntry.path;
			// geode::log::info("path.extension().value(): {}", path.extension().value());
			if (path.extension().value() != "png" && path.extension().value() != "jpg") continue;
			// geode::log::info("adding it hopefully");
			const std::filesystem::path& stdPath = path.toStdPath().unwrapOr(std::filesystem::path{});
			// geode::log::info("path: {}", path);
			// geode::log::info("stdPath: {}", stdPath);
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