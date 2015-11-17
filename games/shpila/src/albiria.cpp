#include "Headers.h"

BaseGameObject* AlbiriaWarrior::constructor()
{
	return new AlbiriaWarrior();
}

AlbiriaWarrior::AlbiriaWarrior()
: BaseWarrior()
, _fireListener(*this)
, _rechangeListener(*this)
{
}

void AlbiriaWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, const Matrix transform)
{
	float scale = ALBIRIA_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale, scale, scale);

	ActionClips* clips = (ActionClips*)_unitActions.Actions[UnitActions::Attack];
	clips->addBeginListener(&_rechangeListener);
	clips->addListener(&_fireListener, (int)(0.25f * (float)clips->getDuration()));
}

void AlbiriaWarrior::update(float time)
{
	BaseWarrior::update(time);
	/*Node* holder = _node->findNode("Bip001 Xtra02");
	Node* bullet = _node->findNode("bullet");
	Matrix mRes;
	Matrix mScale;
	Matrix::createScale(3, 3, 3, &mScale);
	Matrix::multiply(holder->getWorldMatrix(), mScale, &mRes);
	bullet->set(mRes);
	if (!_dead)
	{
		Quaternion rot;
		Quaternion::multiply(_node->getRotation(), Quaternion(Vector3(0.0f, 1.0f, 0.0f), MATH_PI), &rot);
		_node->setRotation(rot);
	}*/
}

void AlbiriaWarrior::disappearing(float time)
{
	BaseWarrior::disappearing(time);	
}

void AlbiriaWarrior::fire()
{
	/*Node* bullet = _node->findNode("bullet");
	Node* holder = _node->findNode("Bip001 Xtra02");

	Matrix mRes;
	Matrix mScale;
	Matrix mHolder;
	Matrix mCharacterRoot = _node->getWorldMatrix();
	Matrix::createScale(3, 3, 3, &mScale);
	Matrix::multiply(holder->getWorldMatrix(), mScale, &mHolder);
	Matrix::multiply(mCharacterRoot, mHolder, &mRes);
	
	BaseBullet* object = new BaseBullet();
	object->Target = Target;
	object->init(*_manager, bullet, PlayerID, mRes);

	bullet->setEnabled(false);*/
}

void AlbiriaWarrior::recharge()
{
	/*Node* bullet = _node->findNode("bullet");
	bullet->setEnabled(true);*/
}