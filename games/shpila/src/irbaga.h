#ifndef IRBAGA_H_
#define IRBAGA_H_

using namespace gameplay;

class IrbagaWarrior : public BaseWarrior
{
public:
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void updateMovementSpeed(float time);
private:
	SimpleTimer _chargeAbilityTimer;
	SimpleTimer _chargeAbilityColdDownTimer;
};



#endif