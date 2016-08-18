#include "VideoSystem.h"

VideoSystem::VideoSystem() {
	this->videoFrambufferIndex = 0;
	this->initializeVideoSystem();
}

void VideoSystem::initializeVideoSystem() {

	VIDEO_Init();
	
	this->videoMode = VIDEO_GetPreferredMode(NULL);

	for(uint8_t videoIndex = 0; videoIndex < FRAMEBUFFER_SIZE; videoIndex++) {
		this->videoFramebuffer[videoIndex] = (uint32_t *)MEM_K0_TO_K1(SYS_AllocateFramebuffer(this->videoMode));
		VIDEO_ClearFrameBuffer(this->videoMode, this->videoFramebuffer[videoIndex], COLOR_BLACK);
	}
	this->videoFrambufferIndex = 0;
	
	VIDEO_Configure(this->videoMode);
	VIDEO_SetNextFramebuffer(this->videoFramebuffer[0]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	
	if(this->videoMode->viTVMode & VI_NON_INTERLACE) {
		VIDEO_WaitVSync();
	}
}
GXRModeObj *VideoSystem::getVideoMode() {
	return this->videoMode;
}

uint32_t *VideoSystem::getVideoFramebuffer() {
	return this->videoFramebuffer[this->videoFrambufferIndex];
}

void VideoSystem::flipVideoFramebuffer() {

	VIDEO_SetNextFramebuffer(this->getVideoFramebuffer());

	VIDEO_Flush();
	VIDEO_WaitVSync();

	this->videoFrambufferIndex ^= 1;
}
