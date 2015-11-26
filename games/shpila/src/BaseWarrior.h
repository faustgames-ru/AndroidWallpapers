#ifndef BASEWARRIOR_H_
#define BASEWARRIOR_H_

using namespace gameplay;

class BaseGameObject;
class PlayerObject;

class ActionClips: public Ref
{
public:
	AutoRef<AnimationClip> CurrentClip;
	std::vector<AutoRef<AnimationClip>> Clips;

	ActionClips();

	bool Playing();
	void Play();
	void addBeginListener(AnimationClip::Listener* listener);
	void addListener(AnimationClip::Listener* listener, unsigned long eventTime);
	unsigned long getDuration();
};

class UnitActions
{
public:
	enum Action
	{
		Run,
		Attack,
		Death,
		Dead
	};
	typedef std::map<Action, AutoRef<ActionClips>> ActionsMap;

	AutoRef<Animation> _animation;

	AutoRef<ActionClips> CurrentAction;
	ActionsMap Actions;

	UnitActions()
		: CurrentAction()
		, Actions()
	{}
};

class BaseWarrior : public BaseActor
{
public:
	BaseWarrior();
	~BaseWarrior();
	static BaseGameObject* constructor();
	void makeIllusion();

	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
	virtual bool illusionable();
	virtual void interaction(BaseGameObject* object);
	virtual void update(float time);
	virtual void updateMovementSpeed(float time);
	virtual bool deleted();
	virtual void disappearing(float time);

protected:
	bool _initialized;
	bool _dead;
	float _deadAltitude;
	SimpleTimer _illusionTimer;
	UnitActions _unitActions;
	void updateMidLineState();
	void updateAnimationState();
	void switchToAction(UnitActions::Action action);
	void updatePositionFromServer(float time);
	void updateAttack(float time, BaseGameObject* object);
	void updateMoveToPoint(float time, Vector3 point);
};

#endif