#ifndef ALBIRIA_H_
#define ALBIRIA_H_

using namespace gameplay;

class AlbiriaWarrior : public BaseWarrior
{
public:
	class FireListener : public AnimationClip::Listener
	{
	public:
		AlbiriaWarrior& _owner;
		FireListener(AlbiriaWarrior& owner): _owner(owner){}
		void animationEvent(AnimationClip* clip, EventType type) { _owner.fire(); }
	};

	class RechargeListener : public AnimationClip::Listener
	{
	public:
		AlbiriaWarrior& _owner;
		RechargeListener(AlbiriaWarrior& owner) : _owner(owner){}
		void animationEvent(AnimationClip* clip, EventType type) { _owner.recharge(); }
	};
	static BaseGameObject* constructor();
	AlbiriaWarrior();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
	virtual void update(float time);
	virtual void disappearing(float time);
	void fire();
	void recharge();
private:
	FireListener _fireListener;
	RechargeListener _rechangeListener;
};



#endif