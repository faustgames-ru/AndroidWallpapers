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

class BaseGameObject : public CLinkObject
{
public:
	PlayerObject* Player;
	Subscribable<BaseGameObject> Target;
	int ID;
	bool Holder;
	float SearchRadius;
	LocalActorData LocalGameData;
	std::string TypeName;
	bool UsedForIllusion;

	BaseGameObject();
	~BaseGameObject();
	bool InteractionPossible(BaseGameObject* object);
	Node* node();
	void attachNode(const char* nodeName, Node* node);
	bool getAura(Aura::Value aura);
	void setAura(Aura::Value aura, bool enabled);
	//return true if distance less value
	void updateMaterials(char* materialsFilename);
	void setTransparency(float transparency);

	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform);
	virtual int ActorType();
	virtual const Vector3 position();
	virtual const Vector3 massCenterPosition();
	virtual void setPosition(const Vector3 pos);
	virtual void setPositionOnServer(const Vector3 pos);
	virtual void interaction(BaseGameObject* object);
	virtual bool volumed();
	virtual bool interactive();
	virtual bool illusionable();//can be used for create illusions
	virtual bool getDetected();
	virtual void setDetected(bool value);
	virtual void update(float time);
	virtual bool deleted();
	virtual void doDamage(BaseGameObject* object);
	virtual bool isAttackToTargetAllowed(BaseGameObject* object);
	virtual void rangeFire();
	virtual float getAttackDistance(BaseGameObject* object);
	virtual float getDetectDistance(BaseGameObject* object);
	virtual float getTargetingDistance(BaseGameObject* object);
	virtual bool checkDistanceToObject(BaseGameObject* object, float value);
	virtual bool checkTargetingDistanceToObject(BaseGameObject* object);
	virtual bool friendly(BaseGameObject* object);

protected:
	GameObjectManager* _manager;
	AutoRef<Node> _node;
	float getInteractionDistance(BaseGameObject* object);
	Valuable<Vector3> _positionOnServer;
	bool _synkPositionMode;
};

#endif