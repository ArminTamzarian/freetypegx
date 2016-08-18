#ifndef GRAPHICSSYSTEM_H_
#define GRAPHICSSYSTEM_H_

#include <gccore.h>
#include <malloc.h>
#include <string.h>
#include "VideoSystem.h"

#define DEFAULT_FIFO_SIZE (1024*1024)

class GraphicsSystem {
	private:
		void* gsFifo;
		uint32_t gsWidth;
		uint32_t gsHeight;
		
		void initializeGraphicsSystem(VideoSystem *videoSystem);

	public:
		GraphicsSystem(VideoSystem *videoSystem);
		void updateScene(uint32_t *frameBuffer);
};

#endif /*GRAPHICSSYSTEM_H_*/
