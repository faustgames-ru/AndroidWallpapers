#ifndef LAMAKI_H_
#define LAMAKI_H_

using namespace gameplay;

class LamakiWarrior : public BaseWarrior
{
public:
	class FireListener : public AnimationClip::Listener
	{
	public:
		LamakiWarrior& _owner;
		FireListener(LamakiWarrior& owner) : _owner(owner){}
		void animationEvent(AnimationClip* clip, EventType type) { _owner.fire(); }
	};

	LamakiWarrior();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
	virtual void update(float time);
	virtual void rangeFire();
	virtual void attackEnable(bool value);
private:
	void fire();
	SimpleTimer _blinkAbilityColdDownTimer;
	FireListener _fireListener;
	AutoRef<ActionClips> _attackClip;
};

#endif