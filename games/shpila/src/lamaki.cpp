#include "Headers.h"

LamakiWarrior::LamakiWarrior()
: BaseWarrior()
, _blinkAbilityColdDownTimer()
, _fireListener(*this)
, _attackClip()
{}

BaseGameObject* LamakiWarrior::constructor()
{
	return new LamakiWarrior();
}

void LamakiWarrior::init(GameObjectManager& manager, const ActorData* gameData, Node* node, PlayerObject* player, Matrix transform)
{
	float scale = LAMAKI_SCALE;
	BaseWarrior::init(manager, gameData, node, player, transform);
	_node->setScale(scale * 1.25f, scale, scale * 1.25f);
	_blinkAbilityColdDownTimer.enable(false);

	if (_unitAnimation.size() > 0)
	{
		_attackClip = (*_unitAnimation[0]->_clips)[UnitAnimation::Attack];
		_attackClip->addListener(&_fireListener, (int)(0.25f * (float)_attackClip->getDuration()));
	}
	_damageTimer.enable(false);
}

void LamakiWarrior::update(float time)
{
	BaseWarrior::update(time);

	if (Player->upgrades()->getUpgrade(Upgrades::LamakiUpgrade) && !_blinkAbilityColdDownTimer.enabled())
	{
		if (Target && (Target->position().distanceSquared(position()) < SQR(LAMAKI_BLINK_DISTANCE)))
		{
			if (LocalGameData.Health <= ((LocalGameData.GameData->HP + LocalGameData.GameData->shield) * LAMAKI_BLINK_MIN_HEALTH_PERCENT))
			{
				setPosition(position() - _node->getForwardVectorWorld().normalize() * LAMAKI_BLINK_DISTANCE);
				_blinkAbilityColdDownTimer.enable(true);
				_blinkAbilityColdDownTimer.start(LAMAKI_BLINK_COLDDOWN, 0.0f);
			}
		}
	}
	if (_blinkAbilityColdDownTimer.action(time))
		_blinkAbilityColdDownTimer.enable(false);
	Node* hand = _node->findNode("weapon");//Bip001 R Hand //weapon
	Node* weapon = _node->findNode("obj_weapon");
	/*if ((_attackClip != NULL) && _attackClip->isPlaying() && (Target != NULL))
	{
		Matrix handMat = hand->getWorldMatrix();
		Vector3 translate;
		handMat.getTranslation(&translate);
		translate = Vector3(lerp(btVector3(translate.x, translate.y, translate.z),
			btVector3(Target->position().x, Target->position().y, Target->position().z), _attackClip->getElapsedTime() / _attackClip->getDuration()));
		handMat.m[12] = translate.x;
		handMat.m[13] = translate.y;
		handMat.m[14] = translate.z;
		weapon->set(handMat);
	}
	else*/
	{
		weapon->set(hand->getWorldMatrix());
	}
}

void LamakiWarrior::rangeFire()
{
	Node* holder = _node->findNode("weapon");

	Matrix mRes;
	Matrix mScale;
	Matrix mHolder;
	Matrix mCharacterRoot = _node->getWorldMatrix();
	Matrix::createScale(1, 1, 1, &mScale);
	Matrix::multiply(holder->getWorldMatrix(), mScale, &mHolder);
	Matrix::multiply(mCharacterRoot, mHolder, &mRes);

	Vector3 trn;
	mRes.getTranslation(&trn);
	BoomerangBullet* bullet = (BoomerangBullet*)_manager->createObject("boomerang", trn, _node->getForwardVectorWorld().normalize(), Player);
	bullet->Target = Target;
	bullet->LocalGameData.init(LocalGameData.GameData, LocalGameData.Upgrades);
}

void LamakiWarrior::attackEnable(bool value)
{

}

void LamakiWarrior::fire()
{
	doDamage(Target);
}