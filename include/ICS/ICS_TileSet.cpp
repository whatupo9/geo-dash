#include "ICS_TileSet.h"	// the declaration of the ICS_Tileset class
#include "SOIL.h"			// OpenGL image library
#include "ICS_Texture.h"	// the declaration of the ICS_Texture class
#include "ICS_Game.h"		// the definition of ICS_Game

// initialize the static member for the tileset bank
std::map<std::tuple<std::string, int, int>, ICS_Tileset*>* ICS_Tileset::_tilesetBank = NULL;

/**
 * Creates a tileset.
 *
 * @param filename		The name of the file containing the image for the tileset.
 * @param tileWidth		The width of each tile in pixels.
 * @param tileHeight	The height of each tile in pixels.
 */
ICS_Tileset*
ICS_Tileset::createTileset(std::string filename, int tileWidth, int tileHeight)
{
	// check if the tileset bank exists and create it if necessary.
	if (!_tilesetBank)
	{
		_tilesetBank = new std::map<std::tuple<std::string, int, int>, ICS_Tileset*>;
	}

	// declare the tileset to return
	ICS_Tileset* tileset = NULL;

	// create a key for this tileset
	std::tuple<std::string, int, int> key = std::make_tuple(filename, tileWidth, tileHeight);

	// check if the tileset is already created
	if (_tilesetBank->find(key) != _tilesetBank->end())
	{
		// get the tileset
		tileset = (*_tilesetBank)[key];

		// increase the reference count (the number of things using the tilest)
		tileset->_referenceCount++;
	}

	// if the tileset is not already created, create it
	else
	{
		// create the tileset
		tileset = new ICS_Tileset(filename, tileWidth, tileHeight);

		// add the tileset to the bank
		(*_tilesetBank)[key] = tileset;
	}

	// return the tileset
	return tileset;
}

/**
 * Creates a copy of the tileset.
 *
 * @param tileset		The tileset to copy
 *
 * @returns				The requested tileset if successful, a null pointer if not.
 */
ICS_Tileset*
ICS_Tileset::copyTileset(ICS_Tileset* tileset)
{
	// increase the reference count
	if (tileset)
	{
		tileset->_referenceCount++;
	}

	// return the tileset
	return tileset;
}

/**
 * Deletes the tileset.
 *
 * @param tileset		The tileset to delete.
 */
void
ICS_Tileset::deleteTileset(ICS_Tileset* tileset)
{
	// delete the tileset if it is valid
	if (tileset)
	{
		// reduce the reference count
		if (tileset->_referenceCount > 0)
		{
			tileset->_referenceCount--;
		}

		// this should never happen
		else
		{
			// ERROR
		}
	}
}

/**
 * Deletes all unused tilesets stored in the bank to free memory.
 */
void
ICS_Tileset::flushTilesetBank()
{
	// make sure the tileset bank exists
	if (_tilesetBank)
	{
		// create an iterator to iterate the bank
		std::map<std::tuple<std::string, int, int>, ICS_Tileset*>::iterator it = _tilesetBank->begin();

		// iterate through the bank
		while (it != _tilesetBank->end())
		{
			// get the tileset
			ICS_Tileset* tileset = it->second;

			// check if the tileset is no longer in use
			if (tileset->_referenceCount == 0)
			{
				// remove the tileset from the bank
				_tilesetBank->erase(it++);

				// delete the tileset
				delete tileset;
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
 * The ICS_Tileset constructor.
 *
 * @param fileName		The name of the file containing the image for the tileset.
 * @param tileWidth		The width of each tile in pixels.
 * @param tileHeight	The height of each tile in pixels.
 */
ICS_Tileset::ICS_Tileset(std::string fileName, int tileWidth, int tileHeight)
	:
	_fileName(fileName),
	_tileWidth(tileWidth),
	_tileHeight(tileHeight),
	_height(0),
	_width(0),
	_textures(NULL),
	_referenceCount(1)
{
	// initialize the tileset
	initialize();
}

/**
 * The ICS_Tileset destructor.
 */
ICS_Tileset::~ICS_Tileset()
{
	// delete the textures
	if (_textures)
	{
		// delete each row of textures
		for (int tileRow = 0; tileRow < _height; tileRow++)
		{
			// delete each texture in the row
			for (int tileColumn = 0; tileColumn < _width; tileColumn++)
			{
				// delete the texture
				ICS_Texture::deleteTexture(_textures[tileRow][tileColumn]);
			}

			// delete memory allocated for this row of textures
			delete[] _textures[tileRow];
		}

		// delete memory allocate for the rows of textures
		delete[] _textures;
	}
}

/**
 * Returns the texture for the tile at the specified index.
 * The texture is deleted when the tile set is deleted.
 *
 * @param index		The index of the tile.
 *
 * @returns			The requested texture or NULL if the index is invalid.
 */
ICS_Texture*
ICS_Tileset::getTexture(int index)
{
	// prevent divide by 0 when there aren't any tiles
	if (_width > 0)
	{
		// determine the coordinates of the tile
		int x = index % _width;
		int y = index / _width;

		// return the requested tile
		return getTexture(x, y);
	}

	// if the coordinates are invalid, return NULL
	return NULL;
}

/**
 * Returns the texture for the tile at the specified location.
 * The texture is deleted when the tile set is deleted.
 *
 * @param x		The horizontal position of the tile.
 * @param y		The vertical position of the tile.
 *
 * @returns		The requested texture or NULL if the row and column are invalid.
 */
ICS_Texture*
ICS_Tileset::getTexture(int x, int y)
{
	// verify the coordinates of the texture
	if (_textures and x >= 0 and x < _width and y >= 0 and y < _height)
	{
		return _textures[x][y];
	}

	// if the coordinates are invalid, return NULL
	return NULL;
}

/**
 * Prepares the tileset for rendering.  Should be called after the graphics pipeline is initialized and before the tileset is rendered.
 */
void
ICS_Tileset::initialize()
{
	// make sure the game is initialized and the tileset isn't already initialized
	if (ICS_Game::getInstance().isInitialized() and not _initialized)
	{
		// load the image data from file
		int textureWidth;
		int textureHeight;
		int channels;

		unsigned char* imageData = SOIL_load_image(_fileName.c_str(), &textureWidth, &textureHeight, &channels, SOIL_LOAD_AUTO);

		// make sure the image data was successfully loaded
		if (imageData)
		{
			// determine the number of tiles in the tileset
			_width = textureWidth / _tileWidth;
			_height = textureHeight / _tileHeight;

			// allocate memory for each column of textures
			_textures = new ICS_Texture**[_width];

			// calculate values to be re-used with each texture
			int tileWidthBytes = _tileWidth * channels;		// the width of each tile texture in bytes
			int tileBytes = tileWidthBytes * _tileHeight;	// the total bytes for each tile texture

			// allocate memory for the raw texture data for each tile texture
			unsigned char* rawData = new unsigned char[tileBytes];

			// create each texture in the column
			for (int x = 0; x < _width; x++)
			{
				// allocate memory for each texture in the column
				_textures[x] = new ICS_Texture*[_height];

				// copy the raw texture data for each texture in the column
				for (int y = 0; y < _height; y++)
				{
					// determine the offset of the texture data within the original texture
					int offset = x * _tileWidth * channels + (y * textureWidth * _tileHeight * channels);

					// copy the raw texture data
					for (int row = 0; row < _tileHeight; row++)
					{
						memcpy(&rawData[row * tileWidthBytes], &imageData[offset + row * textureWidth * channels], tileWidthBytes);
					}

					// create the texture and store it in the array
					_textures[x][y] = ICS_Texture::createTexture(rawData, _tileWidth, _tileHeight, channels);
				}
			}

			// delete memory allocated for the raw texture data for each tile
			delete[] rawData;

			// free the allocated memory
			SOIL_free_image_data(imageData);
		}

		// the tileset has been initialized
		_initialized = true;
	}
}