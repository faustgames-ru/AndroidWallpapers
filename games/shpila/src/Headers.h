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
#define ACTOR_TYPE_CHASOVOY 1
#define ACTOR_TYPE_BUDFOOR 2
#define ACTOR_TYPE_DARK 3
#define ACTOR_TYPE_BARAR 4
#define ACTOR_TYPE_ARCHON 5
#define ACTOR_TYPE_OBSERVER 6
#define ACTOR_TYPE_IMMORTAL 7
#define ACTOR_TYPE_COLOSSUS 8
#define ACTOR_TYPE_ALBIRIA 9
#define ACTOR_TYPE_VOIDRAY 10
#define ACTOR_TYPE_CARRIER 11
#define ACTOR_TYPE_TEMPEST 12
#define ACTOR_TYPE_MOTHERSHIP 13
#define ACTOR_TYPE_MOTHERSHIPCORE 14

#define ACTOR_TYPE_TOWER 15
#define ACTOR_TYPE_BASE 16

#include "algorithms\Links.h"
#include "algorithms\Utils.h"
#include "algorithms\Valuable.h"
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
#include "Tower.h"
#include "thebase.h"
#include "Warriors.h"
#include "BaseBullet.h"
#include "BoomerangBullet.h"
#include "HiddenObject.h"
#include "GameObjectManager.h"
#include "BaseWarrior.h"
#include "Player.h"
#include "GameHUD.h"
#include "UnitMovement.h"
#include "StarsParticles.h"

#define TIME_SCALE 0.6f

#define COMMON_SCALE 0.1f
#define IRBAGA_SCALE COMMON_SCALE
#define BUDFOOR_SCALE COMMON_SCALE
#define BARAR_SCALE COMMON_SCALE
#define ALBIRIA_SCALE COMMON_SCALE
#define CHASOVOY_SCALE COMMON_SCALE

typedef unsigned short PLAYERID;

#endif