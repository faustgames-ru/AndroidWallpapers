/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "ServerClient.h"

using namespace RakNet;

static const int NUM_CLIENTS=100;
#define SERVER_PORT 1234
#define RANDOM_DATA_SIZE_1 50
char randomData1[RANDOM_DATA_SIZE_1];
#define RANDOM_DATA_SIZE_2 100
char randomData2[RANDOM_DATA_SIZE_2];
char *remoteIPAddress=0;

// Connects, sends data over time, disconnects, repeat
Client::Client()
: isConnected(false)
, _pRakClient(RakNetworkFactory::GetRakClientInterface())
, nextSendTime(0)
{
}
Client::~Client()
{
	RakNetworkFactory::DestroyRakClientInterface(_pRakClient);
}
void Client::Startup(void)
{
	_pRakClient->SetMTUSize(576);
	
	nextSendTime = 0;
	isConnected=false;
}
bool Client::Connect(char *szHostname, int iPort, char *szNickname, char *szPassword)
{
	_pRakClient->SetPassword(szPassword);
	return isConnected = _pRakClient->Connect(szHostname, iPort, 0, 0, 5);
}
void Client::Disconnect(void)
{
	_pRakClient->Disconnect(0);
	isConnected=false;
}
void Client::Update(RakNet::TimeMS curTime)
{
	Packet *p = _pRakClient->Receive();
	while (p)
	{
		switch (p->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
			isConnected=true;
			break;
			// print out errors
		case ID_CONNECTION_ATTEMPT_FAILED:
			printf("Client Error: ID_CONNECTION_ATTEMPT_FAILED\n");
			isConnected=false;
			break;
		case ID_ALREADY_CONNECTED:
			printf("Client Error: ID_ALREADY_CONNECTED\n");
			break;
		case ID_CONNECTION_BANNED:
			printf("Client Error: ID_CONNECTION_BANNED\n");
			break;
		case ID_INVALID_PASSWORD:
			printf("Client Error: ID_INVALID_PASSWORD\n");
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("Client Error: ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("Client Error: ID_NO_FREE_INCOMING_CONNECTIONS\n");
			isConnected=false;
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			//printf("ID_DISCONNECTION_NOTIFICATION\n");
			isConnected=false;
			break;
		case ID_CONNECTION_LOST:
			printf("Client Error: ID_CONNECTION_LOST\n");
			isConnected=false;
			break;
		}
		_pRakClient->DeallocatePacket(p);
		p = _pRakClient->Receive();
	}

	if (curTime>nextSendTime && isConnected)
	{
		/*if (randomMT()%10==0)
		{
			peer->Send((const char*)&randomData2,RANDOM_DATA_SIZE_2,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		}
		else
		{
			peer->Send((const char*)&randomData1,RANDOM_DATA_SIZE_1,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		}*/

		nextSendTime=curTime+50;
	}
}

// Just listens for ID_USER_PACKET_ENUM and validates its integrity
Server::Server()
: _pRakServer(RakNetworkFactory::GetRakServerInterface())
{
	nextSendTime=0;
}
Server::~Server()
{
	RakNetworkFactory::DestroyRakServerInterface(_pRakServer);
}
void Server::Start(unsigned short port)
{
	_pRakServer->Start(6, 0, 5, port);
	_pRakServer->StartOccasionalPing();
	//RakNet::SocketDescriptor socketDescriptor;
	//socketDescriptor.port=(unsigned short) SERVER_PORT;
	//bool b = peer->Startup((unsigned short) 600,&socketDescriptor,1)==RakNet::RAKNET_STARTED;
	//RakAssert(b);
	//peer->SetMaximumIncomingConnections(600);
}
unsigned Server::ConnectionCount(void) const
{
	unsigned short numberOfSystems = 0;
	//_pRakServer->get GetConnectionList(0, &numberOfSystems);
	return numberOfSystems;
}
void Server::Update(RakNet::TimeMS curTime)
{
	Packet *p = _pRakServer->Receive();
	while (p)
	{
		switch (p->data[0])
		{
		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
		case ID_NEW_INCOMING_CONNECTION:
			printf("Connections = %i\n", ConnectionCount());
			break;
// 				case ID_USER_PACKET_ENUM:
// 					{
// 						peer->Send((const char*) p->data, p->length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->guid, true);
// 						break;
// 					}
		}
		_pRakServer->DeallocatePacket(p);
		p = _pRakServer->Receive();
	}

	if (curTime>nextSendTime)
	{
		/*if (randomMT()%10==0)
		{
			peer->Send((const char*)&randomData2,RANDOM_DATA_SIZE_2,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		}
		else
		{
			peer->Send((const char*)&randomData1,RANDOM_DATA_SIZE_1,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		}*/

		nextSendTime=curTime+100;
	}
}

/*int main(void)
{
	Client clients[NUM_CLIENTS];
	Server server;
//	int clientIndex;
	int mode;

	printf("Connects many clients to a single server.\n");
	printf("Difficulty: Intermediate\n\n");
	printf("Run as (S)erver or (C)lient or (B)oth? ");
	char ch = getche();
	static char *defaultRemoteIP="127.0.0.1";
	char remoteIP[128];
	static char *localIP="127.0.0.1";
	if (ch=='s' || ch=='S')
	{
		mode=0;
	}
	else if (ch=='c' || ch=='c')
	{
		mode=1;
		remoteIPAddress=remoteIP;
	}
	else
	{
		mode=2;
		remoteIPAddress=localIP;
	}
	printf("\n");

	if (mode==1 || mode==2)
	{
		printf("Enter remote IP: ");
		Gets(remoteIP, sizeof(remoteIP));
		if (remoteIP[0]==0)
		{
			strcpy(remoteIP, defaultRemoteIP);
			printf("Using %s\n", defaultRemoteIP);
		}
	}

	unsigned i;
	randomData1[0]=(char) ID_USER_PACKET_ENUM;
	for (i=0; i < RANDOM_DATA_SIZE_1-1; i++)
		randomData1[i+1]=i;
	randomData2[0]=(char) ID_USER_PACKET_ENUM;
	for (i=0; i < RANDOM_DATA_SIZE_2-1; i++)
		randomData2[i+1]=i;

	if (mode==0 || mode==2)
	{
		server.Start();
		printf("Started server\n");
	}
	if (mode==1 || mode==2)
	{
		printf("Starting clients...\n");
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].Startup();
		printf("Started clients\n");
		printf("Connecting clients...\n");
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].Connect();
		printf("Done.\n");
	}
	
	RakNet::TimeMS endTime = RakNet::GetTimeMS()+60000*5;
	RakNet::TimeMS time = RakNet::GetTimeMS();
	while (time < endTime)
	{
		if (mode==0 || mode==2)
			server.Update(time);
		if (mode==1 || mode==2)
		{
			for (i=0; i < NUM_CLIENTS; i++)
				clients[i].Update(time);
		}

		if (kbhit())
		{
			char ch = getch();
			if (ch==' ')
			{
				FILE *fp;
				char text[2048];
				if (mode==0 || mode==2)
				{
					printf("Logging server statistics to ServerStats.txt\n");
					fp=fopen("ServerStats.txt","wt");
					for (i=0; i < NUM_CLIENTS; i++)
					{
						RakNetStatistics *rssSender;
						rssSender=server.peer->GetStatistics(server.peer->GetSystemAddressFromIndex(i));
						StatisticsToString(rssSender, text, 3);
						fprintf(fp,"==== System %i ====\n", i+1);
						fprintf(fp,"%s\n\n", text);
					}
					fclose(fp);
				}
				if (mode==1 || mode==2)
				{
					printf("Logging client statistics to ClientStats.txt\n");
					fp=fopen("ClientStats.txt","wt");
					for (i=0; i < NUM_CLIENTS; i++)
					{
						RakNetStatistics *rssSender;
						rssSender=clients[i].peer->GetStatistics(clients[i].peer->GetSystemAddressFromIndex(0));
						StatisticsToString(rssSender, text, 3);
						fprintf(fp,"==== Client %i ====\n", i+1);
						fprintf(fp,"%s\n\n", text);
					}
					fclose(fp);
				}
			}	
			if (ch=='q' || ch==0)
				break;
		}

		time = RakNet::GetTimeMS();
		RakSleep(30);
	}

	if (mode==0 || mode==2)
		server.peer->Shutdown(0);
	if (mode==1 || mode==2)
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].peer->Shutdown(0);

	printf("Test completed");
	return 0;
}*/
