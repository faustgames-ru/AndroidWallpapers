#ifndef WARRIORS_H_
#define WARRIORS_H_

using namespace gameplay;

class ActorData
{
public:
	std::string Name;
	float Health;
	int Price;
	float Damage;
	float AttackDistance;
	float AttackDelay;
};

extern ActorData ACTOR_DATA[];

const ActorData& getActorData(const char* actorName);

class DarkWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	DarkWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_DARK; }
};

class ArchonWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ArchonWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_ARCHON; }
};

class ObserverWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ObserverWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_OBSERVER; }
};

class ImmortalWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ImmortalWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_IMMORTAL; }
};

class ColossusWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ColossusWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_COLOSSUS; }
};

class VoidRayWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	VoidRayWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_VOIDRAY; }
};

class CarrierWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	CarrierWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_CARRIER; }
};

class TempestWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	TempestWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_TEMPEST; }
};

class MothershipWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	MothershipWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_MOTHERSHIP; }
};

class CoreWarrior : public BaseWarrior
{
public:
	
	static BaseGameObject* constructor();
	CoreWarrior();
	virtual void init(GameObjectManager& manager, Node* node, int playerID, Matrix transform);
	virtual int ActorType() { return ACTOR_TYPE_MOTHERSHIPCORE; }
	virtual void update(float time);
protected:
	float _altitude;
};



#endif