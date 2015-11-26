#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

class PlayerObject;

class AIManager: public Ref
{
public:
	void setPlayer(PlayerObject* player);
	void setAIPlayer(PlayerObject* player);
	bool CreateWarrior(const char* name, const Vector3 position);
	void update(float time);

	static void luaRegister();

	AIManager();
private:
	CLink<PlayerObject> _player;
	CLink<PlayerObject> _AIPlayer;
	std::string _NewWarriorName;
	Vector3 _NewWarriorPosition;
};

int lua_AIManager__gc(lua_State* state);
int lua_AIManager_CreateWarrior(lua_State* state);

#endif