/*

ICS_DebugLog

	Created: 2024-05-18

	Change log:

		2024-05-18
			- added error reporting / debugging module that can be used without a console window or Windows specific code
			- all messages are logged in an output file with file name and line number automatically determined
			- use the macro ICS_LOG_ERROR(errorMessage) to log an error
			- use the macro ICS_LOG_MESSAGE(message) to log a debug message

*/

#pragma once

#include <fstream>	// for ofstream

/**
 * A singleton class for logging errors and debug messages.
 **/
class ICS_DebugLog
{

private:

	// indicates the log has been started
	bool _initialized;

	// the file to which all messages are logged
	std::ofstream _errorFile;

public:

// singleton

	/**
	 * This class use a singleton.  Only one instance can exist.
	 * This function returns a reference to the instance.
	 *
	 * @returns		The singleton instance of ICS_DebugLog.
	 */
	static ICS_DebugLog& getInstance();

private:

// constructors

	/**
	 * Private constructor.  The class can only be accessed by using getInstance.
	 */
	ICS_DebugLog();

	/**
	 * Delete the copy constructor (only 1 instance can exist).
	 */
	ICS_DebugLog(const ICS_DebugLog& error) = delete;

	/**
	 * Opens the log file and records the date and time.
	 */
	void startLogging();

public:

	/**
	 * For logging an message.  The information is reported to the error log file.
	 *
	 * @param type			The type of message to log (error or debug).
	 * @param fileName		The name of the file that reported the error.
	 * @param lineNumber	The line number that the error was reported on.
	 * @param errorMessage	The details of the error.
	 */
	void logMessage(std::string type, std::string fileName, int lineNumber, std::string errorMessage);

};

// for stripping the path off a file name
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// errors can be reported with ICS_LOG_ERROR(message) and the file name and line number will be added automatically
#define ICS_LOG_ERROR(message) ICS_DebugLog::getInstance().logMessage("ERROR", __FILENAME__, __LINE__, message)

// debug messages can be reported with ICS_LOG_MESSAGE(message) and the file name and line number will be added automatically
#define ICS_LOG_MESSAGE(message) ICS_DebugLog::getInstance().logMessage("DEBUG", __FILENAME__, __LINE__, message)