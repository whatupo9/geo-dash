#include "ICS_Server.h"	// the declaration of ICS_Server

#include <ws2tcpip.h>

#include "ICS_Game.h"	// the declaration of ICS_Game

#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

/**
 * ICS_Server constructor.
 *
 * @param port			The port to communicate over.
 * @param maxClients	The maximum number of clients that can connect at once.
 */
ICS_Server::ICS_Server(std::string port, unsigned int maxClients)
	:
	_port(port),
	_maxClients(maxClients)
{
	// initialize the client sockets
	{
		// reserve access to the sockets array by locking the mutex
		_clientSocketsMutex.lock();

		// allocate memory
		_clientSockets = new SOCKET[_maxClients];

		// initialize each socket
		for (unsigned int i = 0; i < _maxClients; i++)
		{
			_clientSockets[i] = INVALID_SOCKET;
		}

		// unlock the mutex
		_clientSocketsMutex.unlock();
	}

	// allocate memory for the peer names
	_peerNames = new std::string[_maxClients];

	// allocate memory for the client threads
	_clientThreadsMutex.lock();
	_clientThreads = new std::thread[_maxClients];
	_clientThreadsMutex.unlock();

	// start recieving update events
	ICS_Game::getInstance().addUpdateEventListener(this);
}

/**
 * ICS_Server destructor.
 */
ICS_Server::~ICS_Server()
{
	// stop recieving update events
	ICS_Game::getInstance().removeUpdateEventListener(this);

	// stop the server (if it is running)
	stop();

	// free allocated memory
	_clientThreadsMutex.lock();
	delete[] _clientThreads;
	_clientThreadsMutex.unlock();

	delete[] _peerNames;

	_clientSocketsMutex.lock();
	delete[] _clientSockets;
	_clientSocketsMutex.unlock();
}

/**
 * Gets an IP address for the server.
 *
 * @param i		The index of the IP address (between 0 and one less than the number of addresses).
 *
 * @returns		An IP address for the server.
 */
std::string
ICS_Server::getIPAddress(unsigned int i)
{
	if (i < _IPAddresses.size())
	{
		return _IPAddresses[i];
	}

	return "";
}

/**
 * Handles update events.  Allows the server to dispatch events.
 *
 * @param elapsed	The number of seconds that have elapsed since the last update.
 */
void
ICS_Server::handleUpdateEvent(float elapsed)
{
	// for handling an event
	EventData data;

	// keep getting event data until there are no more events
	while (getEventData(data))
	{
		// handle a connection event
		if (data.type == CONNECTION_EVENT && _clientConnectCallback)
		{
			_clientConnectCallback(data.clientNumber);
		}

		// handle a disconnection event
		if (data.type == DISCONNECTION_EVENT && _clientDisconnectCallback)
		{
			_clientDisconnectCallback(data.clientNumber);
		}

		// handle recieving data
		if (data.type == RECEIVE_DATA_EVENT && _receiveDataCallback)
		{
			_receiveDataCallback(data.clientNumber, data.data);
		}
	}
}

/**
 * Attempts to start the server.
 *
 * @returns		true if the server was successfully started or already running, false otherwise
 */
bool
ICS_Server::start()
{
	// the server can't be started when it is in the process of stopping
	if (_stopping)
	{
		return false;
	}

	// make sure the server is fully stopped
	if (_stopped)
	{
		// initialize Winsock
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			return false;
		}

		// generate hints about what type of sockets to support
		addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));	// wipe memory
		hints.ai_family = AF_INET;			// internetwork: UDP, TCP, etc.
		hints.ai_socktype = SOCK_STREAM;	// stream socket
		hints.ai_protocol = IPPROTO_TCP;	// use TCP protocol
		hints.ai_flags = AI_PASSIVE;		// socket address will be used in bind() call

		// resolve the server address and port
		if (getaddrinfo(NULL, _port.c_str(), &hints, &_addressInfo) != 0)
		{
			freeAddressInfo();
			WSACleanup();
			return false;
		}

		// the server has started... set the flags
		_stopping = false;
		_stopped = false;

		// start listening for new clients
		_listenForClientsThread = std::thread([this] { listenForClients(); });
	}

	// get the host and address data
	retrieveHostName();
	retrieveIPAddresses();

	// the server is running
	return true;
}

/**
 * Attempts to stop the server.
 *
 * @returns		true if the process of stopping the server was successful or already in process, false otherwise.
 */
bool
ICS_Server::stop()
{
	// make sure the server isn't in the process of stopping or already stopped
	if (!_stopping && !_stopped)
	{
		// the stoppping process has begun
		_stopping = true;

		// free memory allocated for the address info
		freeAddressInfo();

		// close down winsock
		WSACleanup();

		// stop listening for new clients
		_listenForClientsThread.join();

		// the server has been stopped
		_stopped = true;

		// reset the host name and IP addresses
		_hostName = "";
		_IPAddresses.clear();
	}

	// success
	return true;
}

/**
 * Attempts to send data to a specific client.
 *
 * @param clientNumber	The id of the client to send the data to.
 * @param data			The data to send.
 * @param dataLength	The length of the data, in bytes.
 *
 * @returns		true if successful, false otherwise.
 */
bool
ICS_Server::sendData(unsigned int clientNumber, const char* data, int dataLength)
{
	// verify the client number
	if (clientNumber >= _maxClients)
	{
		return false;
	}

	// the return result
	bool result = true;

	// lock access to the client sockets
	_clientSocketsMutex.lock();

	// attempt to send the data
	if (_clientSockets[clientNumber] == INVALID_SOCKET || send(_clientSockets[clientNumber], data, dataLength, 0) == SOCKET_ERROR)
	{
		result = false;
	}

	// release access to the client sockets
	_clientSocketsMutex.unlock();

	// return the result
	return result;
}

/**
 * Attempts to send data to all connected clients.
 *
 * @param data			The data to send.
 * @param dataLength	The length of the data, in bytes.
 *
 * @returns				true if successful, false otherwise.
 */
bool
ICS_Server::broadcastData(const char* data, int dataLength)
{
	// loop through each connected client and send the data
	for (unsigned int clientNumber = 0; clientNumber < _clients; clientNumber++)
	{
		sendData(clientNumber, data, dataLength);
	}

	// success
	return true;
}

/**
 * Attempts to disconnect a specific client.
 *
 * @param clientNumber	The id of the client to disconnect.
 *
 * @returns				true if the client was disconnected or was not connected, false otherwise.
 */
bool
ICS_Server::disconnectClient(unsigned int clientNumber)
{
	// verify the client number
	if (clientNumber >= _maxClients)
	{
		return false;
	}

	// if the socket is valid, close the connection
	_clientSocketsMutex.lock();

	if (_clientSockets[clientNumber] != INVALID_SOCKET)
	{
		shutdown(_clientSockets[clientNumber], SD_SEND);
	}

	_clientSocketsMutex.unlock();

	// wait for the thread to complete
	_clientThreadsMutex.lock();

	if (_clientThreads[clientNumber].joinable())
	{
		_clientThreads[clientNumber].join();
	}

	_clientThreadsMutex.unlock();
	
	// return the result
	return true;
}

/**
 * Listens for data sent from the specified client and generates an event when data is received.
 *
 * @param clientNumber	The id of the client.
 */
void
ICS_Server::listenForData(unsigned int clientNumber)
{
	// get the client to listen for data from
	_clientSocketsMutex.lock();
	SOCKET clientSocket = _clientSockets[clientNumber];
	_clientSocketsMutex.unlock();

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
		int bytesReceived = recv(clientSocket, buffer, _bufferLength, 0);
		
		// if the result is greater than 0, the data is valid
		if (bytesReceived > 0)
		{
			// add the event to the queue to be dispatched later
			addEventData(RECEIVE_DATA_EVENT, clientNumber, buffer);
		}

		// if the result is 0 or negative, the client was disconnected
		else
		{
			disconnected = true;
		}
	}

	// free memory allocated for received data
	delete[] buffer;

	// disconnect the client
	_clientSocketsMutex.lock();
	closesocket(_clientSockets[clientNumber]);
	_clientSockets[clientNumber] = INVALID_SOCKET;
	_clientSocketsMutex.unlock();

	// add the event to the queue to be dispatched later
	addEventData(DISCONNECTION_EVENT, clientNumber);
}

/**
 * Listens for clients connecting to the server.
 */
void
ICS_Server::listenForClients()
{
	// create a socket for connecting to server
	SOCKET listenSocket = socket(_addressInfo->ai_family, _addressInfo->ai_socktype, _addressInfo->ai_protocol);

	// make sure the socket is valid
	if (listenSocket != INVALID_SOCKET)
	{
		// setup the TCP listening socket
		if (bind(listenSocket, _addressInfo->ai_addr, (int)_addressInfo->ai_addrlen) != SOCKET_ERROR)
		{
			// continue to listen for client connections until the connection is closed
			while (listen(listenSocket, SOMAXCONN) != SOCKET_ERROR)
			{
				// accept a client socket
				SOCKET clientSocket = accept(listenSocket, NULL, NULL);

				// verify the socket
				if (clientSocket != INVALID_SOCKET)
				{
					// get the peer name for the client
					SOCKADDR_IN clientInfo = { 0 };
					int addressSize = sizeof(clientInfo);

					getpeername(clientSocket, (struct sockaddr*)&clientInfo, &addressSize);

					std::string peerName = inet_ntoa(clientInfo.sin_addr);

					// determine the client number
					_clientNumber = _clients;

					for (unsigned int i = 0; i < _clients; i++)
					{
						if (_peerNames[i] == peerName)
						{
							_clientNumber = i;
						}
					}

					// lock access to the client sockets
					_clientSocketsMutex.lock();

					// make sure the limit is not reached
					if (_clientNumber >= _maxClients)
					{
						std::string data = "Connection refused";
						send(clientSocket, data.c_str(), (int)data.length(), 0);
						shutdown(clientSocket, SD_SEND);
					}

					// make sure the peer name is unique
					else if (_clientNumber < _clients && _clientSockets[_clientNumber] != INVALID_SOCKET)
					{
						std::string data = "Connection refused";
						send(clientSocket, data.c_str(), (int)data.length(), 0);
						shutdown(clientSocket, SD_SEND);
					}

					// connect the client
					else
					{
						// add the event to the queue to be dispatched later
						addEventData(CONNECTION_EVENT, _clientNumber);

						// add the new client to the set
						_clientSockets[_clientNumber] = clientSocket;

						if (_clientNumber == _clients)
						{
							_peerNames[_clients] = peerName;
							_clients++;
						}

						// reserve access to the client threads array by locking the mutex
						_clientThreadsMutex.lock();

						// if this client connected previously, join its thread
						if (_clientThreads[_clientNumber].joinable())
						{
							_clientThreads[_clientNumber].join();
						}

						// spawn a new thread
						_clientThreads[_clientNumber] = std::thread([this] { listenForData(_clientNumber); });

						// release the mutex lock
						_clientThreadsMutex.unlock();
					}

					// release access to the client sockets
					_clientSocketsMutex.unlock();
				}
			}
		}

		// no longer need server socket
		closesocket(listenSocket);
	}

	// shut down all client connections
	for (unsigned int i = 0; i < _clients; i++)
	{
		disconnectClient(i);
	}
}

/**
 * Determines the host name for the server.
 */
void
ICS_Server::retrieveHostName()
{
	// create a place to hold the host name
	char hostName[80];

	// attempt to retrieve the host name
	if (gethostname(hostName, sizeof(hostName)) != SOCKET_ERROR)
	{
		// set the member attribute
		_hostName = hostName;
	}
}

/**
 * Determines the IP addresses for the server.
 */
void
ICS_Server::retrieveIPAddresses()
{
	// make sure the host name is set
	if (_hostName.length() > 0)
	{
		// retrieve the host data
		struct hostent* hostData = gethostbyname(_hostName.c_str());

		// make sure the data is valid
		if (hostData != nullptr)
		{
			// iterate through each IP address
			for (int i = 0; hostData->h_addr_list[i] != 0; ++i)
			{
				// get the address
				struct in_addr address;
				memcpy(&address, hostData->h_addr_list[i], sizeof(struct in_addr));

				// save the address
				_IPAddresses.push_back(inet_ntoa(address));
			}
		}
	}
}

/**
 * Frees memory allocated for the address info
 */
void
ICS_Server::freeAddressInfo()
{
	if (_addressInfo)
	{
		freeaddrinfo(_addressInfo);
		_addressInfo = NULL;
	}
}

/**
* Adds an event to be dispatched later.
*
* @param type				The type of event.
* @param clientNumber		The id of the client associated with the event.
* @param data				The data received from the client.
*/
void
ICS_Server::addEventData(EventType type, unsigned int clientNumber, std::string data)
{
	// lock access to the event queue
	_eventsMutex.lock();

	// create the event data object
	EventData eventData;

	eventData.type = type;
	eventData.clientNumber = clientNumber;
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
ICS_Server::getEventData(EventData& data)
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