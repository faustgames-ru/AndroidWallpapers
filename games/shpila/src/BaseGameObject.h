#ifndef BASEGAMEOBJECT_H_
#define BASEGAMEOBJECT_H_

using namespace gameplay;

class PlayerObject;
class GameObjectManager;
class BaseGameObject;

typedef void(*TimerEventHandler) (BaseGameObject* object);
typedef bool(*TimerEventEnableHandler) (BaseGameObject* object);

struct Timer
{
public:
	Timer(float time, float startTime, TimerEventHandler handler, TimerEventEnableHandler enableHandler);
	void update(float time, BaseGameObject* object);
private:
	float _time;
	TimerEventHandler _handler;
	TimerEventEnableHandler EnableHandler;
	float _timeCounter;
	float _startTime;
};

class LocalActorData
{
public:
	const ActorData* GameData;
	float Health;
	float Shield;
	float ArmorUpgrade;
	float DamageUpgrade;
	float ShieldUpgrade;

	LocalActorData();
	void init(const ActorData* gameData);
};

class BaseGameObject : public CLinkObject
{
public:
	PlayerObject* Player;
	CLink<BaseGameObject> Target;
	int ID;
	bool Holder;
	float SearchRadius;
	LocalActorData LocalGameData;
	BaseGameObject();
	~BaseGameObject();
	bool InteractionPossible(BaseGameObject* object);
	Node* node();

	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual int ActorType();
	virtual const Vector3 position();
	virtual const Vector3 massCenterPosition();
	virtual void setPosition(const Vector3 pos);
	virtual void setPositionOnServer(const Vector3 pos);
	virtual void interaction(BaseGameObject* object);
	virtual bool volumed();
	virtual bool interactive();
	virtual void update(float time);
	virtual bool deleted();
	virtual void doDamage(BaseGameObject* target);

	void addTimer(const Timer timer);
protected:
	GameObjectManager* _manager;
	AutoRef<Node> _node;
	float getInteractionDistance(BaseGameObject* object);
	Valuable<Vector3> _positionOnServer;
	bool _synkPositionMode;
	std::vector<Timer> _timers;
};

#endif