#pragma once

#include <Geode/Geode.hpp>
#include <memory>
// code originally by mat, modified by raydeeux with help from prevter to remove all mentions of BGRA for android compat

class RenderTexture {
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	GLint m_oldFBO = 0;
	GLint m_oldRBO = 0;
	GLuint m_fbo = 0;
	GLuint m_depthStencil = 0;
	GLuint m_texture = 0;
	float m_oldScaleX = 0;
	float m_oldScaleY = 0;
public:
	RenderTexture(unsigned width, unsigned height);
	~RenderTexture();

	enum class PixelFormat {
		// RGB,
		// BGR,
		RGBA // ,
		// BGRA
	};

	// begin, visit, end a node
	std::unique_ptr<uint8_t[]> capture(cocos2d::CCNode* node, PixelFormat format = PixelFormat::RGBA);

	void begin();
	void end();

	// must be called before end because my code sucks
	std::unique_ptr<uint8_t[]> readDataFromTexture(PixelFormat format = PixelFormat::RGBA);

	GLuint getTexture() const { return m_texture; }
	
	// creates a CCTexture2D from the internal texture. this class should not be used after this
	// as cocos now owns the texture, and a new one isnt created
	cocos2d::CCTexture2D* intoTexture();

	void applyWinSize();
	void restoreWinSize();
};

void screenshot(std::unique_ptr<uint8_t[]> data, const cocos2d::CCSize& size, bool copy, const std::string& filename, unsigned int x = 0, unsigned int y = 0, unsigned int a = 0, unsigned int b = 0);