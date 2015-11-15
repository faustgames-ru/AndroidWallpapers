#ifndef WARRIORS_H_
#define WARRIORS_H_

using namespace gameplay;




class CheidaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	CheidaWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class HantîWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	HantîWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class ObserverWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ObserverWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class ImmortalWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ImmortalWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class ColossusWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	ColossusWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class VoidRayWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	VoidRayWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class CarrierWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	CarrierWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class TempestWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	TempestWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class MothershipWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	MothershipWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
};

class QbiWarrior : public BaseWarrior
{
public:
	
	static BaseGameObject* constructor();
	QbiWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
private:
	SimpleTimer _abilityTimer;
};

#endif