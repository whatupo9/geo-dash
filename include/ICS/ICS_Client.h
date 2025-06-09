/*

ICS_Client

	Created: ???

	Change log:

		2024-02-17
			- inherits from ICS_EventListener so it can receive update events

		2024-06-06
			- made "address info" be a local variable in the connectToServer function

*/

#pragma once

#include "ICS_EventListener.h"	// ICS_Client inherits from ICS_EventListener (so it can receive update events)

#include <winsock2.h>			// for Windows internet communication
#include <mutex>				// for protecting variables in threads
#include <string>				// for std::string
#include <list>					// for std::list data structure

/**
 * Models a web client.  Allows for server connection and communication over the internet.
 */
class ICS_Client: public ICS_EventListener
{

// event data
private:

	/**
	 * Different types of server events.
	 */
	enum EventType { CONNECTION_EVENT, DISCONNECTION_EVENT, RECEIVE_DATA_EVENT };

	/**
	 * Represent an event that occured.
	 */
	struct EventData
	{
		EventType type;				// the type of event
		std::string data;			// data received from the client (if applicable)
	};

	std::list<EventData> _events;	// all events that have occured since the last update
	std::mutex _eventsMutex;		// for protecting the event list in threads

// attrbutes
private:

	unsigned int _bufferLength = 4096;		// the max size of data received from a client

	SOCKET _socket = INVALID_SOCKET;		// for connecting to the server
	std::string _serverAddress = "";		// the address of the server
	std::string _serverPort = "";			// the port that the server is using

	bool _connecting = false;				// indicates that the client is attemting to connect to the server
	std::mutex _connectMutex;				// for locking the connecting flag for read / write access
	std::thread _connectThread;				// a thread for listening for data from the server

	std::thread _listenForDataThread;		// a thread for listening for data from the server

	void(*_connectCallback)() = nullptr;							// the callback for connection events
	void(*_disconnectCallback)() = nullptr;							// the callback for disconnection events
	void(*_receiveDataCallback)(const std::string& data) = nullptr;	// the callback for receiving data from the server

// constructor / destructor

public:

	/**
	 * ICS_Client constructor.
	 */
	ICS_Client();

	/**
	 * ICS_Client destructor.
	 */
	~ICS_Client();

// callback setters
public:

	/**
	* Sets the callback for handling connection events.
	*
	* @param connectCallback		The callback for handling connection events.
	*								The function must match the return type and parameter list of this prototype:
	*
	*								void functionName()
	*/
	void setConnectCallback(void(*connectCallback)())
	{
		_connectCallback = connectCallback;
	}

	/**
	* Sets the callback for handling disconnection events.
	*
	* @param disconnectCallback		The callback for handling disconnection events.
	*								The function must match the return type and parameter list of this prototype:
	*
	*								void functionName()
	*/
	void setDisconnectCallback(void(*disconnectCallback)())
	{
		_disconnectCallback = disconnectCallback;
	}

	/**
	 * Sets the callback for handling receiving data from the server.
	 *
	 * @param receiveDataCallback	The callback for handling receiving data from the server.
	 *								The function must match the return type and parameter list of this prototype:
	 *
	 *								void functionName(const std::string&)
	 */
	void setReceiveDataCallback(void(*receiveDataCallback)(const std::string&))
	{
		_receiveDataCallback = receiveDataCallback;
	}

private:

// event handlers

	/**
	 * Handles update events.  Allows the client to dispatch events.
	 *
	 * @param elapsed	The number of seconds that have elapsed since the last update.
	 */
	void handleUpdateEvent(float elapsed);

// operations
public:

	/**
	 * Attempts to connect to the server at the specified address.
	 *
	 * @param address	The IP address of the server.
	 * @param port		The port to communicate over.
	 *
	 * @returns		true if the client connected or was already connected, false otherwise.
	 */
	void connectToServer(std::string address, std::string port);

	/**
	 * Attempts to disconnect from the server.
	 *
	 * @returns		true if the client disconnected or was already disconnected, false otherwise.
	 */
	bool disconnectFromServer();

	/**
	 * Attempts to send data to the server.
	 *
	 * @param data			The data to send.
	 * @param dataLength	The length of the data, in bytes.
	 *
	 * @returns				true if successful, false otherwise.
	 */
	bool sendData(const char* data, int dataLength);

	/**
	 * Attempts to send data to the server.
	 *
	 * @param data			The data to send.
	 * @param dataLength	The length of the data, in bytes.
	 *
	 * @returns				true if successful, false otherwise.
	 */
	bool sendData(const std::string& data)
	{
		return sendData(data.c_str(), (int)data.length());
	}

// thread functions
private:

	/**
	 * Attempts to connect to the server at the specified address.
	 *
	 * @returns		true if the client connected or was already connected, false otherwise.
	 */
	void connectToServer();

	/**
	 * Checks if the client is attempting to connect.
	 *
	 * @returns		true if the client is currently connecting, false otherwise.
	 */
	bool isConnecting()
	{
		_connectMutex.lock();
		bool result = _connecting;
		_connectMutex.unlock();

		return result;
	}

	/**
	 * Sets the onnecting flag.
	 *
	 * @param value		true if the client is currently connecting, false otherwise.
	 */
	void setConnecting(bool value)
	{
		_connectMutex.lock();
		_connecting = value;
		_connectMutex.unlock();
	}

	/**
	 * Listens for data sent from the server and generates an event when data is received.
	 */
	void listenForData();

// helpers
private:

	/**
	 * Adds an event to be dispatched later.
	 *
	 * @param type		The type of event.
	 * @param data		The data received from the server.
	 */
	void addEventData(EventType type, std::string data = "");

	/**
	 * Attempts get event data from the queue.
	 *
	 * @param data		Holds the result of the function call.
	 *
	 * @returns			true if there was data in the queue, false otherwise.
	 */
	bool getEventData(EventData& data);

};