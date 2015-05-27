#include <stdio.h>
#include <windows.h>
#include <time.h>
//#include "../../common/common.h"

#define BAN_FILENAME "RakSAMPServer.ban"

#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include "SAMP\samp_netencr.h"


extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}