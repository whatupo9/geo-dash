#include "ICS_Font.h"					// the definition of ICS_Font
#include "ICS_Game.h"					// the definition of ICS_Game
#include "ICS_DebugLog.h"				// the definition of ICS_DebugLog

#pragma comment (lib, "freetype.lib")	// the FreeType font library

// initialize the static member for the font bank
std::map<std::pair<std::string, int>, ICS_Font*>* ICS_Font::_fontBank = NULL;

/**
 * Creates a font.
 *
 * @param filename		The name of the file containing the TrueType font (.ttf).
 * @param height		The height of the font in pixels.
 */
ICS_Font*
ICS_Font::createFont(std::string filename, int height)
{
	// check if the font bank exists and create it if necessary.
	if (!_fontBank)
	{
		_fontBank = new std::map<std::pair<std::string, int>, ICS_Font*>;
	}

	// the font to return
	ICS_Font* font = NULL;

	// create a key for this font
	std::pair<std::string, int> key = std::make_pair(filename, height);

	// check if the font is already created
	if (_fontBank->find(key) != _fontBank->end())
	{
		// get the texture
		font = (*_fontBank)[key];

		// something else is using this font
		font->_referenceCount++;
	}

	// if the font is not already created, create it
	else
	{
		// create the texture
		font = new ICS_Font(filename, height);

		// add the font to the map
		(*_fontBank)[key] = font;
	}

	// return the font
	return font;
}

/**
 * Creates a copy of the font.
 *
 * @param font	The font to copy
 *
 * @returns		The requested font if successful, a null pointer if not.
 */
ICS_Font*
ICS_Font::copyFont(ICS_Font* font)
{
	// increase the reference count
	if (font)
	{
		font->_referenceCount++;
	}

	// return the font
	return font;
}

/**
 * Deletes the font.
 *
 * @param font		The font to delete.
 */
void
ICS_Font::deleteFont(ICS_Font* font)
{
	// delete the font if it is valid
	if (font)
	{
		// reduce the reference count
		if (font->_referenceCount > 0)
		{
			font->_referenceCount--;
		}

		// this should never happen
		else
		{
			// ERROR
		}
	}
}

/**
 * Deletes all unused fonts stored in the bank to free memory.
 */
void
ICS_Font::flushFontBank()
{
	// make sure the font bank exists
	if (_fontBank)
	{
		// create an iterator to iterate the bank
		std::map<std::pair<std::string, int>, ICS_Font*>::iterator it = _fontBank->begin();

		// iterate through the bank
		while (it != _fontBank->end())
		{
			// get the font
			ICS_Font* font = it->second;

			// check if the font is no longer in use
			if (font->_referenceCount == 0)
			{
				// remove the font from the bank
				_fontBank->erase(it++);

				// delete the font
				delete font;
			}

			// increment the iterator
			else
			{
				++it;
			}
		}
	}
}

/**
 * The ICS_Font constructor.
 *
 * @param filename		The name of the file containing the TrueType font (.ttf).
 * @param height		The height of the font in pixels.
 */
ICS_Font::ICS_Font(std::string fileName, int height)
	:
	_fileName(fileName),
	_height(height),
	_baseDisplayList(0),
	_referenceCount(1)
{
	// give the character widths default values
	for (int i = 0; i < 128; i++)
	{
		_characterWidths[i] = 0;
	}

	// initialize the font
	initialize();
}

/**
 * The destructor.
 */
ICS_Font::~ICS_Font()
{
	// free the display list
	glDeleteLists(_baseDisplayList, 128);

	// free the textures
	glDeleteTextures(128, _textures);
}

/**
 * Renders text to the game window.  Will not work correctly until the font has been initialized.
 */
int
ICS_Font::render(float x, float y, std::string text)
{
	// make sure the font is initialized
	if (_initialized)
	{
		// enable textures
		glEnable(GL_TEXTURE_2D);	// enable textures

		// set the display list for the font
		glListBase(_baseDisplayList);

		// remember the transformations so they won't be overwritten
		glPushMatrix();

		// translate the text to the correct screen position
		glTranslatef(x, y, 0);

		// render the text
		glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());

		// restore the previous transformations
		glPopMatrix();

		// return the width of the rendered text
		return getTextWidth(text);
	}

	// this only happens if the font has not been initialized
	return 0;
}

/**
 * Gets the width of the character in pixels.
 *
 * @param c		The character to get the width of.
 */
int
ICS_Font::getCharacterWidth(char c)
{
	// make sure the character is valid
	if (c < 128)
	{
		return _characterWidths[c];
	}
	
	// the character is not valid 
	return 0;
}

/**
 * Gets the width of the text in pixels.
 *
 * @param text		The text to get the width of.
 */
int
ICS_Font::getTextWidth(std::string text)
{
	// keep track of the total width
	int width = 0;

	// add the width of each character to the total
	for (unsigned int i = 0; i < text.length(); i++)
	{
		// make sure the character is valid
		if (text[i] < 128)
		{
			width += _characterWidths[text[i]];
		}
	}

	// return the total
	return width;
}

/**
 * Returns the lowest power of two which is greater than or equal to the value.
 *
 * @param value		The value to find the next power of two for.
 */
unsigned int
ICS_Font::nextPowerOfTwo(unsigned int value)
{
	// start at the lowest power of two
	unsigned int power = 1;

	// keep increasing the power until it is at least as big as the value
	while (power < value)
	{
		power *= 2;
	}

	// return the result
	return power;
}

/**
 * Creates a texture and display for the given character.
 *
 * @param face		A handle to a typographic face object. A face object models a given typeface, in a given style.
 * @param c			The character to generate a display list for.
 */
void
ICS_Font::makeDisplayList(FT_Face face, char ch)
{
	// load the glyph for the character
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
	{
		ICS_LOG_ERROR(std::string("Failed to load glyph for character '") + ch + "'");
	}

	// Move The Face's Glyph Into A Glyph Object.
	FT_Glyph glyph;

	if (FT_Get_Glyph(face->glyph, &glyph))
	{
		ICS_LOG_ERROR(std::string("Failed to get glyph for character '") + ch + "'");
	}

	// convert the glyph to a bitmap
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

	// get the bitmap from the glyph
	FT_Bitmap& bitmap = bitmapGlyph->bitmap;

	// get dimensions needed for the texture
	unsigned int width = nextPowerOfTwo(bitmap.width);
	unsigned int height = nextPowerOfTwo(bitmap.rows);

	// create the texture
	{
		// allocate memory for the texture data
		GLubyte* textureData = new GLubyte[2 * width * height];

		// fill in the texture data
		for (unsigned int j = 0; j < height; j++)
		{
			for (unsigned int i = 0; i < width; i++)
			{
				textureData[2 * (i + j * width)] = 255;
				textureData[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
			}
		}

		// bind the texture and initilize properties
		glBindTexture(GL_TEXTURE_2D, _textures[ch]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, textureData);

		// delete the texture data
		delete[] textureData;
	}

	// begin a new display list
	glNewList(_baseDisplayList + ch, GL_COMPILE);

	{
		// bind the texture for the character so it will be rendered on the quad
		glBindTexture(GL_TEXTURE_2D, _textures[ch]);

		// remember the current transformations
		glPushMatrix();
		{
			// move the character over so it is not on top of the previous one
			glTranslatef(bitmapGlyph->left, 0, 0);

			// flip the texure upside down (FreeType has a different coordinate system) and move it down so characters with tails (like g and p) sit on a line
			glTranslatef(0, _height - (face->glyph->metrics.height >> 6) + ((int)bitmap.rows - (int)bitmapGlyph->top), 0);

			// calculate the location of the character within the bitmap
			float x = (float)bitmap.width / (float)width;
			float y = (float)bitmap.rows / (float)height;

			// render the texture to a quad
			glBegin(GL_QUADS);
			{
				glTexCoord2d(0, y); glVertex2f(0, bitmap.rows);
				glTexCoord2d(0, 0); glVertex2f(0, 0);
				glTexCoord2d(x, 0); glVertex2f(bitmap.width, 0);
				glTexCoord2d(x, y); glVertex2f(bitmap.width, bitmap.rows);
			}
			glEnd();
		}
		// restore the previous transformation
		glPopMatrix();

		// add a horizontal translation for the next character
		glTranslatef(face->glyph->advance.x >> 6, 0, 0);
	}

	// Finish The Display List
	glEndList();

	// free the glyph
	FT_Done_Glyph(glyph);
}

/**
 * Prepares the font for rendering.  Should be called after the graphics pipeline is initialized and before the font is rendered.
 */
void
ICS_Font::initialize()
{
	// make sure the game is initialized and the font isn't already initialized
	if (ICS_Game::getInstance().isInitialized() and not _initialized)
	{
		// create and initilize a FreeType font library.
		FT_Library library;

		if (FT_Init_FreeType(&library))
		{
			ICS_LOG_ERROR("Failed to initialize the FreeType library.");
			return;
		}

		// declare a face to hold font information
		FT_Face face;

		// attempt to load the TrueType font
		if (FT_New_Face(library, _fileName.c_str(), 0, &face))
		{
			ICS_LOG_ERROR("Failed to load the \"" + _fileName + "\" font.");
			return;
		}

		// FreeType measures font size in 1/64ths of a pixel.
		FT_Set_Char_Size(face, _height << 6, _height << 6, 96, 96);

		// allocate resources for the display lists and textures
		_baseDisplayList = glGenLists(128);
		glGenTextures(128, _textures);

		// generate a display list for each character
		for (unsigned char i = 0; i < 128; i++)
		{
			// generate the display list
			makeDisplayList(face, i);

			// keep track of the character width
			_characterWidths[i] = (face->glyph->advance.x >> 6);
		}

		// free the face
		FT_Done_Face(face);

		// free the library
		FT_Done_FreeType(library);

		// the font is initialized
		_initialized = true;
	}
}