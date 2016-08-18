#ifndef VIDEOSYSTEM_H_
#define VIDEOSYSTEM_H_

#include <gccore.h>

#define FRAMEBUFFER_SIZE 2

class VideoSystem {
	private:
		GXRModeObj *videoMode;

		uint32_t *videoFramebuffer[FRAMEBUFFER_SIZE];
		uint32_t videoFrambufferIndex;

		void initializeVideoSystem();
		
	public:
		VideoSystem();
		GXRModeObj *getVideoMode();
		uint32_t *getVideoFramebuffer();
		void flipVideoFramebuffer();
};

#endif /*VIDEOSYSTEM_H_*/
