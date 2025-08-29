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

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	std::unique_ptr<uint8_t[]>& getData() const;

private:
	class Impl;
	std::shared_ptr<Impl> m_impl;
};