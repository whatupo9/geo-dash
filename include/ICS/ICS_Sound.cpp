#include "ICS_Sound.h"					// the declaration of ICS_Sound
#include "ICS_Helpers.h"				// for ICS_clamp

#include <map>							// for mapping sound channels to callback functions
#include <vector>						// for storing all active sound channels
#include <Windows.h>					// for creating a thread to update the FMOD system
#include <time.h>						// for updating the FMOD system at a consistent rate

#pragma comment (lib, "fmod_vc.lib")	// tells the linker to link the FMOD library

std::vector<FMOD_CHANNEL*> ICS_SoundChannels;									// all channels created for playing sounds
std::map<FMOD_CHANNEL*, ICS_SoundCallbackFunction> ICS_SoundChannelCallbackMap;	// for mapping channels to callbacks

// initialize the static member for the FMOD system
FMOD_SYSTEM* ICS_Sound::_fmodSystem = NULL;

// this thread is used to update the FMOD system every "frame"
DWORD WINAPI updateThread(LPVOID lpParameter)
{
	// get the FMOD system
	FMOD_SYSTEM* fmodSystem = (FMOD_SYSTEM*)lpParameter;

	// determines how many times per second to update
	const int FPS = 30;

	// start the clock
	clock_t lastTime = clock();

	// loop forever
	while (true)
	{
		// determine how much time has elapsed since the last update
		clock_t currentTime = clock();
		clock_t  elapsed = currentTime - lastTime;

		// limit the framerate
		if (elapsed > CLK_TCK / FPS)
		{
			// update the time for the next call
			lastTime = currentTime;

			// update the FMOD system
			FMOD_System_Update(fmodSystem);
		}
	}

	// unreachable... satisfying the return type of the function
	return 0;
}

/**
 * The callback for channel events.
 *
 * @param chanControl	The object that triggered the event.
 * @param controlType	Indicates the type of object that triggered the event
 * @param callbackType	Indicates the type of event.
 */
FMOD_RESULT F_CALLBACK ICS_SoundEventCallback(FMOD_CHANNELCONTROL* chanControl, FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void*, void*)
{
	// handle channel playback ending events
	if (controlType == FMOD_CHANNELCONTROL_CHANNEL && callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
	{
		// get the channel
		FMOD_CHANNEL* channel = (FMOD_CHANNEL*)chanControl;

		// check if it is mapped to a callback
		if (ICS_SoundChannelCallbackMap.find(channel) != ICS_SoundChannelCallbackMap.end())
		{
			// invoke the callback
			ICS_SoundChannelCallbackMap[channel]();

			// delete the callback from the map
			ICS_SoundChannelCallbackMap.erase(channel);
		}
	}

	// success
	return FMOD_OK;
}

/**
 * The default constructor.
 */
ICS_Sound::ICS_Sound()
{
	// check if the FMOD system is already initialized
	if (_fmodSystem == NULL)
	{
		// initialize the sound system
		FMOD_System_Create(&_fmodSystem);
		FMOD_System_Init(_fmodSystem, 32, FMOD_INIT_NORMAL, 0);

		DWORD threadID;
		CreateThread(0, 0, updateThread, _fmodSystem, 0, &threadID);
	}
}

/**
 * This plays the indicated sound file using FMOD.
 *
 * @param fileName	The name of the file containing the sound to play.
 * @param loop		If this is true, the sound will loop.  If it is false, the sound will not loop.
 *
 * @returns			The id of the sound.  This can be used to pause or play the sound.
 */
int
ICS_Sound::streamSound(std::string fileName, bool loop)
{
	// create the sound
	FMOD_SOUND* sound;
	FMOD_System_CreateStream(_fmodSystem, fileName.c_str(), loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF, 0, &sound);

	// play the sound
	FMOD_CHANNEL* channel;
	FMOD_System_PlaySound(_fmodSystem, sound, 0, false, &channel);

	// add the channel to the list of all sound channels
	ICS_SoundChannels.push_back(channel);

	// use the number of exisiting channels to determine the sound id
	return (int)(ICS_SoundChannels.size() - 1);
}

/**
 * This plays the indicated sound file using FMOD.
 *
 * @param fileName	The name of the file containing the sound to play.
 * @param callback	The function to call when the sound finishes playing.
 *
 * @returns			The id of the sound.  This can be used to pause or play the sound.
 */
int
ICS_Sound::streamSound(std::string fileName, ICS_SoundCallbackFunction callback)
{
	// play the sound
	int soundId = streamSound(fileName, false);

	// get the channel for the sound
	FMOD_CHANNEL* channel = ICS_SoundChannels[soundId];

	// set the callback for the sound
	if (channel)
	{
		ICS_SoundChannelCallbackMap[channel] = callback;
		FMOD_Channel_SetCallback(channel, ICS_SoundEventCallback);
	}

	// return the sound id
	return soundId;
}

/**
 * Stops a sound.
 *
 * @param soundId	The id of the sound to stop.
 *
 * @returns			true if succssful, false otherwise.
 */
bool
ICS_Sound::stopSound(int soundId)
{
	// make sure the sound id is valid
	if (soundId != ICS_INVALID_SOUND_ID and soundId < (int)ICS_SoundChannels.size())
	{
		// stop the sound
		FMOD_Channel_Stop(ICS_SoundChannels[soundId]);
		return true;
	}

	// couldn't stop the sound (the id was invalid)
	return false;
}

/**
 * This pauses the sound associated with the provided sound id.
 *
 * @param soundId	The id of the sound to pause.
 *
 * @returns			true if succssful, false otherwise.
 */
bool
ICS_Sound::pauseSound(int soundId)
{
	// make sure the sound id is valid
	if (soundId != ICS_INVALID_SOUND_ID and soundId < (int)ICS_SoundChannels.size())
	{
		// pause the sound
		FMOD_Channel_SetPaused(ICS_SoundChannels[soundId], true);
		return true;
	}
	
	// couldn't pause the sound (the id was invalid)
	return false;
}

/**
 * This unpauses the sound associated with the provided sound id.
 *
 * @param soundId	The id of the sound to unpause.
 *
 * @returns			true if succssful, false otherwise.
 */
bool
ICS_Sound::unpauseSound(int soundId)
{
	// make sure the sound id is valid
	if (soundId != ICS_INVALID_SOUND_ID and soundId < (int)ICS_SoundChannels.size())
	{
		// unpause the sound
		FMOD_Channel_SetPaused(ICS_SoundChannels[soundId], false);
		return true;
	}
	
	// couldn't unpause the sound (the id was invalid)
	return false;
}

/**
 * This adjusts the volume of the sound associated with the provided sound id.
 *
 * @param soundId	The id of the sound.
 * @param volume	The volume level of the sound (0 is the minimum, 100 is max volume)
 *
 * @returns			true if succssful, false otherwise.
 */
bool
ICS_Sound::setSoundVolume(int soundId, float volume)
{
	// make sure the volume is valid
	ICS_clamp(volume, ICS_MIN_VOLUME, ICS_MAX_VOLUME);

	// convert from percent to decimal
	volume /= ICS_MAX_VOLUME;

	// make sure the sound id is valid
	if (soundId != ICS_INVALID_SOUND_ID and soundId < (int)ICS_SoundChannels.size())
	{
		// adjust the volume
		FMOD_Channel_SetVolume(ICS_SoundChannels[soundId], volume);
		return true;
	}
	
	// couldn't adjust the volume (the id was invalid)
	return false;
}

/**
 * Checks if a sound is playing or not.
 *
 * @param soundId	The id of the sound.
 *
 * @returns			true if the sound is playing, false otherwise.
 */
bool
ICS_Sound::isPlaying(int soundId)
{
	// make sure the sound id is valid
	if (soundId != ICS_INVALID_SOUND_ID and soundId < (int)ICS_SoundChannels.size())
	{
		// check if the sound is playing and return the result
		FMOD_BOOL result = false;
		FMOD_Channel_IsPlaying(ICS_SoundChannels[soundId], &result);
		return (bool)result;
	}
	
	// the sound isn't playing since the id isn't valid
	return false;
}