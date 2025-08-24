#include "Screenshot.hpp"
#include <thread>
#ifdef GEODE_IS_MOBILE
#include <prevter.imageplus/include/api.hpp>
#endif

using namespace geode::prelude;

Screenshot::Screenshot(unsigned int width, unsigned int height, CCNode* node) : m_width(width), m_height(height), m_node(node), m_tex(RenderTexture(width, height)) {
	m_data = m_tex.capture(node);
}

Screenshot::Screenshot(const CCSize& size, CCNode* node) : m_width(static_cast<int>(size.width)), m_height(static_cast<int>(size.height)), m_node(node), m_tex(RenderTexture(size.width, size.height))  {
	m_data = m_tex.capture(node);
}

CCTexture2D* Screenshot::intoTexture() {
	return m_tex.intoTexture();
}

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
	std::thread([=, width = std::move(m_width), height = std::move(m_height), data = std::move(m_data), isFromPRNTSCRNAndWantsSFXMoved = std::move(isFromPRNTSCRNAndWantsSFX)]() {
		log::info("vertically flipping bytevector");
		GLubyte* newData = new GLubyte[width * width * 4];
		for (int i = 0; i < height; ++i) {
			memcpy(&newData[i * width * 4],
					&data.get()[(height - i - 1) * width * 4],
					width * 4);
		}
		#ifdef GEODE_IS_WINDOWS
		CCImage image{};
		image.m_nBitsPerComponent = 8;
		image.m_nHeight = height;
		image.m_nWidth = width;
		image.m_bHasAlpha = true;
		image.m_bPreMulti = false;
		image.m_pData = newData;
		image.saveToFile(filename.c_str(), true);
		#elif defined(GEODE_IS_MOBILE)
		log::info("calling prevter's image API");
		auto encodeResult = imgp::encode::png((void*)(newData), width, height);
		const bool isOK = encodeResult.isOk();
		if (isOK) {
			log::info("encoding success! writing binary");
			auto writeBinaryResult = geode::utils::file::writeBinary(filename, std::move(encodeResult).unwrap());
			if (writeBinaryResult.isOk()) {
				log::info("binary write success!");
			} else log::error("binary write error! filename: {}, error: {}", filename, writeBinaryResult.unwrapErr());
		} else log::error("error: {}", encodeResult.unwrapErr());
		delete[] newData; // prevent memory leak (prevter)
		log::info("memory leak prevented by prevter :fire:");
		#endif
		#ifdef GEODE_IS_WINDOWS
		if (isFromPRNTSCRNAndWantsSFXMoved) {
		#elif defined(GEODE_IS_MOBILE)
		log::info("checking for isFromPRNTSCRNAndWantsSFX");
		if (isFromPRNTSCRNAndWantsSFXMoved && isOK) {
		#endif
			log::info("queuing SFX");
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
	std::thread([=, width = std::move(m_width), height = std::move(m_height), data = std::move(m_data)]() {
		auto bitmap = CreateBitmap(width, height, 1, 32, data.get());

		if (OpenClipboard(NULL)) {
			if (EmptyClipboard()) {
				SetClipboardData(CF_BITMAP, bitmap);
				CloseClipboard();
			}
		}
	}).detach();
}

#endif
