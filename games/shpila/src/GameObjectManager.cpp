#include "Headers.h"


GameObjectManager::GameObjectManager()
: Players()
, Selected()
, _scene()
, _units()
, _objects()
, _all()
, _pd(NULL)
, _store()
{}

void GameObjectManager::setScene(Scene* scene)
{
	_scene = scene;
}

Scene* GameObjectManager::scene()
{
	return _scene;
}

bool GameObjectManager::initializeNodeMaterials(Node* node)
{
	Model* model = dynamic_cast<Model*>(node->getDrawable());
	if (model)
	{
		for (int i = 0; i < (int)model->getMeshPartCount(); i++)
		{
			initializeMaterial(node, model->getMaterial(i));
		}
	}	
	return true;
}

float CHARACTER_SCALE = 0.007f;

bool GameObjectManager::initializeCharacterNodeScale(Node* node)
{
	Joint* joint = dynamic_cast<Joint*>(node);
	if (joint)
	{
		joint->setScale(CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
	}
	return true;
}

void GameObjectManager::initializeMaterial(Node* node, Material* material)
{
	if (material)
	{
		material->getParameter("u_lightColor")->setVector3(Vector3(1.0f, 1.0f, 1.0f));
		material->getParameter("u_lightDirection")->setVector3(Vector3(-1.0f, -1.0f, -1.0f));
		material->getParameter("u_modulateColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		material->getParameter("u_diffuseColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		material->getParameter("u_modulateAlpha")->setFloat(1.0f);
		material->getParameter("u_pointLightPosition")->setVector3(Vector3(0.0f, 0.0f, 0.0f));
	}	
}

void GameObjectManager::update(const float currentTime, const float elapsedTime)
{
	Itr<BaseGameObject> it = _objects.GetFirst();
	while (it)
	{
		if (it->interactive())
		{
			interaction(it);
		}
		++it;
	}
	it = _objects.GetFirst();
	BaseGameObject* deleted = NULL;
	while (it)
	{
		it->update(elapsedTime);
		if (it->deleted())
			deleted = it;
		++it;
		if (deleted != NULL)
		{
			SAFE_DELETE(deleted);
			deleted = NULL;
		}
	}
}

Itr<BaseGameObject> GameObjectManager::objects()
{
	return _objects.GetFirst();
}

void GameObjectManager::addUnit(const char* filename, const char* name, GameObjectConstructorProc constructor)
{
	if (filename != NULL)
	{
		AutoRef<Scene> scene = Scene::load(filename)->Auto();
		Node* root = Node::create("root");
		root->setTag("dynamic");
		Node* child = scene->getFirstNode();
		Node* childNext;
		while (child != NULL)
		{
			childNext = child->getNextSibling();
			root->addChild(child);
			child = childNext;
		}
		root->setScale(CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
		scene->addNode(root);

		root = root->clone();
		Node* node = root->getFirstChild();
		while (node)
		{
			Animation* animation = node->getAnimation("animations");
			if (!animation)
			{
				animation = node->getAnimation(NULL);
			}
			if (animation)
			{
				std::string scenePath = Scene::getGPBPath(filename);
				int pos = scenePath.find_last_of('.');
				if (pos > 2)
				{
					std::string animationPath = scenePath.substr(0, pos);
					animationPath.append(".animation");
					animation->createClips(animationPath.c_str());
				}
			}
			node = node->getNextSibling();
		}
		if (!_store)
		{
			_store = Scene::create("store")->Auto();
		}
		_store->addNode(root);
		_units[name] = GameUnit(root, constructor);
	}
	else
	{
		_units[name] = GameUnit(NULL, constructor);
	}
}

void GameObjectManager::initUnits()
{
	_store->visit(this, &GameObjectManager::initializeNodeMaterials);
}

BaseGameObject* GameObjectManager::createObject(const char* name, const Matrix& transform, PlayerObject* player)
{
	GP_ASSERT(_scene);

	GameUnit& unit = _units[name];
	BaseGameObject* object = unit._constructor(); 
	object->TypeName = name;
	object->init(*this, &getActorData(name), unit._node, player, transform);
	object->ID = player->getNewObjectID();
	return object;
}

BaseGameObject* GameObjectManager::createObject(const char* name, Vector3 position, Vector3 forwardDirection, PlayerObject* player)
{
	Matrix transform, rotate;
	transform.translate(position);
	createCharacterRotationMatrix(forwardDirection, &rotate);
	transform.rotate(rotate);
	return createObject(name, transform, player);
}

Node* GameObjectManager::AttachUnitModel(const char* unitName, BaseGameObject* object, const char* nodeName)
{
	GameUnit& unit = _units[unitName];
	Node *res = NULL;
	if (unit._node)
	{
		res = unit._node->getFirstChild()->clone();
		object->attachNode(nodeName, res);
	}
	return res;
}

void GameObjectManager::registerMovementController(UnitMovementBase* controller)
{
	controller->init(*_pd);
	_all.push_back(controller);
}

void GameObjectManager::unregisterMovementController(UnitMovementBase* controller)
{
	SAFE_DELETE(controller->proximityToken);
	_all.erase(std::find(_all.begin(), _all.end(), controller));
}

void GameObjectManager::registerObject(BaseGameObject* object)
{
	_objects.Add(object);
}

void GameObjectManager::registerSceneNode(Node* node)
{
	_scene->addNode(node);
	initializeNodeMaterials(node);
}

void GameObjectManager::open(void)
{
	const Vec3 center;
	const float div = 20.0f;
	const Vec3 divisions(div, 1.0f, div);
	const float diameter = 80.0f;
	const Vec3 dimensions(diameter, diameter, diameter);
	typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
	_pd = new LQPDAV(center, dimensions, divisions);
}

void GameObjectManager::interaction(BaseGameObject* object)
{
	Itr<BaseGameObject> it = _objects.GetFirst();
	while (it)
	{
		if (it->interactive() && (it != object))
		{
			if (object->InteractionPossible(it))
			{
				object->interaction(it);
			}
		}
		++it;
	}
}