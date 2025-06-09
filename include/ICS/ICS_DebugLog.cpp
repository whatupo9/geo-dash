#include "ICS_DebugLog.h"		// the declaration of ICS_DebugLog
#include "ICS_Helpers.h"		// for ICS_toUpperCase

#include <chrono>			// for getting the date and time
#include <ctime>			// for getting the date and time
#include <string>			// for std::string

/**
 * This class use a singleton.  Only one instance can exist.
 * This function returns a reference to the instance.
 *
 * @returns		The singleton instance of ICS_DebugLog.
 */
ICS_DebugLog&
ICS_DebugLog::getInstance()
{
	static ICS_DebugLog _instance;

	return _instance;
}

/**
 * Private constructor.  The class can only be accessed by using getInstance.
 */
ICS_DebugLog::ICS_DebugLog()
	:
	_initialized(false)
{
}

/**
 * Opens the log file and records the date and time.
 */
void
ICS_DebugLog::startLogging()
{
	// make sure logging hasn't already been started
	if (not _initialized)
	{
		// check if the log already exists
		bool logExists = false;
		std::ifstream testFile("Debug Log.txt");

		if (testFile.is_open())
		{
			logExists = true;
			testFile.close();
		}

		// open the log file
		_errorFile.open("Debug Log.txt", std::ios_base::app);

		// make sure the log file was opened successfully
		if (_errorFile.is_open())
		{
			// get the current date and time
			std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			char buffer[30];
			ctime_s(buffer, sizeof(buffer), &currentTime);

			std::string dateAndTime = buffer;

			// leave space between this report and the previous one
			if (logExists)
			{
				_errorFile << std::endl;
			}

			// print the date and time to the error file
			_errorFile << dateAndTime;

			// underline the date and time
			for (unsigned int i = 0; i < dateAndTime.length(); i++)
			{
				_errorFile << "-";
			}

			_errorFile << std::endl;
		}

		// error logging has begun
		_initialized = true;
	}
}

/**
 * For logging an message.  The information is reported to the error log file.
 *
 * @param type			The type of message to log (error or debug).
 * @param fileName		The name of the file that reported the error.
 * @param lineNumber	The line number that the error was reported on.
 * @param errorMessage	The details of the error.
 */
void
ICS_DebugLog::logMessage(std::string type, std::string fileName, int lineNumber, std::string errorMessage)
{
	// start error logging if neccessary
	startLogging();

	// make sure the log file is open
	if (_errorFile.is_open())
	{
		// make the file name all caps
		fileName = ICS_toUpperCase(fileName);

		// log the error
		_errorFile << type << " - " << fileName << " - LINE " << lineNumber << " - " << errorMessage << std::endl;
	}
}