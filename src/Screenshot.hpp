#include "RenderTexture.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Screenshot {
public:
	Screenshot(unsigned int width, unsigned int height, CCNode* node);

	void intoFile(const std::string& filename, bool jpeg);
	void intoClipboard();
	CCTexture2D* intoTexture();

private:
	unsigned int m_width;
	unsigned int m_height;
	RenderTexture m_tex;
	CCNode* m_node;
	std::unique_ptr<uint8_t[]> m_data;
};