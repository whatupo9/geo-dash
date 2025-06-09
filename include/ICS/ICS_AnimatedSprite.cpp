#include "ICS_Game.h"				// the declaraction of ICS_Game
#include "ICS_TileSet.h"			// the declaration of ICS_Tileset
#include "ICS_AnimatedSprite.h"		// the declaration of ICS_AnimatedSprite
#include "ICS_Animation.h"			// the declaration of ICS_Animation

#include <fstream>					// for ifstream (used to read animation data from file)
#include <sstream>					// for sstream (used for tokenizing animation data)

using namespace std;				// for string, vector and map

/**
 * The constructor.
 *
 * @param filename	The name of the file containing the sprite animation data.
 * @param width		The width to render the sprite in pixels.
 * @param height	The height to render the sprite in pixels.
 */
ICS_AnimatedSprite::ICS_AnimatedSprite(string filename, float width, float height)
	:
	_frame(0),
	_elapsed(0),
	_tileset(NULL),
	_currentAnimation(NULL)
{
	// attempt to open the file
	ifstream inFile(filename.c_str());

	// make sure the file was opened successfully
	if (inFile.is_open())
	{
		string fileName;				// the name of the file to use for the tileset
		int tileWidth = 0;				// the width of each tile in pixels
		int tileHeight = 0;				// the height of each tile in pixels

		string line;					// for reading a line from the file
		string parserState = "tileset";	// the current state of the parser

		// continue to read from the file until the entire file is parsed
		while (getline(inFile, line))
		{
			// remove comments from the line
			line = removeComment(line);

			// replace all white space with single spaces
			if (parserState != "tileset")
			{
				line = ICS_findAndReplace(line, "\t", " ");
				line = ICS_findAndReplace(line, "  ", " ");
			}

			// check for loading the texture
			if (line.length() > 0 && parserState == "tileset")
			{
				fileName = line;
				parserState = "dimensions";	// advance the parser to the next state
			}

			// check for getting the texture tile dimensions
			else if (line.length() > 0 && parserState == "dimensions")
			{
				// tokenize the line
				vector<string> tokens = ICS_tokenize(line, ' ');

				// get the texture tile dimensions
				if (tokens.size() == 2)
				{
					// determine the dimensions
					tileWidth = atoi(tokens[0].c_str());
					tileHeight = atoi(tokens[1].c_str());

					// create the tileset
					_tileset = ICS_Tileset::createTileset(fileName, tileWidth, tileHeight);
				}

				// advance the parser to the next state
				parserState = "animation";
			}
			
			// check for adding an animation to the sprite
			else if (line.length() > 0)
			{
				// remove spaces from the frames
				string lineCopy = "";
				int inBrackets = 0;

				// check each character in the line
				for (unsigned int i = 0; i < line.length(); i++)
				{
					// if the character is an opening bracket, start excluding spaces
					if (line[i] == '{')
					{
						inBrackets++;
					}

					// if the character is a closing bracket, start including spaces
					else if (line[i] == '}')
					{
						inBrackets--;
					}

					// include the character if it is not a space in brackets
					else if (line[i] != ' ' || inBrackets <= 0)
					{
						lineCopy += line[i];
					}
				}

				// overwrite the original line with the formatted one
				line = lineCopy;

				// tokenize the line
				vector<string> tokens = ICS_tokenize(line, ' ');

				// validate the data
				if (tokens.size() == 4)
				{
					// to hold the animation frames
					vector<ICS_Pair<int>> frames;

					// start at the first character of the token
					unsigned int i = 0;

					// tokens[1] holds the animation frames
					// parse one character at a time
					while (i < tokens[1].length())
					{
						// if the frame is represented as a (row,column) pair...
						if (tokens[1][i] == '(')
						{
							// skip the opening bracket
							i++;

							// to hold the row and column
							string row = "";
							string column = "";

							// get the row
							while (i < tokens[1].length() and tokens[1][i] != ',')
							{
								row += tokens[1][i];
								i++;
							}

							// skip the comma
							i++;

							// get the column
							while (i < tokens[1].length() and tokens[1][i] != ')')
							{
								column += tokens[1][i];
								i++;
							}

							// add the frame
							frames.push_back(ICS_Pair<int>(atoi(column.c_str()), atoi(row.c_str())));
						}

						// if the frame is represented by a single index...
						else
						{
							// to hold the index
							string index = "";

							// get the index
							while (i < tokens[1].length() and tokens[1][i] != ',')
							{
								index += tokens[1][i];
								i++;
							}

							// add the frame
							frames.push_back(ICS_Pair<int>(atoi(index.c_str()), 0));
						}

						// skip commas and closing brackets
						while (i < tokens[1].length() and (tokens[1][i] == ',' or tokens[1][i] == ')'))
						{
							i++;
						}
					}

					// add the animation to the sprite
					addAnimation(tokens[0], frames, atoi(tokens[2].c_str()), tokens[3] == "true");
				}
			}
		}
	}

	// set the dimensions of the sprite
	setDimensions(width, height);

	// start recieving update events
	ICS_Game::getInstance().addUpdateEventListener(this);
}

/**
 * The copy constructor.
 *
 * @param sprite	The animated sprite to copy.
 */
ICS_AnimatedSprite::ICS_AnimatedSprite(const ICS_AnimatedSprite& sprite)
	:
	ICS_Sprite(sprite),
	_frame(0),
	_elapsed(0),
	_tileset(ICS_Tileset::copyTileset(sprite._tileset)),
	_animations(sprite._animations),
	_currentAnimation(NULL)
{
	// set the current animation
	if (sprite._currentAnimation)
	{
		// play the same animation as the sprite being copied
		playAnimation(sprite._currentAnimation->getName());

		// sync this sprite with the one being copied
		_frame = sprite._frame;
		_elapsed = sprite._elapsed;
		updateTexture();
	}

	// start recieving update events
	ICS_Game::getInstance().addUpdateEventListener(this);
}

/**
 * The destructor.
 */
ICS_AnimatedSprite::~ICS_AnimatedSprite()
{
	// stop recieving update events
	ICS_Game::getInstance().removeUpdateEventListener(this);

	// return the texture to the tileset before the ICS_Sprite destructor tries to delete it
	_texture = NULL;

	// delete the tileset
	if (_tileset)
	{
		ICS_Tileset::deleteTileset(_tileset);
	}
}

/**
 * Assignment operator.
 *
 * @param sprite	The animated sprite to copy.
 * 
 * @returns			A reference to this instance.
 */
ICS_AnimatedSprite&
ICS_AnimatedSprite::operator=(const ICS_AnimatedSprite& sprite)
{
	// the texture is no longer valid
	_texture = NULL;

	// delete the old tileset
	if (_tileset)
	{
		ICS_Tileset::deleteTileset(_tileset);
	}

	// invoke the parent assignment operator
	ICS_Sprite::operator=(sprite);

	// set each attribute
	_frame = 0;
	_elapsed = 0;
	_tileset = ICS_Tileset::copyTileset(sprite._tileset);
	_animations = sprite._animations;
	_currentAnimation = NULL;

	// set the current animation
	if (sprite._currentAnimation)
	{
		// play the same animation as the sprite being copied
		playAnimation(sprite._currentAnimation->getName());

		// sync this sprite with the one being copied
		_frame = sprite._frame;
		_elapsed = sprite._elapsed;
		updateTexture();
	}

	// return a reference to this instance
	return *this;
}

/**
 * Plays an animation.
 *
 * @param name	The name of the animation to play.  The name must match the one specified in addAnimation.
 */
void
ICS_AnimatedSprite::playAnimation(string name)
{
	// convert the name to upper case
	name = ICS_toUpperCase(name);

	// make sure the animation exists
	map<string, ICS_Animation>::iterator iterator = _animations.find(name);

	if (iterator != _animations.end() && _currentAnimation != &_animations[name])
	{
		_frame = 0;
		_elapsed = 0;
		_currentAnimation = &_animations[name];

		// gets the appropriate texture from the tileset based on the current animation
		updateTexture();
	}
}

/**
 * Handles update events.
 *
 * @param elapsed	The number of seconds that have elapsed since the last update.
 */
void
ICS_AnimatedSprite::handleUpdateEvent(float elapsed)
{
	// if there is an animation, update it
	if (_currentAnimation)
	{
		// update the elapsed time
		_elapsed += elapsed;

		// keep the elapsed time between 0 and the time for a single loop of the animation
		while (_elapsed > _currentAnimation->getLoopTime())
		{
			_elapsed -= _currentAnimation->getLoopTime();
		}

		// determine the index of the current frame of animation
		_frame = (int)(_elapsed * _currentAnimation->getFramesPerSecond()) % _currentAnimation->getFrameCount();

		// gets the appropriate texture from the tileset based on the current animation
		updateTexture();
	}
}

/**
 * This is used to assist in parsing animation data files.  It removes any comments and trailing white space from a line of text.
 *
 * @param text	The text to remove the comment from.
 *
 * @returns		A copy of the text with the comment and trailing white space removed.
 */
std::string
ICS_AnimatedSprite::removeComment(std::string text)
{
	// look for sequential forward slashes
	unsigned int index = (unsigned int)text.find("//");

	// remove the comment if found
	if (index != string::npos)
	{
		text = text.substr(0, index);
	}

	// remove trailing white space
	index = (unsigned int)text.length();

	while (index > 0 && (text[index - 1] == ' ' || text[index - 1] == '\t'))
	{
		index--;
	}

	text = text.substr(0, index);

	// return the result
	return text;
}

/**
 * Adds an animation to the sprite.
 *
 * @param name				The name to give to the animation... this is needed to play the animation.
 * @param frames			The sequence of frame indexes that make the animation.
 * @param framesPerSecond	The number of frame to render per second.
 * @param loop				Indicates whether or not the animation should loop.
 */
void
ICS_AnimatedSprite::addAnimation(string name, vector<ICS_Pair<int>> frames, int framesPerSecond, bool loop)
{
	// convert the name to upper case
	name = ICS_toUpperCase(name);

	// if the key is already in use, delete the old animation
	map<string, ICS_Animation>::iterator iterator = _animations.find(name);

	// create a new animation and add it to the key
	_animations[name] = ICS_Animation(name, frames, framesPerSecond);
}

/**
 * Gets the appropriate texture from the tileset based on the current animation.
 */
void
ICS_AnimatedSprite::updateTexture()
{
	if (_tileset and _currentAnimation)
	{
		ICS_Pair<int> frame = _currentAnimation->getFrame(_frame);

		_texture = _tileset->getTexture(frame[0], frame[1]);
	}
}
