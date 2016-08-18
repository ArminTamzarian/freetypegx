#ifndef VIDEOSYSTEM_H_
#define VIDEOSYSTEM_H_

#include <gccore.h>

#define FRAMEBUFFER_SIZE 2

class VideoSystem {
	private:
		GXRModeObj *videoMode;

		u32 *videoFramebuffer[FRAMEBUFFER_SIZE];
		u32 videoFrambufferIndex;

		void initializeVideoSystem();
		
	public:
		VideoSystem();
		GXRModeObj *getVideoMode();
		u32 *getVideoFramebuffer();
		void flipVideoFramebuffer();
};

#endif /*VIDEOSYSTEM_H_*/
