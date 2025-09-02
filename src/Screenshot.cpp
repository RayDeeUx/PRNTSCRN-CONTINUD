#include "Screenshot.hpp"
#include <thread>
#ifdef GEODE_IS_MOBILE
#include <prevter.imageplus/include/api.hpp>
#endif

using namespace geode::prelude;

class Screenshot::Impl {
public:
	uint32_t m_width;
	uint32_t m_height;
	CCNode* m_node;
	std::unique_ptr<uint8_t[]> m_data;

	Impl(uint32_t width, uint32_t height, CCNode* node) : m_width(width), m_height(height), m_node(node) {
		m_data = RenderTexture(width, height).capture(node);
	}
};

Screenshot::Screenshot(uint32_t width, uint32_t height, CCNode* node)
	: m_impl(std::make_shared<Impl>(width, height, node)) {}

Screenshot::Screenshot(const CCSize& size, CCNode* node)
	: m_impl(std::make_shared<Impl>(static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height), node)) {}

uint32_t Screenshot::getWidth() const { return m_impl->m_width; }
uint32_t Screenshot::getHeight() const { return m_impl->m_height; }
std::unique_ptr<uint8_t[]>& Screenshot::getData() const { return m_impl->m_data; }

#ifndef GEODE_IS_MACOS

// function formerly impl'd by ninxout, reimpl'd by prevter. former code preserved for posterity
void Screenshot::intoFile(const std::string& filename, bool isFromPRNTSCRNAndWantsSFX, bool jpeg) { // jpeg is unused because saveToFile handles automatically based on file extension. however it is being used in Screenshot.mm (macOS support)
	/*
	std::thread([=, data = std::move(m_data)]() {
		GLubyte* newData = nullptr;
		newData = new GLubyte[m_width * m_width * 4];
		for (int i = 0; i < m_height; ++i){
			memcpy(&newData[i * m_width * 4],
					&data.get()[(m_height - i - 1) * m_width * 4],
					m_width * 4);
		}

		Loader::get()->queueInMainThread([=](){
			CCImage* image = new CCImage();
			if (image->initWithImageData(newData, m_width * m_height * 4, CCImage::EImageFormat::kFmtRawData, m_width, m_height, 8)) {
				image->autorelease();
				image->saveToFile(filename.c_str(), true);
				if (isFromPRNTSCRNAndWantsSFX) FMODAudioEngine::get()->playEffect("screenshot_Windows_Android.mp3"_spr);
			}
			delete[] newData; // prevter caught this memleak --raydeeux
		});

	}).detach();
	*/
	std::thread([=, impl = m_impl]() {
		bool isOK = true;
		GLubyte* newData = new GLubyte[impl->m_width * impl->m_width * 4];
		for (int i = 0; i < impl->m_height; ++i) {
			memcpy(&newData[i * impl->m_width * 4],
					&impl->m_data.get()[(impl->m_height - i - 1) * impl->m_width * 4],
					impl->m_width * 4);
		}

		#ifdef GEODE_IS_WINDOWS
		CCImage image{};
		image.m_nBitsPerComponent = 8;
		image.m_nHeight = impl->m_height;
		image.m_nWidth = impl->m_width;
		image.m_bHasAlpha = true;
		image.m_bPreMulti = false;
		image.m_pData = newData;
		image.saveToFile(filename.c_str(), true);
		#elif defined(GEODE_IS_MOBILE)
		auto encodeResult = imgp::encode::png((void*)(newData), impl->m_width, impl->m_height);
		isOK = encodeResult.isOk();
		if (isOK) {
			auto writeBinaryResult = geode::utils::file::writeBinary(filename, encodeResult.unwrap());
			if (writeBinaryResult.isOk()) {
				log::info("binary write success!");
			} else log::error("binary write error! filename: {}, error: {}", filename, writeBinaryResult.unwrapErr());
		} else log::error("error: {}", encodeResult.unwrapErr());
		delete[] newData; // prevent memory leak (prevter)
		#endif

		if (isFromPRNTSCRNAndWantsSFX && isOK) {
			Loader::get()->queueInMainThread([](){
				auto system = FMODAudioEngine::get()->m_system;
				FMOD::Channel* channel;
				FMOD::Sound* sound;
				#ifdef GEODE_IS_IOS
				std::string customSound = "screenshot_macOS_iOS.mp3";
				#elif defined(GEODE_IS_ANDROID) || defined(GEODE_IS_WINDOWS)
				std::string customSound = "screenshot_Windows_Android.mp3";
				#endif
				system->createSound(geode::utils::string::pathToString((Mod::get()->getResourcesDir() / customSound)).c_str(), FMOD_DEFAULT, nullptr, &sound);
				system->playSound(sound, nullptr, false, &channel);
				channel->setVolume(85.f / 100.0f);
			});
		}
	}).detach();
}

#endif

#ifdef GEODE_IS_WINDOWS

void Screenshot::intoClipboard() {
	std::thread([impl = m_impl]() {
		auto width = impl->m_width;
		auto height = impl->m_height;

		BITMAPV5HEADER v5{};
		v5.bV5Size        = sizeof(BITMAPV5HEADER);
		v5.bV5Width       = width;
		v5.bV5Height      = height;
		v5.bV5Planes      = 1;
		v5.bV5BitCount    = 32;
		v5.bV5Compression = BI_BITFIELDS;
		v5.bV5RedMask     = 0x00FF0000;
		v5.bV5GreenMask   = 0x0000FF00;
		v5.bV5BlueMask    = 0x000000FF;
		v5.bV5AlphaMask   = 0xFF000000;
		v5.bV5CSType      = LCS_WINDOWS_COLOR_SPACE;
		v5.bV5Intent      = LCS_GM_IMAGES;
		v5.bV5SizeImage   = static_cast<DWORD>(width) * static_cast<DWORD>(height) * 4;

		auto headerSize = sizeof(BITMAPV5HEADER);
		auto pixelBytes = static_cast<SIZE_T>(width) * height * 4;
		auto totalSize  = headerSize + pixelBytes;

		auto hMem = GlobalAlloc(GMEM_MOVEABLE, totalSize);
		if (!hMem) {
			log::error("failed to allocate global memory");
			return;
		}

		void* base = GlobalLock(hMem);
		if (!base) {
			log::error("failed to lock global memory");
			GlobalFree(hMem);
			return;
		}

		std::memcpy(base, &v5, headerSize);
		{
			auto dst = static_cast<uint8_t*>(base) + headerSize;
			auto src = impl->m_data.get();
			auto n = static_cast<size_t>(width) * height;
			for (size_t i = 0; i < n; ++i) {
				dst[0] = src[2]; // B
				dst[1] = src[1]; // G
				dst[2] = src[0]; // R
				dst[3] = src[3]; // A
				dst += 4;
				src += 4;
			}
		}

		GlobalUnlock(hMem);

		if (!OpenClipboard(nullptr)) {
			log::error("failed to open clipboard");
			GlobalFree(hMem);
			return;
		}

		EmptyClipboard();

		if (!SetClipboardData(CF_DIBV5, hMem)) {
			log::error("failed to set clipboard data");
			GlobalFree(hMem);
			CloseClipboard();
			return;
		}

		// hMem is owned by clipboard, don't free it
		CloseClipboard();
	}).detach();
}

#endif