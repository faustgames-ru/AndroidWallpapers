/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "Headers.h"
#include "main.h"


using namespace RakNet;

static const int NUM_CLIENTS=100;
#define SERVER_PORT 1234
#define RANDOM_DATA_SIZE_1 50
char randomData1[RANDOM_DATA_SIZE_1];
#define RANDOM_DATA_SIZE_2 100
char randomData2[RANDOM_DATA_SIZE_2];
char *remoteIPAddress=0;

char g_szNickName[32];

struct stPlayerInfo
{
	int iIsConnected;
	int iIsStreamedIn;
	int iGotMarkersPos;
	char szPlayerName[20];
	int iScore;
	DWORD dwPing;
	int iAreWeInAVehicle;

};

struct stPlayerInfo playerInfo[32];

void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char *)Data, (iBitLength / 8) + 1, false);

	PLAYERID playerId;
	int  iPlayerScore;
	DWORD dwPlayerPing;

	for (PLAYERID i = 0; i<(iBitLength / 8) / 9; i++)
	{
		bsData.Read(playerId);
		bsData.Read(iPlayerScore);
		bsData.Read(dwPlayerPing);
		Shpila* shpila = (Shpila*)Game::getInstance();
		shpila->_ping = dwPlayerPing;
		//playerInfo[playerId].iScore = iPlayerScore;
		//playerInfo[playerId].dwPing = dwPlayerPing;
	}
}

void InitNetGame(RPCParameters *rpcParams)
{

}

void tmp1(RPCParameters *rpcParams)
{

}




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
void Client::Startup(GameObjectManager* manager)
{
	_manager = manager;
	_pRakClient->SetMTUSize(576);

	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs, UpdateScoresPingsIPs);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_InitGame, InitNetGame);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleAdd, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_ServerJoin, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_ServerQuit, tmp1);	
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerAdd, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerDeath, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerRemove, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleRemove, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_ConnectionRejected, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_ClientMessage, tmp1);
	_pRakClient->RegisterAsRemoteProcedureCall(&RPC_Chat, tmp1);
	
	nextSendTime = GetTickCount();
	isConnected=false;
}
bool Client::Connect(char *szHostname, int iPort, char *szNickname, char *szPassword)
{
	_pRakClient->SetPassword(szPassword);
	return isConnected = _pRakClient->Connect(szHostname, iPort, 0, 0, 5);
}
void Client::Disconnect(void)
{
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ServerJoin);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ServerQuit);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_InitGame);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldPlayerAdd);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldPlayerDeath);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldPlayerRemove);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldVehicleAdd);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldVehicleRemove);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ConnectionRejected);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ClientMessage);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_Chat);
	_pRakClient->UnregisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs);

	_pRakClient->Disconnect(0);
	isConnected=false;
}
void Client::Update(RakNet::TimeMS curTime)
{
	updateNetwork();

	if ((GetTickCount() - nextSendTime) > 50)
	{
		Shpila *shpila = (Shpila*)Game::getInstance();
		Send_ActorSync(shpila->_netPlayerID);
		nextSendTime = GetTickCount();
	}
}

void Client::updateNetwork()
{
	unsigned char packetIdentifier;
	Packet *pkt;

	while (pkt = _pRakClient->Receive())
	{
		if (pkt->data[0] == ID_TIMESTAMP)
		{
			if (pkt->length > sizeof(unsigned char)+sizeof(unsigned int))
				packetIdentifier = (unsigned char)pkt->data[sizeof(unsigned char)+sizeof(unsigned int)];
			else
				return;
		}
		else
			packetIdentifier = (unsigned char)pkt->data[0];

		switch (packetIdentifier)
		{
		/*case ID_DISCONNECTION_NOTIFICATION:
			//Log("[RAKSAMP] Connection was closed by the server. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;
		case ID_CONNECTION_BANNED:
			//Log("[RAKSAMP] You are banned. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			//Log("[RAKSAMP] Connection attempt failed. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			//Log("[RAKSAMP] The server is full. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;
		case ID_INVALID_PASSWORD:
			//Log("[RAKSAMP] Invalid password. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;
		case ID_CONNECTION_LOST:
			//Log("[RAKSAMP] The connection was lost. Reconnecting in %d seconds.", iReconnectTime / 1000);
			//resetPools(1, iReconnectTime);
			break;*/
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Packet_ConnectionSucceeded(pkt);
			break;
		case ID_ACTOR_SYNC:
			Packet_ActorSync(pkt);
			break;
			
		/*case ID_PLAYER_SYNC:
			Packet_PlayerSync(pkt);
			break;
		case ID_VEHICLE_SYNC:
			Packet_VehicleSync(pkt);
			break;
		case ID_PASSENGER_SYNC:
			Packet_PassengerSync(pkt);
			break;
		case ID_AIM_SYNC:
			Packet_AimSync(pkt);
			break;
		case ID_TRAILER_SYNC:
			Packet_TrailerSync(pkt);
			break;
		case ID_UNOCCUPIED_SYNC:
			Packet_UnoccupiedSync(pkt);
			break;
		case ID_MARKERS_SYNC:
			Packet_MarkersSync(pkt);
			break;*/
		}

		_pRakClient->DeallocatePacket(pkt);
	}

	UpdatePlayerScoresAndPings(1, 3000);
}

void Client::Packet_ConnectionSucceeded(Packet *p)
{
	Shpila *shpila = (Shpila*)Game::getInstance();
	RakNet::BitStream bsSuccAuth((unsigned char *)p->data, p->length, false);
	PLAYERID myPlayerID;
	unsigned int uiChallenge;

	bsSuccAuth.IgnoreBits(8); // ID_CONNECTION_REQUEST_ACCEPTED
	bsSuccAuth.IgnoreBits(32); // binaryAddress
	bsSuccAuth.IgnoreBits(16); // port

	bsSuccAuth.Read(myPlayerID);
	shpila->_netPlayerID = myPlayerID;
	playerInfo[myPlayerID].iIsConnected = 1;
	strcpy(playerInfo[myPlayerID].szPlayerName, g_szNickName);

	bsSuccAuth.Read(uiChallenge);

	//Log("Connected. Joining the game...");

	char *pszAuthBullshit = AUTH_BS;
	int iVersion = NETGAME_VERSION;
	BYTE byteMod = 1;
	BYTE byteNameLen = (BYTE)strlen(g_szNickName);
	BYTE byteAuthBSLen = (BYTE)strlen(pszAuthBullshit);
	unsigned int uiClientChallengeResponse = uiChallenge ^ iVersion;

	RakNet::BitStream bsSend;
	bsSend.Write(iVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(g_szNickName, byteNameLen);
	bsSend.Write(uiClientChallengeResponse);
	bsSend.Write(byteAuthBSLen);
	bsSend.Write(pszAuthBullshit, byteAuthBSLen);
	_pRakClient->RPC(&RPC_ClientJoin, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
}

void Client::Packet_ActorSync(Packet *p)
{
	RakNet::BitStream bsActorSync((unsigned char *)p->data, p->length, false);
	int playerID;
	int count;
	ActorSyncData actorSyncData;

	bsActorSync.IgnoreBits(8);
	bsActorSync.Read(playerID);
	bsActorSync.Read(count);
	for (int i = 0; i < count; i++)
	{
		bsActorSync.Read(actorSyncData);
		bool found = false;
		Itr<BaseGameObject> it = _manager->objects();
		while (it)
		{
			if (it->ID == actorSyncData.actorID)
			{
				it->setPositionOnServer(actorSyncData.pos); 
				found = true;
				break;
			}
			++it;
		}
		if (!found)
		{
			_manager->createObject(ACTOR_DATA[actorSyncData.actorType].Name.c_str(), actorSyncData.pos, playerID);
		}
	}
	//
}

void Client::Send_ActorSync(int playerID)
{
	int count = 0;
	Itr<BaseGameObject> it = _manager->objects();
	while (it)
	{
		if (it->PlayerID == playerID)
		{
			count++;
		}
		++it;
	}

	ActorSyncData data;
	RakNet::BitStream bsActorSync;
	bsActorSync.Write((BYTE)ID_ACTOR_SYNC);
	bsActorSync.Write(playerID);
	bsActorSync.Write(count);
	it = _manager->objects();
	while (it)
	{
		if (it->PlayerID == playerID)
		{
			data.actorID = it->ID;
			data.pos = it->position();
			data.actorType = it->ActorType();
			data.angle = 0.0f;
			bsActorSync.Write((PCHAR)&data, sizeof(ActorSyncData));
		}
		++it;
	}
	_pRakClient->Send(&bsActorSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

void Client::UpdatePlayerScoresAndPings(int iWait, int iMS)
{
	static DWORD dwLastUpdateTick = 0;

	if (iWait)
	{
		if ((GetTickCount() - dwLastUpdateTick) > (DWORD)iMS)
		{
			dwLastUpdateTick = GetTickCount();
			RakNet::BitStream bsParams;
			_pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsParams, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}
	}
	else
	{
		RakNet::BitStream bsParams;
		_pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsParams, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
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
