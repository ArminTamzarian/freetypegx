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
		u32 gsWidth;
		u32 gsHeight;
		
		void initializeGraphicsSystem(VideoSystem *videoSystem);

	public:
		GraphicsSystem(VideoSystem *videoSystem);
		void updateScene(u32 *frameBuffer);
};

#endif /*GRAPHICSSYSTEM_H_*/
