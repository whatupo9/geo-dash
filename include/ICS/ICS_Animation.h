/*

ICS_AnimatedSprite

	Created: ???

	Change log:

		2024-02-09
			- animation frames are now ordered pairs (x, y)

		2024-06-06
			- added a default constructor (so ICS_AnimatedSprite can create a map of animations, rather than pointers)
			- added a name attribute (needed for ICS_AnimatedSprite copy constructor)
			- made all getters constant

*/
#pragma once

#include <vector>		// for std::vector (used to store animation frames)
#include <string>		// for std::string (to represent the animation's name)
#include "ICS_Pair.h"	// for ICS_Pair (used to store animation frames)

class ICS_Animation
{

private:

// member attributes

	std::string					_name;				// The name of the animation.
	std::vector<ICS_Pair<int>>	_frames;			// The sequence of frame indexes that make the animation.
	int							_framesPerSecond;	// The number of frame to render per second.

public:

// constructors

	/**
	 * The default constructor.
	 */
	ICS_Animation();

	/**
	 * The constructor
	 *
	 * @param name				The name of the animation.
	 * @param frames			The sequence of frame indexes that make the animation.
	 * @param framesPerSecond	The number of frame to render per second.
	 */
	ICS_Animation(std::string name, std::vector<ICS_Pair<int>> frames, int framesPerSecond);

// getters

	/**
	 * Gets the animation's name.
	 * 
	 * @returns		The name of the animation.
	 */
	std::string getName() const
	{
		return _name;
	}

	/**
	 * Gets the animation frame corresponding to the frame index.
	 *
	 * @param index		The index of the frame in the animation sequence.
	 *
	 * @returns			The frame number (referring to its location in the texture).
	 */
	ICS_Pair<int> getFrame(unsigned int index) const;

	/**
	 * Gets the number of frames in the animation sequence.
	 *
	 * @returns		The number of frames in the animation sequence.
	 */
	int getFrameCount() const
	{
		return (int)_frames.size();
	}

	/**
	 * Gets the number of frame to render per second.
	 *
	 * @returns		The number of frame to render per second.
	 */
	int getFramesPerSecond() const
	{
		return _framesPerSecond;
	}

	/**
	 * Gets the number of seconds it takes to render a full loop of the animation.
	 *
	 * @returns		The number of seconds it takes to render a full loop of the animation.
	 */
	float getLoopTime() const
	{
		return (1.0f / _framesPerSecond) * _frames.size();
	}

};