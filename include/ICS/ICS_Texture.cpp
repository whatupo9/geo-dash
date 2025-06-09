#include "ICS_Game.h"		// the ICS_Game definition
#include "ICS_Texture.h"	// declaration of ICS_Texture
#include "SOIL.h"			// OpenGL image library

#include <glut.h>			// the library for glut (OpenGL)

// initialize the static member for the texture bank
std::map<std::string, ICS_Texture*>* ICS_Texture::_textureBank = NULL;

/**
 * Creates a texture by loading an image from file.
 *
 * @param fileName	The name of the file containing the image to use for the texture.
 *
 * @returns			The requested texture if successful, a null pointer if not.
 */
ICS_Texture*
ICS_Texture::createTexture(std::string fileName)
{
	// check if the texture bank exists and create it if necessary.
	if (!_textureBank)
	{
		_textureBank = new std::map<std::string, ICS_Texture*>;
	}

	// the texture to return
	ICS_Texture* texture = NULL;

	// check if the texture is already created
	if (_textureBank->find(fileName) != _textureBank->end())
	{
		// get the texture
		texture = (*_textureBank)[fileName];
		texture->_referenceCount++;
	}

	// if the texture is not already created, create the texture
	else
	{
		// create the texture
		texture = new ICS_Texture(fileName);

		// add the texture to the bank
		(*_textureBank)[fileName] = texture;
	}
	
	// return the texture
	return texture;
}

/**
 * Creates a texture from raw image data
 *
 * @param imageData		The raw image data.
 * @param width			The width of the texture in pixels.
 * @param height		The height of the texture in pixels.
 * @param channels		The number of channels in the image data.
 *
 * @returns				The requested texture if successful, a null pointer if not.
 */
ICS_Texture*
ICS_Texture::createTexture(unsigned char* imageData, int width, int height, int channels)
{
	return new ICS_Texture(imageData, width, height, channels);
}

/**
 * Creates a copy of the texture.
 *
 * @param texture		The texture to copy
 *
 * @returns				The requested texture if successful, a null pointer if not.
 */
ICS_Texture*
ICS_Texture::copyTexture(ICS_Texture* texture)
{
	// increase the reference count
	if (texture)
	{
		texture->_referenceCount++;
	}

	// return the texture
	return texture;
}

/**
 * Deletes the texture.
 *
 * @param texture	The texture to delete.
 */
void
ICS_Texture::deleteTexture(ICS_Texture* texture)
{
	// delete the texture if it is valid
	if (texture)
	{
		// reduce the reference count
		if (texture->_referenceCount > 0)
		{
			texture->_referenceCount--;
		}

		// this should never happen
		else
		{
			// ERROR
		}
	}
}

/**
 * Deletes all unused textures stored in the bank to free memory.
 */
void
ICS_Texture::flushTextureBank()
{
	// make sure the font bank exists
	if (_textureBank)
	{
		// create an iterator to iterate the bank
		std::map<std::string, ICS_Texture*>::iterator it = _textureBank->begin();

		// iterate through the bank
		while (it != _textureBank->end())
		{
			// get the texture
			ICS_Texture* texture = it->second;

			// check if the texture is no longer in use
			if (texture->_referenceCount == 0)
			{
				// remove the texture from the bank
				_textureBank->erase(it++);

				// delete the texture
				delete texture;
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
 * ICS_Texture constructor which loads texture data from file.
 *
 * @param fileName	The name of the file containing the image to use for the texture.
 */
ICS_Texture::ICS_Texture(std::string fileName)
	:
	_fileName(fileName),
	_width(0),
	_height(0),
	_channels(0),
	_imageData(NULL),
	_alphaData(NULL),
	_glTexture(0),
	_referenceCount(1)
{
	// initialize if the game is initialized
	if (ICS_Game::getInstance().isInitialized())
	{
		initialize();
	}
}

/**
 * ICS_Texture constructor which creates a texture using raw image data.
 *
 * @param imageData		The raw image data.
 * @param width			The width of the texture in pixels.
 * @param height		The height of the texture in pixels.
 * @param channels		The number of channels in the image data.
 */
ICS_Texture::ICS_Texture(unsigned char* imageData, int width, int height, int channels)
	:
	_fileName(""),
	_width(width),
	_height(height),
	_channels(channels),
	_imageData(NULL),
	_alphaData(NULL),
	_glTexture(0),
	_referenceCount(1)
{
	// copy the image data
	if (_width > 0 and _height > 0 and _channels > 0)
	{
		_imageData = new unsigned char[_width * _height * _channels];
		memcpy(_imageData, imageData, width * height * _channels);
	}

	// initialize the texture
	initialize();
}

/**
 * The destructor.
 */
ICS_Texture::~ICS_Texture()
{
	// free the texture
	if (_glTexture)
	{
		glDeleteTextures(1, &_glTexture);
	}

	// delete the alpha data
	if (_alphaData)
	{
		delete[] _alphaData;
	}

	// delete the image data
	if (_imageData)
	{
		delete[] _imageData;
	}
}

/**
 * Binds this texture to the so that it will be applied to polygons.
 *
 * @returns				true if the texture was ready for use and successfully bound, false otherwise.
 */
bool
ICS_Texture::bind()
{
	// if the texture id is valid, bind it
	if (_glTexture)
	{
		glBindTexture(GL_TEXTURE_2D, _glTexture);
		return true;
	}
	
	// the texture id is not valid
	return false;
}

/**
 * Generates alpha data for the texture.
 */
void
ICS_Texture::generateAlphaData()
{
	// image data is required
	if (not _imageData)
	{
		return;
	}

	// delete old alpha data
	if (_alphaData)
	{
		delete[] _alphaData;
		_alphaData = NULL;
	}

	// allocate memory for the alpha data
	_alphaData = new unsigned char[_width * _height];

	// if the data is RGBA, get the alpha value from the data
	if (_channels == 4)
	{
		for (int i = 0; i < _width * _height; i++)
		{
			_alphaData[i] = _imageData[i * 4 + 3];
		}
	}

	// for all other formats, just set the alpha value to the max (opaque)
	else
	{
		for (int i = 0; i < _width * _height; i++)
		{
			_alphaData[i] = 255;
		}
	}
}

/**
 * Prepares the texture for rendering.  Should be called after the graphics pipeline is initialized and before the texture is used.
 */
void
ICS_Texture::initialize()
{
	// make sure the game is initialized and the texture isn't already initialized
	if (ICS_Game::getInstance().isInitialized() and not _initialized)
	{
		// keep track of how the image data was allocated
		bool usedNew = true;

		// load the image data from file if it doesn't exist
		if (not _imageData and _fileName != "")
		{
			// get the image data from file using the SOIL library
			_imageData = SOIL_load_image(_fileName.c_str(), &_width, &_height, &_channels, SOIL_LOAD_AUTO);
			usedNew = false;
		}

		// create the texture if the image data is valid
		if (_imageData)
		{
			// generate alpha data for the texture
			generateAlphaData();

			// create the OpenGL texure using the SOIL library
			_glTexture = SOIL_create_OGL_texture(_imageData, _width, _height, _channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

			// if the texture was loaded, initialize it using common settings
			if (_glTexture)
			{
				// bind the texture and initilize properties
				glBindTexture(GL_TEXTURE_2D, _glTexture);

				// set filtering attributes
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				// set wrapping attributes
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
		}

		// delete the image data (no longer needed)
		if (_imageData and usedNew)
		{
			delete[] _imageData;
		}
		else if (_imageData)
		{
			SOIL_free_image_data(_imageData);
		}

		_imageData = NULL;

		// the texture is initialized
		_initialized = true;
	}
}
