/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "RakPeerInterface.h"

#include "BitStream.h"
#include <stdlib.h> // For atoi
#include <cstring> // For strlen
#include "Rand.h"
#include "RakNetStatistics.h"
#include "MessageIdentifiers.h"
#include <stdio.h>
#include "Kbhit.h"
#include "GetTime.h"
#include "RakAssert.h"
#include "RakSleep.h"
#include "Gets.h"
#include "RakNetTime.h"
#include "RakNetworkFactory.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"

using namespace RakNet;

#ifdef _WIN32
#include "WindowsIncludes.h" // Sleep64
#else
#include <unistd.h> // usleep
#include <cstdio>
#include "Getche.h"
#endif

//static const int NUM_CLIENTS=100;
#define SERVER_PORT 1234
#define RANDOM_DATA_SIZE_1 50
//char randomData1[RANDOM_DATA_SIZE_1];
#define RANDOM_DATA_SIZE_2 100
//char randomData2[RANDOM_DATA_SIZE_2];
//char *remoteIPAddress=0;

// Connects, sends data over time, disconnects, repeat
class Client
{
	public:
		Client();
		~Client();
		void Startup(void);
		bool Connect(char *szHostname, int iPort, char *szNickname, char *szPassword);
		void Disconnect(void);
		void Update(RakNet::TimeMS curTime);

		bool isConnected;
		RakClientInterface *_pRakClient;
		RakNet::TimeMS nextSendTime;
};

// Just listens for ID_USER_PACKET_ENUM and validates its integrity
class Server
{
	public:
		Server();
		~Server();
		void Start(unsigned short port);
		unsigned ConnectionCount(void) const;
		void Update(RakNet::TimeMS curTime);
		

		RakNet::TimeMS nextSendTime;
		RakServerInterface *_pRakServer;
};
