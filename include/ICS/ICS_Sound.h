/*

ICS_Sound

	Created: ???

	Change log:

		2023-10-18
			- invalid sound id was set to 0 which should be valid.
			- changed invalid sound id to -1
			- changed all sound ids to int (instead of unsigned int)

		2023-12-07
			- added thread to update FMOD system

*/

#pragma once

#include "fmod.hpp"			// FMOD sound system

#include <string>			// for std::string

// define the type for sound callbacks (void, no parameters)
typedef void(*ICS_SoundCallbackFunction)();

const int ICS_INVALID_SOUND_ID = -1;	// represents an invalid sound id
const float ICS_MIN_VOLUME = 0.0f;		// the minimum volume level
const float ICS_MAX_VOLUME = 100.0f;	// the maximum volume level

/**
 * For playing sounds.
 */
class ICS_Sound
{

// member attributes

private:

	static FMOD_SYSTEM* _fmodSystem;	// an FMOD sound system

public:

// constructor / destructor

	/**
	 * ICS_Sound constructor.
	 */
	ICS_Sound();

public:

	/**
	 * This plays the indicated sound file using FMOD.
	 *
	 * @param fileName	The name of the file containing the sound to play.
	 * @param loop		If this is true, the sound will loop.  If it is false, the sound will not loop.
	 *
	 * @returns			The id of the sound.  This can be used to pause or play the sound.
	 */
	int streamSound(std::string fileName, bool loop = false);

	/**
	 * This plays the indicated sound file using FMOD.
	 *
	 * @param fileName	The name of the file containing the sound to play.
	 * @param callback	The function to call when the sound finishes playing.
	 *
	 * @returns			The id of the sound.  This can be used to pause or play the sound.
	 */
	int streamSound(std::string fileName, ICS_SoundCallbackFunction callback);

	/**
	 * Stops a sound.
	 *
	 * @param soundId	The id of the sound to stop.
	 *
	 * @returns			true if succssful, false otherwise.
	 */
	bool stopSound(int soundId);

	/**
	 * This pauses the sound associated with the provided sound id.
	 *
	 * @param soundId	The id of the sound to pause.
	 *
	 * @returns			true if succssful, false otherwise.
	 */
	bool pauseSound(int soundId);

	/**
	 * This unpauses the sound associated with the provided sound id.
	 *
	 * @param soundId	The id of the sound to unpause.
	 *
	 * @returns			true if succssful, false otherwise.
	 */
	bool unpauseSound(int soundId);

	/**
	 * This adjusts the volume of the sound associated with the provided sound id.
	 *
	 * @param soundId	The id of the sound.
	 * @param volume	The volume level of the sound (0 is the minimum, 100 is max volume)
	 *
	 * @returns			true if succssful, false otherwise.
	 */
	bool setSoundVolume(int soundId, float volume);

// inquiry

	/**
	 * Checks if a sound is playing or not.
	 *
	 * @param soundId	The id of the sound.
	 *
	 * @returns			true if the sound is playing, false otherwise.
	 */
	bool isPlaying(int soundId);

};
