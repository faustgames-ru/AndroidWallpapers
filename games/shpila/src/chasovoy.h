#ifndef CHASOVOY_H_
#define CHASOVOY_H_

using namespace gameplay;

class ChasovoyWarrior : public BaseWarrior
{
public:
	ChasovoyWarrior();
	static BaseGameObject* constructor();
	virtual void init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform);
	virtual void update(float time);
	virtual void interaction(BaseGameObject* object);
private:
	SimpleTimer _guardianShieldTimer;
	SimpleTimer _hallucinationTimer;//Illusions
	bool _allowCreateIllusions;
	Node* _auranode;
};

#endif