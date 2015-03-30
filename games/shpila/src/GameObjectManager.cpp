#include "Headers.h"

GameObjectManager::GameObjectManager()
: Players()
, _scene()
, _units()
, _objects()
, _all()
, _pd(NULL)
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

void GameObjectManager::initializeMaterial(Node* node, Material* material)
{
	// Bind light shader parameters to dynamic objects only
	if (node->hasTag("dynamic"))
	{
		material->getParameter("u_ambientColor")->bindValue((Scene*)_scene, &Scene::getAmbientColor);
		Node* lightNode = _scene->findNode("sun");
		if (lightNode)
		{
			lightNode->setLight(Light::createPoint(Vector3(0.7f, 0.75f, 0.65f), 1000.0f));
			//material->getParameter("u_directionalLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
			//material->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorView);
			material->getParameter("u_lightColor")->bindValue(lightNode->getLight(), &Light::getColor);
			material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
			material->getParameter("u_modulateColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			material->getParameter("u_diffuseColor")->setVector4(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			material->getParameter("u_modulateAlpha")->setFloat(1.0f);
			material->getParameter("u_pointLightPosition")->setVector3(Vector3(0.0f, 0.0f, 0.0f));
			material->getParameter("u_pointLightRangeInverse")->setFloat(lightNode->getLight()->getRangeInverse());
		}
	}
}

void GameObjectManager::update(const float currentTime, const float elapsedTime)
{
	Itr<BaseGameObject> it = _objects.GetFirst();
	while (it)
	{
		interaction(it);
		++it;
	}
	it = _objects.GetFirst();
	while (it)
	{
		it->update(elapsedTime);
		++it;
	}
}

void GameObjectManager::addUnit(const char* name, Node* node, GameObjectConstructorProc constructor)
{
	_units[name] = GameUnit(node, constructor);
}

BaseGameObject* GameObjectManager::createObject(const char* name, Vector3 position, int playerID)
{
	GP_ASSERT(_scene);

	GameUnit& unit = _units[name];
	BaseGameObject* object = unit._constructor();
	object->init(*this, unit._node, playerID, position);
	return object;
}

void GameObjectManager::registerMovementController(UnitMovementBase* controller)
{
	controller->init(*_pd);
	_all.push_back(controller);
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
		if (object != it)
		{
			if (object->InteractionPossible(it))
			{
				object->interaction(it);
			}
		}
		++it;
	}
}