#include "ICS_Game.h"	// the declaration of ICS_Game
#include "ICS_Client.h"	// the declaration of ICS_Client

#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

/**
 * ICS_Client constructor.
 */
ICS_Client::ICS_Client()
{
	// start recieving update events
	ICS_Game::getInstance().addUpdateEventListener(this);
}

/**
 * ICS_Client destructor.
 */
ICS_Client::~ICS_Client()
{
	// stop recieving update events
	ICS_Game::getInstance().removeUpdateEventListener(this);

	// disconnect from the server if connected
	disconnectFromServer();
}

/**
 * Handles update events.  Allows the client to dispatch events.
 *
 * @param elapsed	The number of seconds that have elapsed since the last update.
 */
void
ICS_Client::handleUpdateEvent(float elapsed)
{
	// for handling an event
	EventData data;

	// keep getting event data until there are no more events
	while (getEventData(data))
	{
		// handle a connection event
		if (data.type == CONNECTION_EVENT && _connectCallback)
		{
			_connectCallback();
		}

		// handle a disconnection event
		if (data.type == DISCONNECTION_EVENT && _disconnectCallback)
		{
			_disconnectCallback();
		}

		// handle recieving data
		if (data.type == RECEIVE_DATA_EVENT && _receiveDataCallback)
		{
			_receiveDataCallback(data.data);
		}
	}
}

/**
 * Attempts to connect to the server at the specified address.
 *
 * @param address	The IP address of the server.
 * @param port		The port to communicate over.
 *
 * @returns			true if the client connected or was already connected, false otherwise.
 */
void
ICS_Client::connectToServer(std::string address, std::string port)
{
	// check if the client is currently connecting or there is already a connection
	if (isConnecting() || _socket != INVALID_SOCKET)
	{
		return;
	}

	// wait for the connect thread to complete
	if (_connectThread.joinable())
	{
		_connectThread.join();
	}
	
	// set connection attributes
	setConnecting(true);
	_serverAddress = address;
	_serverPort = port;

	// start the thread to connect
	_connectThread = std::thread([this] { connectToServer(); });
}

/**
 * Attempts to disconnect from the server.
 *
 * @returns		true if the client was disconnected or was not connected, false otherwise.
 */
bool
ICS_Client::disconnectFromServer()
{
	// wait for the connect thread to complete
	if (_connectThread.joinable())
	{
		_connectThread.join();
	}

	// if the socket is valid, close the connection
	if (_socket != INVALID_SOCKET)
	{
		shutdown(_socket, SD_SEND);
	}

	// wait for the data thread to complete
	if (_listenForDataThread.joinable())
	{
		_listenForDataThread.join();
	}

	// return the result
	return true;
}

/**
 * Attempts to send data to the server.
 *
 * @param data			The data to send.
 * @param dataLength	The length of the data, in bytes.
 *
 * @returns		true if successful, false otherwise.
 */
bool
ICS_Client::sendData(const char* data, int dataLength)
{
	// attempt to send the data
	if (isConnecting() || _socket == INVALID_SOCKET || send(_socket, data, dataLength, 0) == SOCKET_ERROR)
	{
		return false;
	}

	// success
	return true;
}

/**
* Attempts to connect to the server at the specified address.
*
* @returns		true if the client connected or was already connected, false otherwise.
*/
void
ICS_Client::connectToServer()
{
	// initialize Winsock
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		_serverAddress = "";
		_serverPort = "";

		addEventData(DISCONNECTION_EVENT);

		setConnecting(false);

		return;
	}

	// generate hints about what type of sockets to support
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));	// wipe memory
	hints.ai_family = AF_UNSPEC;		// unspecified
	hints.ai_socktype = SOCK_STREAM;	// stream socket
	hints.ai_protocol = IPPROTO_TCP;	// use TCP protocol

	addrinfo* addressInfo = nullptr;	// holds host address information

	// resolve the server address and port
	if (getaddrinfo(_serverAddress.c_str(), _serverPort.c_str(), &hints, &addressInfo) != 0)
	{
		_serverAddress = "";
		_serverPort = "";

		// free memory allocated for the address info
		if (addressInfo)
		{
			freeaddrinfo(addressInfo);
			addressInfo = NULL;
		}

		WSACleanup();

		addEventData(DISCONNECTION_EVENT);

		setConnecting(false);

		return;
	}

	// attempt to connect to an address until one succeeds
	for (addrinfo* ptr = addressInfo; ptr != NULL && _socket == INVALID_SOCKET; ptr = ptr->ai_next)
	{
		// create a SOCKET for connecting to server
		_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		// verify the socket
		if (_socket == INVALID_SOCKET)
		{
			_serverAddress = "";
			_serverPort = "";

			// free memory allocated for the address info
			if (addressInfo)
			{
				freeaddrinfo(addressInfo);
				addressInfo = NULL;
			}

			WSACleanup();

			addEventData(DISCONNECTION_EVENT);

			setConnecting(false);

			return;
		}

		// connect to the server
		if (connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(_socket);
			_socket = INVALID_SOCKET;
		}
	}

	// free memory allocated for the address info
	if (addressInfo)
	{
		freeaddrinfo(addressInfo);
		addressInfo = NULL;
	}

	// verify the server socket
	if (_socket == INVALID_SOCKET)
	{
		_serverAddress = "";
		_serverPort = "";

		WSACleanup();

		addEventData(DISCONNECTION_EVENT);

		setConnecting(false);

		return;
	}

	// connection complete
	setConnecting(false);

	// add the event to the queue to be dispatched later
	addEventData(CONNECTION_EVENT);

	// start listening for data
	_listenForDataThread = std::thread([this] { listenForData(); });
}

/**
 * Listens for data sent from the server and generates an event when data is received.
 */
void
ICS_Client::listenForData()
{
	// allocate memory for receiving data
	char* buffer = new char[_bufferLength];

	// indicates that the client is disconnected
	bool disconnected = false;

	// keep listening until the client is disconnected
	while (!disconnected)
	{
		// wipe the buffer
		ZeroMemory(buffer, _bufferLength);

		// wait for data from the client
		int bytesReceived = recv(_socket, buffer, _bufferLength, 0);
		
		// if the result is greater than 0, the data is valid
		if (bytesReceived > 0)
		{
			// add the event to the queue to be dispatched later
			addEventData(RECEIVE_DATA_EVENT, buffer);
		}

		// if the result is 0 or negative, the client was disconnected
		else
		{
			disconnected = true;
		}
	}

	// free memory allocated for received data
	delete[] buffer;

	// disconnect from the server
	if (_socket != INVALID_SOCKET)
	{
		closesocket(_socket);
	}

	// set connection attributes
	_socket = INVALID_SOCKET;
	_serverAddress = "";
	_serverPort = "";

	// add the event to the queue to be dispatched later
	addEventData(DISCONNECTION_EVENT);
}

/**
 * Adds an event to be dispatched later.
 *
 * @param type				The type of event.
 * @param data				The data received from the client.
 */
void
ICS_Client::addEventData(EventType type, std::string data)
{
	// lock access to the event queue
	_eventsMutex.lock();

	// create the event data object
	EventData eventData;

	eventData.type = type;
	eventData.data = data;

	// add the event to the queue
	_events.push_back(eventData);

	// release access to the event queue
	_eventsMutex.unlock();
}

/**
 * Attempts get event data from the queue.
 *
 * @param data		Holds the result of the function call.
 *
 * @returns			true if there was data in the queue, false otherwise.
 */
bool
ICS_Client::getEventData(EventData& data)
{
	// the result of the function
	bool result = false;

	// lock access to the event queue
	_eventsMutex.lock();

	// if there is an event, pop it off the queue
	if (_events.size() > 0)
	{
		data = _events.front();
		_events.pop_front();
		result = true;
	}

	// release access to the event queue
	_eventsMutex.unlock();

	// return the result
	return result;
}