#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem(VideoSystem *videoSystem) {
	this->initializeGraphicsSystem(videoSystem);
}

void GraphicsSystem::initializeGraphicsSystem(VideoSystem *videoSystem) {

	GXRModeObj *videoMode = videoSystem->getVideoMode();

	// Init GX
	this->gsFifo = memalign(32, DEFAULT_FIFO_SIZE);
	memset(this->gsFifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init(this->gsFifo, DEFAULT_FIFO_SIZE);

	// Set our background
	GXColor background = {0x00, 0x00, 0xaa, 0x00};
	GX_SetCopyClear(background, 0x00ffffff);

	// Set up the display
	f32 yscale = 0; u32 xfbHeight = 0;
	yscale = GX_GetYScaleFactor(videoMode->efbHeight, videoMode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetViewport(0, 0,videoMode->fbWidth,videoMode->efbHeight, 0, 1);
	GX_SetScissor(0, 0, videoMode->fbWidth, videoMode->efbHeight);
	GX_SetDispCopySrc(0, 0, videoMode->fbWidth, videoMode->efbHeight);
	GX_SetDispCopyDst(videoMode->fbWidth, xfbHeight);
	GX_SetCopyFilter(videoMode->aa, videoMode->sample_pattern, GX_TRUE, videoMode->vfilter);

	// Use these values for GetWidth() and GetHeight()
	this->gsWidth = (u32)videoMode->fbWidth;
	this->gsHeight = (u32)videoMode->efbHeight;
	
	// Some additional Init code
	GX_SetFieldMode(videoMode->field_rendering, ((videoMode->viHeight == 2 * videoMode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

	if(videoMode->aa){
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	}else{
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	}

	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(videoSystem->getVideoFramebuffer(), GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);		

	GX_SetNumChans(1);

	GX_SetNumTexGens(1);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);	

	GX_InvalidateTexAll();
	
	Mtx GXmodelView2D;
	Mtx44 perspective;
	
	// Reset the model view matrix
	guMtxIdentity(GXmodelView2D);
	guMtxTransApply(GXmodelView2D, GXmodelView2D, 0.0f, 0.0f, -5.0f);
	
	// Apply changes to model view matrix
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

	guOrtho(perspective,0,479,0,639,0,300);

	// Apply changes to the projection matrix
	GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

	GX_SetViewport(0, 0, videoMode->fbWidth, videoMode->efbHeight, 0, 1);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);

	// The final scissor box
	GX_SetScissorBoxOffset(0, 0);
	GX_SetScissor(0, 0, this->gsWidth, this->gsHeight);
}

void GraphicsSystem::updateScene(u32 *frameBuffer) {
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(frameBuffer,GX_TRUE);
	GX_DrawDone();
	
	GX_InvalidateTexAll();
}
