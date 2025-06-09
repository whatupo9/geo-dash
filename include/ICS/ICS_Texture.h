/*

ICS_Texture

	Created: ???

	Change log:

		2020-04-17
			- save alpha data from textures to use for per-pixel collision detection

		2020-05-22
			- added ability to create a texture from raw data

		2023-01-27
			- added helper function for generating alpha data
			- textures created from raw data now generate alpha data as well

		2023-01-28
			- the processes from creating a texture from file or raw data were unified into a single process
			- ICS_Texture now inherits from ICS_Resource and so it can be notified when the game engine is ready

		2023-05-31
			- fixed a crash that occured on exit when the texture bank was destroyed before a texture

		2024-01-19
			- added a greyscale version of each created texture

		2024-01-21
			- simplified texture bank management
			- added a function to flush the texture bank, freeing any textures that are not in use

		2024-01-23
			- removed onGameInitialized function as ICS_Resource automatically calls initilize for the texture

		2024-02-10
			- removed greyscale feature (cool but expensive)

*/

#pragma once

#include <string>			// for std::string
#include <map>				// for std::map... for mapping filenames to textures

#include "ICS_Resource.h"	// ICS_Texture inherits from ICS_Resource

/**
 * A texture which can be applied to a sprite or polygon.
 */
class ICS_Texture: public ICS_Resource
{

private:

// attributes

	std::string _fileName;				// the name of the file containing the image to use for the texture.
	int _width;							// the original width of the image (in pixels).
	int _height;						// the original height of the image (in pixels).
	int _channels;						// the number of channels in the image data

	unsigned char* _imageData;			// raw image data
	unsigned char* _alphaData;			// alpha data for the texture (for collistion detection)

	unsigned int _glTexture;			// the OpenGL texture
	
	int _referenceCount;				// tracks how many objects are using this texture

// texture bank management

	// a map from file name to texture pointers so textures can be re-used
	static std::map<std::string, ICS_Texture*>* _textureBank;

public:

	/**
	 * Creates a texture by loading an image from file.
	 *
	 * @param fileName	The name of the file containing the image to use for the texture.
	 *
	 * @returns			The requested texture if successful, a null pointer if not.
	 */
	static ICS_Texture* createTexture(std::string fileName);

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
	static ICS_Texture* createTexture(unsigned char* imageData, int width, int height, int channels);
	
	/**
	 * Creates a copy of the texture.
	 *
	 * @param texture		The texture to copy
	 *
	 * @returns				The requested texture if successful, a null pointer if not.
	 */
	static ICS_Texture* copyTexture(ICS_Texture* texture);

	/**
	 * Deletes the texture.
	 *
	 * @param texture	The texture to delete.
	 */
	static void deleteTexture(ICS_Texture* texture);

	/**
	 * Deletes all unused textures stored in the bank to free memory.
	 */
	static void flushTextureBank();

private:

// constructors / destructor

	/**
	 * ICS_Texture constructor which loads texture data from file.
	 *
	 * @param fileName	The name of the file containing the image to use for the texture.
	 */
	ICS_Texture(std::string fileName);

	/**
	 * ICS_Texture constructor which creates a texture using raw image data.
	 *
	 * @param imageData		The raw image data.
	 * @param width			The width of the texture in pixels.
	 * @param height		The height of the texture in pixels.
	 * @param channels		The number of channels in the image data.
	 */
	ICS_Texture(unsigned char* imageData, int width, int height, int channels);

	/**
	 * The destructor.
	 */
	~ICS_Texture();

public:

// getters

	/**
	 * Gets the name of the file used for the texture.
	 *
	 * @returns		The name of the file containing the image for the texture.
	 */
	std::string getFileName() const
	{
		return _fileName;
	}

	/**
	 * Gets the original width of the image.  OpenGL scales image dimensions to the next power of 2.
	 *
	 * @returns		The original width of the image (in pixels).
	 */
	int getWidth() const
	{
		return _width;
	}

	/**
	 * Gets the original height of the image.  OpenGL scales image dimensions to the next power of 2.
	 *
	 * @returns		The original height of the image (in pixels).
	 */
	int getHeight() const
	{
		return _height;
	}

	/**
	 * Gets the alpha value of the specified pixel.
	 *
	 * @param x		The x coordinate of the pixel.
	 * @param y		The y coordinate of the pixel.
	 */
	unsigned int getAlpha(int x, int y) const
	{
		// make sure the coordinates are valid
		if (_alphaData and x >= 0 and x < _width and y >= 0 and y < _height)
		{
			return _alphaData[x + y * _width];
		}

		// if the coordinates are invalid, return 0 (transparent)
		return 0;
	}

// rendering

	/**
	 * Binds this texture to the so that it will be applied to polygons.
	 *
	 * @returns				true if the texture was ready for use and successfully bound, false otherwise.
	 */
	bool bind();

private:

// helpers

	/**
	 * Generates alpha data for the texture.
	 */
	void generateAlphaData();

	/**
	 * Prepares the texture for rendering.  Should be called after the graphics pipeline is initialized and before the texture is used.
	 */
	void initialize();
};