#include "Avatar.h"

#include "system/ProjectConfig.h"
#include "surface/SurfaceManager.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

#include <time/Time.h>

#include <engine/Engine.h>
#include <engine/RenderView.h>
#include <input/InputManager.h>

#include "components/transform/Transform.h"
#include "components/rendering/Light.h"
#include "components/mesh/MeshFilter.h"
#include "components/mesh/SkinnedMeshRenderer.h"
#include "components/effects/ParticleSystem.h"
#include "components/miscellaneous/Animation.h"
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "components/physics/CharacterController.h"

#include "OgreBillboard.h"
#include "OgreBillboardSet.h"
#include "OgreLight.h"
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

Avatar::Avatar(const QString& name)
    : mState(IDLE),
	isAvatarMale(false),
	myName(name.toStdString()),
	isChangeAvatar(false),
	lastVisibilityLight(false),
	isActive(false),
	axis(Ogre::Vector2::ZERO),
	teleport(false),
	posTeleport(Vector3::ZERO),
	delayTeleport(0.0f),
	gameObject(nullptr),
	lightObject(nullptr),
	controller(nullptr),
	jumpSpeed(50.0f),
	gravity(20.0f),
	moveDirection(Vector3::ZERO),
	jumpFires(false)
{
	float s = SurfaceManager::getInstance()->kRealTo3D();

	mWalkSpeed = GlobalConfig::SPEED_AVATAR * s;
	mRunSpeed = mWalkSpeed * 10;

	//criando game object
	gameObject = new GameObject(name.toStdString());

	//adicionando componentes
	MeshFilter* meshFilter = new MeshFilter();
	meshFilter->setFileName("avt-woman.mesh");
	gameObject->addComponent(meshFilter);
	gameObject->addComponent(new SkinnedMeshRenderer());
	gameObject->addComponent(new Animation());
	gameObject->addComponent(new ParticleSystem());

	//configurando componentes
	gameObject->renderer->setCastShadows(true);
	gameObject->animation->setLoop(true);
	gameObject->animation->play("idle");
	gameObject->renderer->setEnabled(true);
	gameObject->renderer->setLayer(Physics->IgnoreRaycastLayer());
	//gameObject->transform->turnY(180);

	Vector3 pos = GlobalConfig::INITIAL_POS_AVATAR;
	SurfaceManager::getInstance()->convertRealTo3D(pos);

	///SurfaceManager::getInstance()->getHeightAt(pos, 0.05 * s, true);
	//Vector3 pos(0,1800,0);
	gameObject->transform->setLocalPosition(pos);

	gameObject->particleSystem->setEnableEmission(false);

	//criando Character Controller
	controller = new CharacterController();
	gameObject->addComponent(controller);

	gameObject->transform->setLocalScale(Vector3(s,s,s));



	//criando objeto luz
	lightObject = new GameObject("LightAvatar");
	lightObject->addComponent(new Light());
	lightObject->light->setType(Light::SPOT);
	lightObject->light->setDiffuseColor(Color(0.7, 0.7, 0.7));
	lightObject->light->setSpotlightAngles(35, 55);
	lightObject->light->setAttenuation(500, 0.01, 0.001, 0);
	lightObject->light->setSpotlightNearClipDistance(0.1);
	lightObject->transform->setForward(Vector3(0,1,0));

	//criando BillBoard
	/*billboardSet = GRender->getSceneManager()->createBillboardSet("lightbbsAvatar", 1);
    billboardSet->setMaterialName("Examples/Flare");
	billboardSet->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER);
    billboard = billboardSet->createBillboard(0,0,0, Ogre::ColourValue(0.7, 0.7, 0.7));
	float s2 = 1.28f;
    billboard->setDimensions(s2, s2);

	billboardSet2 = GRender->getSceneManager()->createBillboardSet("lightbbsAvatar2", 1);
    billboardSet2->setMaterialName("Examples/Flare2");
	billboardSet2->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER);
	billboard2 = billboardSet2->createBillboard(0,0,0, Ogre::ColourValue(0.7, 0.7, 0.7));
	float s3 = 1.28f;
    billboard2->setDimensions(s3, s3);*/

	//atachando filhos ao bone da cabeça
	updateLightPosition();
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", light, Quaternion::IDENTITY, dir);
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", billboardSet, Quaternion::IDENTITY, dir);
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", billboardSet2, Quaternion::IDENTITY, dir);

	setActivitiyFlashLight(false);
}

Avatar::~Avatar()
{
	if (gameObject)
	{
		GameObjectManager::getInstance()->destroy(gameObject);
		gameObject = 0;
	}

	if (lightObject)
	{
		GameObjectManager::getInstance()->destroy(lightObject);
		lightObject = 0;
	}
}

void Avatar::updateLightPosition()
{
	float s = SurfaceManager::getInstance()->kRealTo3D();
	double realOffset = 0.0024906;
	Vector3 dir = lightObject->transform->getForward();
	dir.normalise();
	dir = dir * (realOffset * s);

	SkinnedMeshRenderer* skinned = dynamic_cast<SkinnedMeshRenderer*>(gameObject->renderer);

	Vector3 bonePosition = skinned->getBonePosition("Bip001 Flashlight");
	Quaternion boneOrientation = skinned->getBoneOrientation("Bip001 Flashlight");

	Quaternion gameObjectOri = gameObject->transform->getRotation();
	Vector3 boneWorldPos = gameObjectOri * bonePosition + gameObject->transform->getPosition() + dir;
	Quaternion boneWorldOrient = gameObjectOri * boneOrientation;
}

bool Avatar::keyPressed(const OIS::KeyEvent &arg)
{
	if (Input->getRenderView()->isOnRender())
	{
		if (arg.key == OIS::KC_F)
		{
			if(!Input->getKey(OIS::KC_LCONTROL) && !Input->getKey(OIS::KC_LMENU))
			{
				if (!isChangingAvatar())
				{
					//falta
					//CustomPreferences::getInstance()->setOnFlashLight(!isActiveFlashLight());
				}
			}
		}

		if (arg.key == OIS::KC_W  && (!Input->getKey(OIS::KC_LCONTROL)))
		{
			axis.y = -1;
		}
		else if (arg.key == OIS::KC_S)
		{
			axis.y = 1;
		}

		if (arg.key == OIS::KC_A)
		{
			axis.x = -1;
		}
		else if (arg.key == OIS::KC_D)
		{
			axis.x = 1;
		}

		if (arg.key == OIS::KC_SPACE)
		{
			if (controller->isGrounded())
				jumpFires = true;
		}
	}

	return true;
}

bool Avatar::keyReleased(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_W)
	{
		axis.y = 0;
	}
	else if (arg.key == OIS::KC_S)
	{
		axis.y = 0;
	}

	if (arg.key == OIS::KC_A)
	{
		axis.x = 0;
	}
	else if (arg.key == OIS::KC_D)
	{
		axis.x = 0;
	}

	return true;
}

bool Avatar::mouseMoved( const OIS::MouseEvent &arg )
{
	if (Input->isMouseGrabbed())
	{
		if (!isChangingAvatar())
		{
			gameObject->transform->turnY(-arg.state.X.rel * 0.1f, Space::WORLD);
		}
	}

	return true;
}

bool Avatar::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (id == OIS::MB_Right)
	{
		if (!isChangingAvatar())
		{
			//USERLOG->write("avatar posicionado", "evento do mouse");
			teleportAvatar();
		}
	}

	return true;
}

void Avatar::activeParticle(bool a)
{
	gameObject->particleSystem->setEnableEmission(a);
}

void Avatar::removeAvatar()
{

	//light->detachFromParent();
	
	//billboardSet->detachFromParent();
	//billboardSet2->detachFromParent();

	//criando particula de fumaça
	/*if (particleTeleport)
	{
		delete particleTeleport;
		particleTeleport = 0;
	}
	
	if (mStaticEntity)
	{
		mStaticEntity->getOgreSceneNode()->detachAllObjects();
		mStaticEntity->detach(false);
		delete mStaticEntity;
		mStaticEntity = 0;
	}*/

	if (gameObject)
	{
		GameObjectManager::getInstance()->destroy(gameObject);
		gameObject = 0;
	}

	if (lightObject)
	{
		GameObjectManager::getInstance()->destroy(lightObject);
		lightObject = 0;
	}
}

void Avatar::constructAvatar(bool isMale)
{
	float s = SurfaceManager::getInstance()->kRealTo3D();

	//criando game object
	gameObject = new GameObject(myName);

	//adicionando componentes
	MeshFilter* meshFilter = new MeshFilter();
	std::string n = isMale ? "avt-man.mesh" : "avt-woman.mesh";
	meshFilter->setFileName(n);
	gameObject->addComponent(meshFilter);
	gameObject->addComponent(new SkinnedMeshRenderer());
	gameObject->addComponent(new Animation());
	gameObject->addComponent(new ParticleSystem());

	//configurando componentes
	gameObject->renderer->setCastShadows(true);
	gameObject->animation->setLoop(true);
	gameObject->animation->play("idle");
	gameObject->renderer->setEnabled(true);
	gameObject->renderer->setLayer(Physics->IgnoreRaycastLayer());
	//gameObject->transform->turnY(180);

	Vector3 pos = GlobalConfig::INITIAL_POS_AVATAR;
	SurfaceManager::getInstance()->convertRealTo3D(pos);
	SurfaceManager::getInstance()->getHeightAt(pos, 0.05 * s, true);
	gameObject->transform->setLocalPosition(pos);

	gameObject->particleSystem->setEnableEmission(false);

	controller = new CharacterController();
	gameObject->addComponent(controller);
	gameObject->transform->setLocalScale(Vector3(s,s,s));

	//criando objeto luz
	lightObject = new GameObject("LightAvatar");
	lightObject->addComponent(new Light());
	lightObject->light->setType(Light::SPOT);
	lightObject->light->setDiffuseColor(Color(0.7, 0.7, 0.7));
	lightObject->light->setSpotlightAngles(35, 55);
	lightObject->light->setAttenuation(500, 0.01, 0.001, 0);
	lightObject->light->setSpotlightNearClipDistance(0.1);
	lightObject->transform->setForward(Vector3(0,1,0));

	double realOffset = 0.0024906;
	Vector3 dir = lightObject->transform->getForward();
	dir.normalise();
	dir = dir * (realOffset * s);


	updateLightPosition();
	//atachando filhos ao bone da cabeça
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", light, Quaternion::IDENTITY, dir);
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", billboardSet, Quaternion::IDENTITY, dir);
	//mStaticEntity->getOgreEntity()->attachObjectToBone("Bip001 Flashlight", billboardSet2, Quaternion::IDENTITY, dir);
}

bool Avatar::isChangingAvatar()
{
	return isChangeAvatar;
}

void Avatar::changeAvatar(bool isMale)
{
	if (isAvatarMale != isMale)
	{
		isChangeAvatar = true;
		Vector3 worldPos = gameObject->transform->getPosition();
		removeAvatar();
		
		if (isMale)
		{
			constructAvatar(true);
		}
		else
		{
			constructAvatar(false);
		}

		gameObject->transform->setPosition(worldPos);

		isAvatarMale = isMale;
		isChangeAvatar = false;
	}
}

void Avatar::swappActivitiyFlashLight()
{
	bool activityL = !lightObject->light->isEnabled();
	lightObject->light->setEnabled(activityL);
	//billboardSet->setVisible(activityL);
	//billboardSet2->setVisible(activityL);
}

void Avatar::setActivitiyFlashLight(bool active)
{
	if (isActive)
	{
		lightObject->light->setEnabled(active);
		//billboardSet->setVisible(active);
		//billboardSet2->setVisible(active);
	}

	lastVisibilityLight = active;
}

bool Avatar::isActiveFlashLight()
{
	return lightObject->light->isEnabled();
}

void Avatar::setAvatarState(AvatarStates state)
{
    mState = state;
}

AvatarStates Avatar::getAvatarStates() const
{
	
    return mState;
}

void Avatar::setWalkSpeed(const float speed)
{
    mWalkSpeed = speed;
}

void Avatar::setRunSpeed(const float speed)
{
    mRunSpeed = speed;
}

float Avatar::getWalkSpeed() const
{
    return mWalkSpeed;
}

float Avatar::getRunSpeed() const
{
    return mRunSpeed;
}

GameObject* Avatar::getGameObject() const
{
	return gameObject;
}

void Avatar::move()
{
	double delta = Time->getDeltaTime();

	if (!teleport)
	{
		bool isMoving = false;
		if( axis.x != 0 || axis.y != 0 )
		{
			isMoving = true;
			if( Input->getKey(OIS::KC_LSHIFT) )
			{
				/*Vector3 oldPos = gameObject->transform->getPosition();
				
				gameObject->transform->translate(-axis.x * mRunSpeed * delta, 0,
										 -axis.y * mRunSpeed * delta);

				Vector3 newPos = gameObject->transform->getPosition();

				float s = SurfaceManager::getInstance()->kRealTo3D();
				if (!SurfaceManager::getInstance()->isSurfaceCollision(oldPos, 0.05 * s, newPos, QF_CUSTOM1,false))
				{
				#ifdef MINE_UG
					if (SurfaceManager::getInstance()->getHeightAt(newPos, 0.05 * s, true, false, false))
						lastValidPosition = newPos;
				#endif
				}
			
				gameObject->transform->setPosition(newPos);*/

				if (!jumpFires)
				{
					gameObject->animation->play("run");
					mState = RUNNING;
				}	
			}
			else
			{
				/*Vector3 oldPos = gameObject->transform->getPosition();

				gameObject->transform->translate(-axis.x * mWalkSpeed * delta, 0,
										 -axis.y * mWalkSpeed * delta);
			
				Vector3 newPos = gameObject->transform->getPosition();

				float s = SurfaceManager::getInstance()->kRealTo3D();
				if (!SurfaceManager::getInstance()->isSurfaceCollision(oldPos, 0.05 * s, newPos, QF_CUSTOM1, false))
				{
				#ifdef MINE_UG
					if (SurfaceManager::getInstance()->getHeightAt(newPos, 0.05 * s, true, false, false))
						lastValidPosition = newPos;
				#endif
				}

				gameObject->transform->setPosition(newPos);

				*/

				if (!jumpFires)
				{
					gameObject->animation->play("walk");
					mState = WALKING; 
				}
			}
			/*
		#ifdef MINE_UG
			if (!hasGround(terrain))
			{
				mStaticEntity->setWorldPosition(lastValidPosition);
			}
		#endif*/

			/*if (!SurfaceManager::getInstance()->isInside(lastValidPosition))
			{
				gameObject->transform->setPosition(lastValidPosition);
			}*/

			//proprio manager já atualiza componentes
			//mStaticEntity->advanceAnimation(delta);
		}
		else if (!jumpFires)
		{
			mState = IDLE;

			gameObject->animation->play("idle");
		}

		//atualizando character controller física
		if (controller->isGrounded())
		{
			//if (isMoving)
			{
				moveDirection = Vector3(-axis.x, 0, -axis.y);
				moveDirection = gameObject->transform->transformDirection(moveDirection);
			}
			
			if (jumpFires)
			{
				moveDirection *= mRunSpeed;
				moveDirection.y = jumpSpeed;
				jumpFires = false;
			}
			else
			{
				moveDirection *= mState == WALKING ? mWalkSpeed : mRunSpeed;
			}
		}

		moveDirection.y -= gravity * delta;
		controller->move(moveDirection * delta, false);
	}
	else
	{
		delayTeleport += delta;
		if (delayTeleport >= 0.4f)
		{
			gameObject->transform->setPosition(posTeleport);
			delayTeleport = 0.0f;
			teleport = false;
			gameObject->renderer->setEnabled(true);
			activeParticle(false);
		}
	}
}

void Avatar::stop()
{
    if (mState != IDLE)
    {
        mState = IDLE;
    }
}

void Avatar::setActive(bool active)
{
	gameObject->renderer->setEnabled(active);

	if (active)
	{
		Vector3 pos = gameObject->transform->getLocalPosition();
		SurfaceManager::getInstance()->getHeightAt(pos, 0.05 * SurfaceManager::getInstance()->kRealTo3D(), true);

		gameObject->transform->setLocalPosition(pos);

		Input->addKeyListener(this);
		Input->addMouseListener(this);
	}
	else
	{
		Input->removeKeyListener(this);
		Input->removeMouseListener(this);
	}

	if (active && lastVisibilityLight)
	{
		lightObject->light->setEnabled(true);
		//billboardSet->setVisible(true);
		//billboardSet2->setVisible(true);

	}
	else
	{
		lightObject->light->setEnabled(false);
		//billboardSet->setVisible(false);
		//billboardSet2->setVisible(false);
		
	}

	isActive = active;
}

bool Avatar::hasGround()
{
	Vector3 p = gameObject->transform->getPosition();
	
	if (SurfaceManager::getInstance()->getHeightAt(p, 0.05 * SurfaceManager::getInstance()->kRealTo3D(), true))
	{
		return true;
	}

	return false;
}

void Avatar::teleportAvatar()
{
	if( Input->getRenderView()->isActiveWindow() )
    {
        //std::cout << "mouseRightClicked\n";
        float mX = Input->mouseX();
        float mY = Input->mouseY();
        Vector2 pos = Input->getRenderView()->getAbsolutePosition();
        float largura = float( Input->getRenderView()->getRenderWindow()->getWidth());
        float altura = float( Input->getRenderView()->getRenderWindow()->getHeight());

		

		Vector3 mousePos = SurfaceManager::getInstance()->
                                    mouseWorldPosition(mX - pos.x,
													   mY - pos.y,
													   largura,
													   altura );
        if( mousePos.x != 599994
                && mousePos.y != 599994
                && mousePos.z != 599994 )
        {
            teleport = true;
            posTeleport = mousePos + Vector3(0, 0.05 * SurfaceManager::getInstance()->kRealTo3D(),0);
            //mAvatar->getEntity()->setWorldPosition(mousePos);
			activeParticle(true);
			gameObject->renderer->setEnabled(false);
        }
    }
}