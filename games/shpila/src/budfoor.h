#ifndef BUDFOOR_H_
#define BUDFOOR_H_

using namespace gameplay;

class BudfoorWarrior : public BaseWarrior
{
public:
	class FireListener : public AnimationClip::Listener
	{
	public:
		BudfoorWarrior& _owner;
		FireListener(BudfoorWarrior& owner) : _owner(owner){}
		void animationEvent(AnimationClip* clip, EventType type) { _owner.fire(); }
	};

	BudfoorWarrior();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void update(float time);
	virtual void rangeFire();
	virtual void attackEnable(bool value);
private:
	void fire();
	SimpleTimer _blinkAbilityColdDownTimer;
	FireListener _fireListener;
	AnimationClip* _attackClip;
};

#endif