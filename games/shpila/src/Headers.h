#ifndef ALL_HEADERS_H_
#define ALL_HEADERS_H_

#include "gameplay.h"

#include "RakPeerInterface.h"
#include "BitStream.h"
#include <stdlib.h> // For atoi
#include <cstring> // For strlen
#include <list>
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

#include "algorithms\Links.h"
#include "algorithms\Utils.h"
#include "algorithms\Valuable.h"
#include "UnitManagement\OpenSteer.h"
#include "UnitManagement\SimpleVehicle.h"
#include "UnitManagement\Proximity.h"
#include "TimeManager.h"
#include "TargetCamera.h"
#include "UnitMovement.h"
#include "ActorData.h"
#include "BaseGameObject.h"
#include "BaseStaticActor.h"
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



#define TIME_SCALE 0.7f

#define DETECTION_DISTANCE 9.0f

#define COMMON_SCALE 0.1f
#define IRBAGA_SCALE COMMON_SCALE
#define BUDFOOR_SCALE COMMON_SCALE
#define BARAR_SCALE COMMON_SCALE
#define ALBIRIA_SCALE COMMON_SCALE
#define CHASOVOY_SCALE COMMON_SCALE

#define AIR_UNITS_ALTITUDE 3.75f
#define EXTRACTOR_BUILD_TIME 30000.0f
#define EXTRACTOR_PROCE 175

typedef unsigned short PLAYERID;

#endif