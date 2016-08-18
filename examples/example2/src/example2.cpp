/*
 * example2
 *
 * This example demonstrates the use of the devkitPro FAT interface routines in order to
 * dynamically load a TrueType font from the SD card. In reality this should be the "correct"
 * way of loading a font file into your program in order to retain both flexibility and small
 * executable object size.
 *
 * For this example simple copy the rursus_compact_mono.ttf font from example1/data into the
 * root directory of the SD and insert it into the Wii.
 *
 * As noted in the documentation realize that the font file interaction routine is for example
 * purposes only, and any production-worthy implementation should contain much more robust
 * error checking and handling.
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
#include <fat.h>

#define TTF_PATH "rursus_compact_mono.ttf"	// Path to the TrueType font on the SD card.

/**
 * Loads a font from the provided font path into the supplied font data buffer.
 *
 * This routine reads in a TypeType font file from the provided file path and loads it into
 * the supplied font data buffer. Note that it is the user's responsibility to free the supplied
 * buffer once it is no longer needed. Also note that this routine is for example purposes only
 * and any actual production code should be much more robust in its error checking.
 *
 * @param filePath	Path to the TrueType font including the file name.
 * @param fontData	A pointer to the font buffer which will hold the file data.
 *
 * @return The size of the TrueType font file which was loaded.
 */
FT_Long loadFontFromFile(const char* filePath, uint8_t** fontData) {
	FILE* ttfFile = fopen(filePath, "r");
	fseek(ttfFile, 0, SEEK_END);	// Calculate the size of the font by seeking to the end of the file
	int fileSize = ftell(ttfFile);	// ...and getting your file cursor position.
	rewind(ttfFile);	// Back up to the head of the file.

	*fontData = new uint8_t[fileSize];			// Allocate your font buffer
	fread (*fontData, 1, fileSize, ttfFile);	// ...and read the font data into the buffer

	if(ttfFile != NULL) {
		fclose(ttfFile);	// Be kind. Always close files that you have opened.
	}

	return fileSize;
}

/**
 * Program entry point.
 *
 * @param argc	Number of provided command line parameters.
 * @param argv	List of command line parameters.
 *
 * @return Program exit status code.
 */
int main(int argc, char **argv) {

	if(!fatInitDefault()) {
		return 0;	// Unrecoverable error.
	}

	uint8_t* rursus_compact_mono_ttf = NULL;	// Placeholder font data pointer which will be populated shortly.
	FT_Long rursus_compact_mono_ttf_size = loadFontFromFile(TTF_PATH, &rursus_compact_mono_ttf); // Size of the TrueType file in bytes. Will be calculated shortly.

	VideoSystem* videoSystem = new VideoSystem();
	GraphicsSystem *graphicsSystem = new GraphicsSystem(videoSystem);
	PadSystem *padSystem = new PadSystem();

	FreeTypeGX *fontSystem = new FreeTypeGX(GX_TF_IA8);
	FT_UInt fontSize = 64;
	fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, fontSize, false);	// Initialize the font system with the dynamically loaded font parameters.

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

	delete rursus_compact_mono_ttf;	// Don't forget to free that font buffer you dynamically allocated!
	delete fontSystem;
	delete padSystem;
	delete graphicsSystem;
	delete videoSystem;
	
	return 0;
}

