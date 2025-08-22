#include "Screenshot.hpp"
#include <thread>

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

#ifndef __APPLE__

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
	std::thread([=, this, data = std::move(m_data)]() {
		log::info("entered thread, initalizing newData with [m_width * m_width * 4]");
		GLubyte* newData = new GLubyte[m_width * m_width * 4];
		log::info("entering forloop");
		for (int i = 0; i < m_height; ++i){
			log::info("i: {}", i);
			memcpy(&newData[i * m_width * 4],
					&data.get()[(m_height - i - 1) * m_width * 4],
					m_width * 4);
		}
		log::info("making new CCImage named image");
		CCImage image{};
		log::info("setting m_nBitsPerComponent");
		image.m_nBitsPerComponent = 8;
		log::info("setting m_nHeight");
		image.m_nHeight = m_height;
		log::info("setting m_nWidth");
		image.m_nWidth = m_width;
		log::info("setting m_bHasAlpha");
		image.m_bHasAlpha = true;
		log::info("setting m_bPreMulti");
		image.m_bPreMulti = false;
		log::info("setting m_pData");
		image.m_pData = newData;
		log::info("saving image to file");
		image.saveToFile(filename.c_str(), true);
		log::info("checking isFromPRNTSCRNAndWantsSFX's value");
		if (isFromPRNTSCRNAndWantsSFX) {
			log::info("queuing SFX");
			Loader::get()->queueInMainThread([](){
				FMODAudioEngine::get()->playEffect("screenshot_Windows_Android.mp3"_spr);
			});
		}
		log::info("leaving thread");
	}).detach();
}

#endif

#ifdef GEODE_IS_WINDOWS

void Screenshot::intoClipboard() {
	std::thread([=, data = std::move(m_data)]() {
		auto bitmap = CreateBitmap(m_width, m_height, 1, 32, data.get());

		if (OpenClipboard(NULL)) {
			if (EmptyClipboard()) {
				SetClipboardData(CF_BITMAP, bitmap);
				CloseClipboard();
			}
		}
	}).detach();
}

#endif