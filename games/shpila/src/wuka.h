#ifndef WUKA_H_
#define WUKA_H_

using namespace gameplay;

class WukaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix & transform);
	virtual void updateMovementSpeed(float time);
	virtual void doDamage(BaseGameObject* object);
private:
	SimpleTimer _chargeAbilityTimer;
	SimpleTimer _chargeAbilityColdDownTimer;
	void targetChange();
};



#endif