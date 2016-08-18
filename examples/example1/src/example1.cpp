/*
 * example1
 *
 * This example demonstrates the use of bin2o in order to compile a font into an
 * object file which can be linked into the final executable dol/elf.
 *
 * Modify your Makefile to scan the directories containing the TrueType font.
 * Leverage the bin2o tool included in the devkitPro by adding the following code to the Makefile:
 *
 * # --- Begin Makefile snippet ---
 *
 * %.ttf.o	:	%.ttf
 *	@echo $(notdir $<)
 *	$(bin2o)
 *
 * # --- End Makefile snippet ---
 *
 * For this example we have converted the Rursus Compact Mono font (data/rursus_compact_mono.tff) and have
 * included the compiled font data pointers as contained in the generated rorsus_compact_mono_ttf.h file.
 *
 * -----------------
 * Program Controls:
 * -----------------
 * Up 		Increase font size
 * Down		Decrease font size
 * Left		Toggle text underlining
 * Right	Toggle text strikethrough
 * Home		Exit
 *
 */
#include "core/GraphicsSystem.h"
#include "core/PadSystem.h"
#include "core/VideoSystem.h"

#include <FreeTypeGX.h>
#include "rursus_compact_mono_ttf.h"	// Include the compiled font.
										// Once compiled you can view the contents of this file in example1/all/rursus_compact_mono_ttf.h
/**
 * Program entry point.
 *
 * @param argc	Number of provided command line parameters.
 * @param argv	List of command line parameters.
 *
 * @return Program exit status code.
 */
int main(int argc, char **argv) {
	VideoSystem* videoSystem = new VideoSystem();
	GraphicsSystem *graphicsSystem = new GraphicsSystem(videoSystem);
	PadSystem *padSystem = new PadSystem();

	FreeTypeGX *fontSystem = new FreeTypeGX(GX_TF_IA8);
	FT_UInt fontSize = 64;
	fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, fontSize, false);	// Initialize the font system with the font parameters from rursus_compact_mono_ttf.h

	uint32_t buttons = 0x0000;
	uint32_t textStyle = FTGX_JUSTIFY_CENTER;
	bool isUnderlined = false;
	bool isStrike = false;

	while(!padSystem->pressedExitButton(buttons = padSystem->scanPads(0))) {

		if(padSystem->pressedUp(buttons)) {	// Increase font size
			fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, ++fontSize, false);
		}
		if(padSystem->pressedDown(buttons)) {	// Decrease font size
			fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, fontSize > 6 ? --fontSize : fontSize, false);
		}
		if(padSystem->pressedLeft(buttons)) {	// Toggle text underlining
			isUnderlined = !isUnderlined;
		}
		if(padSystem->pressedRight(buttons)) {	// Toggle text strikethrough
			isStrike = !isStrike;
		}

		textStyle = FTGX_JUSTIFY_CENTER;
		textStyle = isUnderlined	? textStyle | FTGX_STYLE_UNDERLINE	: textStyle;
		textStyle = isStrike		? textStyle | FTGX_STYLE_STRIKE		: textStyle;

		fontSystem->drawText(320,	50,		_TEXT("THE QUICK BROWN"),	(GXColor){0xff, 0x00, 0x00, 0xff},	textStyle | FTGX_ALIGN_TOP);
		fontSystem->drawText(320,	125,	_TEXT("FOX JUMPS OVER"),	(GXColor){0x00, 0xff, 0x00, 0xff},	textStyle | FTGX_ALIGN_MIDDLE);
		fontSystem->drawText(320,	200,	_TEXT("THE LAZY DOG"),		(GXColor){0x00, 0x00, 0xff, 0xff},	textStyle | FTGX_ALIGN_BOTTOM);

		fontSystem->drawText(320,	275,	_TEXT("the quick brown"),	(GXColor){0xff, 0xff, 0x00, 0xff},	textStyle);
		fontSystem->drawText(320,	350,	_TEXT("fox jumps over"),	(GXColor){0xff, 0x00, 0xff, 0xff},	textStyle);
		fontSystem->drawText(320,	425,	_TEXT("the lazy dog"),		(GXColor){0x00, 0xff, 0xff, 0xff},	textStyle);

		graphicsSystem->updateScene(videoSystem->getVideoFramebuffer());
		videoSystem->flipVideoFramebuffer();
	}

	delete fontSystem;
	delete padSystem;
	delete graphicsSystem;
	delete videoSystem;
	
	return 0;
}

