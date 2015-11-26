#include "Headers.h"
#include "main.h"

bool CreateWarriorHandler(const char* name, const Vector3& position)
{
	Shpila* shpila = static_cast<Shpila*>(Game::getInstance());
	Vector3 newpos = Vector3(-position.x, position.y, -position.z);
	shpila->getScriptController()->executeFunction<void>("OnCreateWarrior", "s<Vector3>", name, &newpos);
	return true;
}

void AIManager::setPlayer(PlayerObject* player)
{
	_player = player;
	_player->OnCreateWarriorHandler = CreateWarriorHandler;
}
void AIManager::setAIPlayer(PlayerObject* player)
{
	_AIPlayer = player;
}

bool AIManager::CreateWarrior(const char* name, const Vector3 position)
{
	if (_AIPlayer)
	{
		_NewWarriorName = name;
		_NewWarriorPosition = position;
		return true;
	}	
		return false;
}

void AIManager::update(float time)
{
	if (!_NewWarriorName.empty())
	{
		_AIPlayer->setCreateWariorName(_NewWarriorName.c_str());
		_AIPlayer->CreateWarrior(false, _NewWarriorPosition);
		_NewWarriorName = "";
	}
}

void AIManager::luaRegister()
{
	const luaL_Reg lua_members[] =
	{
		{ "CreateWarrior", lua_AIManager_CreateWarrior },
		{ NULL, NULL }
	};
	
	std::vector<std::string> scopePath;
	ScriptUtil::registerClass("AIManager", lua_members, NULL, lua_AIManager__gc, NULL, scopePath);
}

AIManager::AIManager()
:_player()
, _AIPlayer()
, _NewWarriorName()
, _NewWarriorPosition()
{
}

static AIManager* getInstance(lua_State* state)
{
	void* userdata = luaL_checkudata(state, 1, "AIManager");
	luaL_argcheck(state, userdata != NULL, 1, "'AIManager' expected.");
	return static_cast<AIManager*>(static_cast<ScriptUtil::LuaObject*>(userdata)->instance);
}

int lua_AIManager__gc(lua_State* state)
{
	// Get the number of parameters.
	int paramCount = lua_gettop(state);

	// Attempt to match the parameters to a valid binding.
	switch (paramCount)
	{
	case 1:
	{
			  if ((lua_type(state, 1) == LUA_TUSERDATA))
			  {
				  void* userdata = luaL_checkudata(state, 1, "AIManager");
				  luaL_argcheck(state, userdata != NULL, 1, "'AIManager' expected.");
				  gameplay::ScriptUtil::LuaObject* object = static_cast<ScriptUtil::LuaObject*>(userdata);
				  if (object->owns)
				  {
					  AIManager* instance = static_cast<AIManager*>(object->instance);
					  SAFE_RELEASE(instance);
				  }

				  return 0;
			  }

			  lua_pushstring(state, "lua_AIManager__gc - Failed to match the given parameters to a valid function signature.");
			  lua_error(state);
			  break;
	}
	default:
	{
			   lua_pushstring(state, "Invalid number of parameters (expected 1).");
			   lua_error(state);
			   break;
	}
	}
	return 0;
}

int lua_AIManager_CreateWarrior(lua_State* state)
{
	// Get the number of parameters.
	int paramCount = lua_gettop(state);

	// Attempt to match the parameters to a valid binding.
	switch (paramCount)
	{
	case 3:
	{
			  do
			  {
				  if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					  (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
					  (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
				  {
					  const char* param1 = gameplay::ScriptUtil::getString(2, false);

					  // Get parameter 1 off the stack.
					  bool param1Valid;
					  gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param1Valid);
					  if (!param1Valid)
						  break;

					  AIManager* instance = getInstance(state);
					  instance->CreateWarrior(param1, *param2);

					  return 0;
				  }
			  } while (0);

			  lua_pushstring(state, "lua_AIManager_CreateWarrior - Failed to match the given parameters to a valid function signature.");
			  lua_error(state);
			  break;
	}
	default:
	{
			   lua_pushstring(state, "Invalid number of parameters (expected 3).");
			   lua_error(state);
			   break;
	}
	}
	return 0;
}