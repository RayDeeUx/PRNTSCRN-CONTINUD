#include "RenderTexture.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Screenshot {
public:
	Screenshot(unsigned int width, unsigned int height, CCNode* node);
	Screenshot(const CCSize& size, CCNode* node);

	void intoFile(const std::string& filename, bool isFromPRNTSCRNAndWantsSFX, bool jpeg);
	#ifdef GEODE_IS_DESKTOP
	void intoClipboard();
	#endif
	CCTexture2D* intoTexture();

private:
	unsigned int m_width;
	unsigned int m_height;
	CCNode* m_node;
	RenderTexture m_tex;
	std::unique_ptr<uint8_t[]> m_data;
};