#ifndef WARRIORS_H_
#define WARRIORS_H_

using namespace gameplay;




class DarkWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	DarkWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class ArchonWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ArchonWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class ObserverWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ObserverWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class ImmortalWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ImmortalWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class ColossusWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ColossusWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class VoidRayWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	VoidRayWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class CarrierWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	CarrierWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class TempestWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	TempestWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class MothershipWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	MothershipWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
};

class CoreWarrior : public BaseWarrior
{
public:
	
	static BaseGameObject* constructor();
	CoreWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void update(float time);
protected:
	float _altitude;
};

#endif