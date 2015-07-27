#include "TargetCamera.h"

TargetCamera::TargetCamera()
    : _pitchNode(NULL), _rootNode(NULL)
{
    
}

TargetCamera::~TargetCamera()
{
    SAFE_RELEASE(_pitchNode);
    SAFE_RELEASE(_rootNode);
}

void TargetCamera::initialize(float nearPlane, float farPlane, float fov)
{
    SAFE_RELEASE(_pitchNode);
    SAFE_RELEASE(_rootNode);
    _rootNode = Node::create("TargetCamera_root");
    _pitchNode = Node::create("TargetCamera_pitch");
    _rootNode->addChild(_pitchNode);

    float aspectRatio = Game::getInstance()->getAspectRatio();
    assert(aspectRatio > 0.0f);
    Camera* camera = Camera::createPerspective(fov, aspectRatio, nearPlane, farPlane);
    _pitchNode->setCamera(camera);
    SAFE_RELEASE(camera);
}

void TargetCamera::setTargetNode(Node* target)
{
	_targetNode = target;
	_targetNode->addChild(getRootNode());
}

Node* TargetCamera::getRootNode()
{
    return _rootNode;
}

Camera* TargetCamera::getCamera()
{
    if (_pitchNode)
        return _pitchNode->getCamera();
    return NULL;
}

const Matrix TargetCamera::getTransform()
{
	Matrix res;
	_pitchNode->getRotation(&res);
	//res.translate(_rootNode->getTranslation());
	res.m[12] = _rootNode->getTranslation().x;
	res.m[13] = _rootNode->getTranslation().y;
	res.m[14] = _rootNode->getTranslation().z;
	return res;
}

void TargetCamera::setRotation(const Matrix& rotation)
{
	_pitchNode->set(Vector3(1.0f, 1.0f, 1.0f),
		rotation, Vector3(0.0f, 0.0f, 0.0f));
}

const Vector3& TargetCamera::getPosition()
{
	return _rootNode->getTranslation();
}

void TargetCamera::setPosition(const Vector3& position)
{
    _rootNode->setTranslation(position);
}

void TargetCamera::moveForward(float amount)
{
    Vector3 v = _pitchNode->getForwardVectorWorld();
    v.normalize().scale(amount);
    _rootNode->translate(v);
}

void TargetCamera::moveBackward(float amount)
{
    moveForward(-amount);
}

void TargetCamera::moveLeft(float amount)
{
    _rootNode->translateLeft(amount);
}

void TargetCamera::moveRight(float amount)
{
    _rootNode->translateLeft(-amount);
}

void TargetCamera::moveUp(float amount)
{
    _rootNode->translateUp(amount);
}

void TargetCamera::moveDown(float amount)
{
    _rootNode->translateUp(-amount);
}

void TargetCamera::rotate(float yaw, float pitch)
{
    _rootNode->rotateY(-yaw);
    _pitchNode->rotateX(pitch);
}
