/*

ICS_Server

	Created: ???

	Change log:

		2024-02-17
			- inherits from ICS_EventListener so it can receive update events

*/

#pragma once

#include "ICS_EventListener.h"			// ICS_Server inherits from ICS_EventListener

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>					// for Windows internet communication
#include <mutex>						// for protecting variables in threads
#include <string>						// for std::string
#include <list>							// for std::list data structure
#include <vector>						// the definition of std::vector (a variable length array)

/**
 * Models a web server.  Allows for client connections and communication over the internet.
 */
class ICS_Server: public ICS_EventListener
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
		unsigned int clientNumber;	// the id of the client that is associated with the event
		std::string data;			// data received from the client (if applicable)
	};

	std::list<EventData> _events;	// all events that have occured since the last update
	std::mutex _eventsMutex;		// for protecting the event list in threads

// attrbutes
private:

	std::string _port = "";						// the port to communicate over
	unsigned int _bufferLength = 4096;			// the max size of data received from a client
	bool _stopping = false;						// indicates the server is in the process of stopping
	bool _stopped = true;						// indicates the server is stopped

	SOCKET* _clientSockets = nullptr;			// all client sockets
	std::string* _peerNames = nullptr;			// the peer name of each client
	unsigned int _clients = 0;					// the number of clients that have connected
	unsigned int _clientNumber = 0;				// for the stupid lamda expression thing... total hack
	unsigned int _maxClients = 0;				// the maximum number of clients that can connect at once
	std::mutex _clientSocketsMutex;				// for protecting the cleint sockets in threads

	addrinfo* _addressInfo = nullptr;			// holds host address information
	std::string _hostName = "";					// the name of the host (the device that the server is running on)
	std::vector<std::string> _IPAddresses;		// the IP addresses of the the host

	std::thread _listenForClientsThread;		// a thread for listening for client connections
	std::thread* _clientThreads = nullptr;		// all client threads
	std::mutex _clientThreadsMutex;				// for protecting access to the client threads

	void(*_clientConnectCallback)(unsigned int) = nullptr;						// the callback for client connection events
	void(*_clientDisconnectCallback)(unsigned int) = nullptr;					// the callback for client disconnection events
	void(*_receiveDataCallback)(unsigned int, const std::string&) = nullptr;	// the callback for receiving data from a client

// constructor / destructor
public:

	/**
	 * ICS_Server constructor.
	 *
	 * @param port			The port to communicate over.
	 * @param maxClients	The maximum number of clients that can connect at once.
	 */
	ICS_Server(std::string port, unsigned int maxClients);

	/**
	 * ICS_Server destructor.
	 */
	~ICS_Server();

// getters

	/**
	 * Checks if the server is running.
	 *
	 * @returns		true if the server is running, false otherwise
	 */
	bool isRunning()
	{
		return !_stopping && !_stopped;
	}

	/**
	 * Returns the number of IP addresses for the server.
	 *
	 * @returns		the number of IP addresses for the server.
	 */
	unsigned int getIPAddressCount()
	{
		return (unsigned int)_IPAddresses.size();
	}

	/**
	 * Gets an IP address for the server.
	 *
	 * @param i		The index of the IP address (between 0 and one less than the number of addresses).
	 *
	 * @returns		An IP address for the server.
	 */
	std::string getIPAddress(unsigned int i);

// callback setters

	/**
	 * Sets the callback for handling client connection events.
	 *
	 * @param clientConnectCallback		The callback for handling client connection events.
	 *									The function must match the return type and parameter list of this prototype:
	 *
	 *									void functionName(unsigned int clientNumber)
	 */
	void setClientConnectCallback(void(*clientConnectCallback)(unsigned int))
	{
		_clientConnectCallback = clientConnectCallback;
	}

	/**
	 * Sets the callback for handling client disconnection events.
	 *
	 * @param clientDisconnectCallback	The callback for handling client disconnection events.
	 *									The function must match the return type and parameter list of this prototype:
	 *
	 *									void functionName(unsigned int clientNumber)
	 */
	void setClientDisconnectCallback(void(*clientDisconnectCallback)(unsigned int))
	{
		_clientDisconnectCallback = clientDisconnectCallback;
	}

	/**
	 * Sets the callback for handling receiving data from a client.
	 *
	 * @param receiveDataCallback		The callback for handling receiving data from a client.
	 *									The function must match the return type and parameter list of this prototype:
	 *
	 *									void functionName(unsigned int clientNumber, const std::string data)
	 */
	void setReceiveDataCallback(void(*receiveDataCallback)(unsigned int, const std::string&))
	{
		_receiveDataCallback = receiveDataCallback;
	}

private:

// event handlers

	/**
	 * Handles update events.  Allows the server to dispatch events.
	 *
	 * @param elapsed	The number of seconds that have elapsed since the last update.
	 */
	void handleUpdateEvent(float elapsed);

public:

// operations

	/**
	 * Attempts to start the server.
	 *
	 * @returns		true if the server was successfully started or already running, false otherwise
	 */
	bool start();

	/**
	 * Attempts to stop the server.
	 *
	 * @returns		true if the process of stopping the server was successful or already in process, false otherwise.
	 */
	bool stop();

	/**
	 * Attempts to send data to a specific client.
	 *
	 * @param clientNumber	The id of the client to send the data to.
	 * @param data			The data to send.
	 * @param dataLength	The length of the data, in bytes.
	 *
	 * @returns				true if successful, false otherwise.
	 */
	bool sendData(unsigned int clientNumber, const char* data, int dataLength);

	/**
	 * Attempts to send data to a specific client.
	 *
	 * @param clientNumber	The id of the client to send the data to.
	 * @param data			The data to send.
	 *
	 * @returns				true if successful, false otherwise.
	 */
	bool sendData(unsigned int clientNumber, const std::string& data)
	{
		return sendData(clientNumber, data.c_str(), (int)data.length());
	}

	/**
	 * Attempts to send data to all connected clients.
	 *
	 * @param data			The data to send.
	 * @param dataLength	The length of the data, in bytes.
	 *
	 * @returns				true if successful, false otherwise.
	*/
	bool broadcastData(const char* data, int dataLength);

	/**
	 * Attempts to send data to all connected clients.
	 *
	 * @param data	The data to send.
	 *
	 * @returns		true if successful, false otherwise.
	*/
	bool broadcastData(const std::string& data)
	{
		return broadcastData(data.c_str(), (int)data.length());
	}

	/**
	 * Attempts to disconnect a specific client.
	 *
	 * @param clientNumber	The id of the client to disconnect.
	 *
	 * @returns				true if the client was disconnected or was not connected, false otherwise.
	 */
	bool disconnectClient(unsigned int clientNumber);

// thread functions
private:

	/**
	 * Listens for data sent from the specified client and generates an event when data is received.
	 *
	 * @param clientNumber	The id of the client.
	 */
	void listenForData(unsigned int clientNumber);

	/**
	 * Listens for clients connecting to the server.
	 */
	void listenForClients();

// helpers
private:

	/**
	 * Determines the host name for the server.
	 */
	void retrieveHostName();

	/**
	 * Determines the IP addresses for the server.
	 */
	void retrieveIPAddresses();

	/**
	 * Frees memory allocated for the address info
	 */
	void freeAddressInfo();

	/**
	 * Adds an event to be dispatched later.
	 *
	 * @param type				The type of event.
	 * @param clientNumber		The id of the client associated with the event.
	 * @param data				The data received from the client.
	 */
	void addEventData(EventType type, unsigned int clientNumber, std::string data = "");

	/**
	 * Attempts get event data from the queue.
	 *
	 * @param data		Holds the result of the function call.
	 *
	 * @returns			true if there was data in the queue, false otherwise.
	 */
	bool getEventData(EventData& data);

};