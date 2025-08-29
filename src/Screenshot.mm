#include "Screenshot.hpp"
#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MACOS

#define CommentType CommentType2
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#undef CommentType

bool CGImageWriteToFile(CGImageRef image, const std::string& name, bool jpeg) {
	CFStringRef file = CFStringCreateWithCString(kCFAllocatorDefault,
	name.c_str(),
	kCFStringEncodingMacRoman);
	CFStringRef type = CFSTR("public.png");
	CFURLRef urlRef = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false );
	CGImageDestinationRef destination = CGImageDestinationCreateWithURL( urlRef, jpeg ? kUTTypeJPEG : kUTTypePNG, 1, NULL );
	CGImageDestinationAddImage( destination, image, NULL );
	CGImageDestinationFinalize( destination );
	if (!destination) {
		return false;
	}

	CGImageDestinationAddImage(destination, image, nil);

	if (!CGImageDestinationFinalize(destination)) {
		CFRelease(destination);
		return false;
	}

	CFRelease(destination);
	return true;
}

void Screenshot::intoFile(const std::string& filename, bool isFromPRNTSCRNAndWantsSFX, bool jpeg) {
    auto width = this->getWidth();
    auto height = this->getHeight();
    auto& data = this->getData();
	int dataLen = width * height * 4;

	GLubyte* newData = nullptr;
	newData = new GLubyte[dataLen];
	for (int i = 0; i < height; ++i){
		memcpy(&newData[i * width * 4],
				&data[(height - i - 1) * width * 4],
				width * 4);
	}

	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, newData, dataLen, NULL);

	CGImageRef cgImg = CGImageCreate(
		width, height,
		8, 8 * 4, width * 4,
		CGColorSpaceCreateDeviceRGB(), 
		kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast, 
		provider,   // data provider
		NULL,	   // decode
		true,		// should interpolate
		kCGRenderingIntentDefault
	);

	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		CGImageWriteToFile(cgImg, filename, jpeg);
		if (isFromPRNTSCRNAndWantsSFX) {
		    auto system = FMODAudioEngine::get()->m_system;
			FMOD::Channel* channel;
			FMOD::Sound* sound;
			system->createSound(geode::utils::string::pathToString((Mod::get()->getResourcesDir() / "screenshot_macOS_iOS.mp3")).c_str(), FMOD_DEFAULT, nullptr, &sound);
			system->playSound(sound, nullptr, false, &channel);
			channel->setVolume(85.f / 100.0f);
		}
	});
}

void Screenshot::intoClipboard() {
    auto width = this->getWidth();
    auto height = this->getHeight();
    auto& data = this->getData();
	int dataLen = width * height * 4;

	GLubyte* newData = nullptr;
	newData = new GLubyte[dataLen];
	for (int i = 0; i < height; ++i){
		memcpy(&newData[i * width * 4],
				&data[(height - i - 1) * width * 4],
				width * 4);
	}

	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, newData, dataLen, NULL);

	CGImageRef cgImg = CGImageCreate(
		width, height,
		8, 8 * 4, width * 4,
		CGColorSpaceCreateDeviceRGB(), 
		kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast, 
		provider,   // data provider
		NULL,	   // decode
		true,		// should interpolate
		kCGRenderingIntentDefault
	);

	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		NSImage* image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(width, height)];

		if (image != nil) {
			NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
			[pasteboard clearContents];
			NSArray *copiedObjects = [NSArray arrayWithObject:image];
			[pasteboard writeObjects:copiedObjects];
		}
	});
}

#endif