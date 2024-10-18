#include "RenderTexture.hpp"
#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MACOS

#define CommentType CommentType2
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#undef CommentType


bool CGImageWriteToFile(CGImageRef image, std::string name) {
    CFStringRef file = CFStringCreateWithCString(kCFAllocatorDefault,
    name.c_str(),
    kCFStringEncodingMacRoman);
    CFStringRef type = CFSTR("public.png");
    CFURLRef urlRef = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false );
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL( urlRef, geode::Mod::get()->getSavedValue<bool>("jpeg-mafia") ? kUTTypeJPEG : kUTTypePNG, 1, NULL );
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

void screenshot(std::unique_ptr<uint8_t[]> data, const cocos2d::CCSize& size, bool copy, const std::string& filename, unsigned int x, unsigned int y, unsigned int a, unsigned int b) {
    int width = size.width;
    int height = size.height;
    int dataLen = width * height * 4;

    GLubyte* newData = nullptr;
    newData = new GLubyte[width * width * 4];
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
        NULL,       // decode
        true,        // should interpolate
        kCGRenderingIntentDefault
    );

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        if (copy) {
            NSImage * image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(width, height)];

            if (image != nil) {
                NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
                [pasteboard clearContents];
                NSArray *copiedObjects = [NSArray arrayWithObject:image];
                [pasteboard writeObjects:copiedObjects];
            }
        } else {
            CGImageWriteToFile(cgImg, (geode::Mod::get()->getConfigDir() / filename).string());
        }
    });
}

#endif