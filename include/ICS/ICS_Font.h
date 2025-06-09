/*

ICS_Font

	Created: 2020-04-11

	Change log:

		2020-04-11
			- created ICS_Font which renders text using FreeType fonts.

		2023-01-20
			- fonts auto add and remove themselves from the game so they can be initialized at the right time
			- added a function to check if the font is initialized

		2023-01-23
			- ICS_Font now inherits from ICS_Resource
			- added font bank for re-using fonts that already exist

		2023-05-31
			- fixed a crash that occured on exit when the font bank was destroyed before a font

		2024-01-20
			- keep fonts in font bank, even when they are no longer in use

		2024-01-21
			- simplified font bank management
			- added a function to flush the font bank, freeing any fonts that are not in use

		2024-01-23
			- removed onGameInitialized function as ICS_Resource automatically calls initilize for the font

		2024-02-04
			- added a function to copy a font

		2024-05-19
			- added error logging

*/

#pragma once

#include <string>				// for std::string
#include <map>					// for std::map... for mapping filenames to fonts
#include <glut.h>				// the library for glut (OpenGL)

#include "ft2build.h"			// for FreeType fonts
#include "freetype/ftglyph.h"	// contains the definition of FT_Face

#include "ICS_Resource.h"		// ICS_Font inherits from ICS_Resource

/**
 * Renders text using FreeType fonts.
 */
class ICS_Font: public ICS_Resource
{

// member attributes

private:

	std::string _fileName;		// the name of the file containing the TrueType font to use
	int _height;				// the font size for the rendered text
	int _characterWidths[128];	// the width of each character in pixels

	GLuint _baseDisplayList;	// the base display list for the font set
	GLuint _textures[128];		// the texture ids

	int _referenceCount;		// tracks how many objects are using this font.

// font bank management

	// a map from file name to fonts so fonts can be re-used
	static std::map<std::pair<std::string, int>, ICS_Font*>* _fontBank;

public:

	/**
	 * Creates a font.
	 *
	 * @param filename		The name of the file containing the TrueType font (.ttf).
	 * @param height		The height of the font in pixels.
	 */
	static ICS_Font* createFont(std::string filename, int height);

	/**
	 * Creates a copy of the font.
	 *
	 * @param font	The font to copy
	 *
	 * @returns		The requested font if successful, a null pointer if not.
	 */
	static ICS_Font* copyFont(ICS_Font* font);

	/**
	 * Deletes the font.
	 *
	 * @param font		The font to delete.
	 */
	static void deleteFont(ICS_Font* font);

	/**
	 * Deletes all unused fonts stored in the bank to free memory.
	 */
	static void flushFontBank();

private:

// constructor / destructor

	/**
	 * The ICS_Font constructor.
	 *
	 * @param filename		The name of the file containing the TrueType font (.ttf).
	 * @param height		The height of the font in pixels.
	 */
	ICS_Font(std::string fileName, int height);

	/**
	 * The ICS_Font destructor.
	 */
	~ICS_Font();

public:

// getters

	/**
	 * Gets the height of the font (in pixels).
	 *
	 * @returns		The height of the font (in pixels).
	 */
	int getHeight() const
	{
		return _height;
	}

// rendering

	/**
	 * Renders text to the game window.  Will not work correctly until the menu has been initialized.
	 */
	int render(float x, float y, std::string text);

	/**
	 * Gets the width of the character in pixels.
	 *
	 * @param c		The character to get the width of.
	 */
	int getCharacterWidth(char c);

	/**
	 * Gets the width of the text in pixels.
	 *
	 * @param text		The text to get the width of.
	 */
	int getTextWidth(std::string text);

private:

// helpers

	/**
	 * Returns the lowest power of two which is greater than or equal to the value.
	 *
	 * @param value		The value to find the next power of two for.
	 */
	unsigned int nextPowerOfTwo(unsigned int value);

	/**
	 * Creates a texture and display for the given character.
	 *
	 * @param face		A handle to a typographic face object. A face object models a given typeface, in a given style.
	 * @param c			The character to generate a display list for.
	 */
	void makeDisplayList(FT_Face face, char c);

	/**
	 * Prepares the font for rendering.  Should be called after the graphics pipeline is initialized and before the font is used.
	 */
	void initialize();
};
