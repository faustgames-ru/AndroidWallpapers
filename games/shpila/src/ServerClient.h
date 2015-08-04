#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H

using namespace RakNet;
using namespace gameplay;

#ifdef _WIN32
#include "WindowsIncludes.h" // Sleep64
#else
#include <unistd.h> // usleep
#include <cstdio>
#include "Getche.h"
#endif

#define NETGAME_VERSION 8830
#define AUTH_BS "5E1C1837D2C54B24EAAED18C3F96D9EA51A2A481003"
#define SERVER_PORT 1234
#define RANDOM_DATA_SIZE_1 50
#define RANDOM_DATA_SIZE_2 100
//static const int NUM_CLIENTS=100;
//char randomData1[RANDOM_DATA_SIZE_1];
//char randomData2[RANDOM_DATA_SIZE_2];
//char *remoteIPAddress=0;

class GameObjectManager;

struct ActorSyncData
{
	int actorID;
	int actorType;
	Vector3 pos;
	float angle;
	bool holder;
};

// Connects, sends data over time, disconnects, repeat
class Client
{
public:
	Client();
	~Client();
	void Startup(GameObjectManager* manager);
	bool Connect(const char *szHostname, int iPort, char *szNickname, char *szPassword);
	void Disconnect(void);
	void Update(RakNet::TimeMS curTime);
private:
	void updateNetwork();
	void Packet_ConnectionSucceeded(Packet *p);
	void Packet_ActorSync(Packet *p);
	void Send_ActorSync(int playerID);
	void UpdatePlayerScoresAndPings(int iWait, int iMS);
	void RequestAddNewActor();

	bool isConnected;
	RakClientInterface *_pRakClient;
	RakNet::TimeMS nextSendTime;
	GameObjectManager* _manager;
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

#endif