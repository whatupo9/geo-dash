#include "ICS_Animation.h"

/**
 * The default constructor.
 */
ICS_Animation::ICS_Animation()
	:
	_name(""),
	_framesPerSecond(0)
{
}

/**
 * The constructor
 *
 * @param name				The name of the animation.
 * @param frames			The sequence of frame indexes that make the animation.
 * @param framesPerSecond	The number of frame to render per second.
 */
ICS_Animation::ICS_Animation(std::string name, std::vector<ICS_Pair<int>> frames, int framesPerSecond)
	:
	_name(name),
	_frames(frames),
	_framesPerSecond(framesPerSecond)
{
}

/**
 * Gets the animation frame corresponding to the frame index.
 *
 * @param index		The index of the frame in the animation sequence.
 *
 * @returns			The frame number (referring to its location in the texture).
 */
ICS_Pair<int>
ICS_Animation::getFrame(unsigned int index) const
{
	// return the frame number if the index is valid
	if (index >= 0 && index < _frames.size())
	{
		return _frames[index];
	}

	// if the index is not valid, return 0
	return ICS_Pair<int>(0, 0);
}