#ifndef ALL_HEADERS_H_
#define ALL_HEADERS_H_

#include "gameplay.h"

#include "RakPeerInterface.h"
#include "BitStream.h"
#include <stdlib.h> // For atoi
#include <cstring> // For strlen
#include "Rand.h"
#include "RakNetStatistics.h"
#include "PacketEnumerations.h"
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
#include "SAMP\SAMPRPC.h"
#include "ServerClient.h"

#define ACTOR_TYPE_IRBAGA 0
#define ACTOR_TYPE_BUDFOOR 1
#define ACTOR_TYPE_BARAR 2
#define ACTOR_TYPE_ALBIRIA 3
#define ACTOR_TYPE_CHASOVOY 4

#include "algorithms\Links.h"
#include "algorithms\Utils.h"
#include "UnitManagement\OpenSteer.h"
#include "UnitManagement\SimpleVehicle.h"
#include "UnitManagement\Proximity.h"
#include "TargetCamera.h"
#include "UnitMovement.h"
#include "BaseGameObject.h"
#include "BaseActor.h"
#include "BaseWarrior.h"
#include "irbaga.h"
#include "budfoor.h"
#include "barar.h"
#include "albiria.h"
#include "chasovoy.h"
#include "BaseBullet.h"
#include "BoomerangBullet.h"
#include "HiddenObject.h"
#include "GameObjectManager.h"
#include "BaseWarrior.h"
#include "Player.h"
#include "GameHUD.h"
#include "UnitMovement.h"
#include "StarsParticles.h"

#define IRBAGA_SCALE 0.003f
#define BUDFOOR_SCALE 0.005f
#define BARAR_SCALE 0.005f
#define ALBIRIA_SCALE 0.015f
#define CHASOVOY_SCALE 0.0035f

extern std::string ACTOR_TYPE[];

typedef unsigned short PLAYERID;

#endif