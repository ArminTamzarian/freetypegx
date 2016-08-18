# FreeTypeGX
FreeTypeGX is a wrapper class for libFreeType which renders a compiled FreeType parseable font into a GX texture for Wii homebrew development.


FreeTypeGX is a wrapper class for libFreeType which renders a compiled FreeType parsable font into a GX texture for Wii homebrew development. FreeTypeGX is written in C++ and makes use of a selectable pre-buffered or buffer-on-demand methodology to allow fast and efficient printing of text to the EFB.

Note: As of version 0.2.1 FreeTypeGX has forked into two disparate projects and now relies upon the [Metaphrasis](https://github.com/ArminTamzarian/metaphrasis) library.

This library was developed in-full by Armin Tamzarian with the support of developers in #wiidev on EFnet.

Full Doxygen API documentation can be found at https://armintamzarian.github.io/freetypegx for assistance with program integration.

## Installation (Source Code)

1. Ensure that you have the FreeType Wii library installed in your development environment with the library added to your Makefile where appropriate.
2. Ensure that you have the Metaphrasis library installed in your development environment with the library added to your Makefile where appropriate.
3. Extract the FreeTypeGX archive.
Copy the contents of the src directory into your project's development path.
Include the FreeTypeGX header file in your code using syntax such as the following:

```c++
include "FreeTypeGX.h"
```

## Installation (Library)

1. Ensure that you have the FreeType Wii library installed in your development environment with the library added to your Makefile where appropriate.
2. Ensure that you have the Metaphrasis library installed in your development environment with the library added to your Makefile where appropriate.
3. Extract the FreeTypeGX archive.
4. Copy the contents of the lib directory into your devKitPro/libogc directory.
5. Include the FreeTypeGX header file in your code using syntax such as the following:

```c++
include "FreeTypeGX.h"
```

## FreeTypeGX Prerequisites

Before you begin using FreeTypeGX in your project you must ensure that the you have some method by which to allocate the font data buffer for use within the initialization routines. For examples showing the most common methods to generate a font data buffer for use with FreeTypeGX see the included examples. These examples illustrate the following methods:

`example1` Compiling a font into the executable

`example2` Loading a font from a file located on an SD card or USB drive

Note that both of these example result with the creation of the following example variables:


`uint8_t* rursus_compact_mono_ttf` A buffer containing the font data.

`FT_Long rursus_compact_mono_ttf_size` A long value containing the size of the font data buffer in bytes.

## FreeTypeGX Usage

Within the file you included the FreeTypeGX.h header create an instance object of the FreeTypeGX class:

```c++
FreeTypeGX *freeTypeGX = new FreeTypeGX();
```

Alternately you can specify a texture format to which you would like to render the font characters. Note that the default value for this parameter is GX_TF_RGBA8.

```c++
FreeTypeGX *freeTypeGX = new FreeTypeGX(GX_TF_RGB565);
```

Furthermore, you can also specify a positional format as defined in your graphics subsystem initialization. Note that the default value for this parameter is GX_POS_XYZ.

```c++
FreeTypeGX *freeTypeGX = new FreeTypeGX(GX_TF_RGB565, GX_POS_XY);
```

Currently supported textures are:
* `GX_TF_I4`
* `GX_TF_I8`
* `GX_TF_IA4`
* `GX_TF_IA8`
* `GX_TF_RGB565`
* `GX_TF_RGB5A3`
* `GX_TF_RGBA8`

Currently supported position formats are:
* `GX_POS_XY`
* `GX_POS_XYZ`

Using the allocated FreeTypeGX instance object call the loadFont function to load the font from the compiled buffer and specify the desired point size:

```c++
fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, 64);
```

Alternately you can specify a flag which will load and cache all available font glyphs immidiately. Note that on large font sets enabling this feature could take a significant amount of time.
```c++
fontSystem->loadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, 64, true);
```

Using the allocated FreeTypeGX instance object call the drawText function to print a string at the specified screen X and Y coordinates to the current EFB:

```c++
freeTypeGX->drawText(10, 25, _TEXT("FreeTypeGX Rocks!"));
```

Alternately you can specify a GXColor object you would like to apply to the printed characters:

```c++
freeTypeGX->drawText(10, 25, _TEXT("FreeTypeGX Rocks!"), (GXColor){0xff, 0xee, 0xaa, 0xff});
```

Furthermore you can also specify a group of styling parameters which will modify the positioning or style of the text:

```c++
freeTypeGX->drawText(10, 25, _TEXT("FreeTypeGX Rocks!"), (GXColor){0xff, 0xee, 0xaa, 0xff}, FTGX_JUSTIFY_CENTER | FTGX_ALIGN_BOTTOM | FTGX_STYLE_UNDERLINE);
```

Currently style parameters are:
* `FTGX_JUSTIFY_LEFT`
* `FTGX_JUSTIFY_CENTER`
* `FTGX_JUSTIFY_RIGHT`
* `FTGX_ALIGN_TOP`
* `FTGX_ALIGN_MIDDLE`
* `FTGX_ALIGN_BOTTOM`
* `FTGX_STYLE_UNDERLINE`
* `FTGX_STYLE_STRIKE`
