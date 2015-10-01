#ifndef BUDFOOR_H_
#define BUDFOOR_H_

using namespace gameplay;

class BudfoorWarrior : public BaseWarrior
{
public:
	BudfoorWarrior();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void update(float time);
private:
	SimpleTimer _blinkAbilityColdDownTimer;
};

#endif