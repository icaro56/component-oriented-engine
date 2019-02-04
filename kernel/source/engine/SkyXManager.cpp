#include "engine/SkyXManager.h"
#include <engine/Engine.h>
#include <engine/RenderingEngine.h>
#include <engine/Scene.h>
#include <engine/RenderView.h>

#include <components/rendering/Camera.h>
#include <components/rendering/Light.h>
#include <components/transform/Transform.h>
#include <gameobject/GameObject.h>
#include <gameobject/LayerMask.h>
//#include <render/LightOld.h>

#include "time/Time.h"
#include "utils/KernelConverter.h"

/****************************** Structs *************************************/
SkyXSettings::SkyXSettings(	 const Vector3 t,
							 const Real& tm,
							 const Real& mp,
							 const SkyX::AtmosphereManager::Options& atmOpt,
							 const bool& lc,
							 const bool& vc,
							 const Real& vcws,
							 const bool& vcauto,
		 					 const Radian& vcwd,
							 const Vector3& vcac,
							 const Vector4& vclr,
							 const Vector4& vcaf,
							 const Vector2& vcw,
							 const bool& vcl,
							 const Real& vclat,
							 const Vector3& vclc,
							 const Real& vcltm)
:	time(t), 
	timeMultiplier(tm), 
	moonPhase(mp), 
	atmosphereOpt(atmOpt), 
	layeredClouds(lc), 
	volumetricClouds(vc), 
	vcWindSpeed(vcws), 
	vcAutoupdate(vcauto), 
	vcWindDir(vcwd), 
	vcAmbientColor(vcac), 
	vcLightResponse(vclr), 
	vcAmbientFactors(vcaf), 
	vcWheater(vcw), 
	vcLightnings(vcl), 
	vcLightningsAT(vclat), 
	vcLightningsColor(vclc), 
	vcLightningsTM(vcltm)
{

}

SkyXSettings::SkyXSettings(	 const Vector3 t, 
							 const Real& tm, 
							 const Real& mp, 
							 const SkyX::AtmosphereManager::Options& atmOpt,
							 const bool& lc, 
							 const bool& vc, 
							 const Real& vcws, 
							 const bool& vcauto, 
							 const Radian& vcwd, 
							 const Vector3& vcac, 
							 const Vector4& vclr,  
							 const Vector4& vcaf, 
							 const Vector2& vcw)
:	time(t), 
	timeMultiplier(tm), 
	moonPhase(mp), 
	atmosphereOpt(atmOpt), 
	layeredClouds(lc), 
	volumetricClouds(vc),
	vcWindSpeed(vcws), 
	vcAutoupdate(vcauto), 
	vcWindDir(vcwd), 
	vcAmbientColor(vcac), 
	vcLightResponse(vclr), 
	vcAmbientFactors(vcaf), 
	vcWheater(vcw), 
	vcLightnings(false)
{

}

SkyXSettings::SkyXSettings(	 const Vector3 t, 
							 const Real& tm, 
							 const Real& mp, 
							 const SkyX::AtmosphereManager::Options& atmOpt, 
							 const bool& lc)
:	time(t), 
	timeMultiplier(tm), 
	moonPhase(mp), 
	atmosphereOpt(atmOpt), 
	layeredClouds(lc), 
	volumetricClouds(false), 
	vcLightnings(false)
{

}

/************************************* Classes ******************************/
SkyXManager* SkyXManager::instance = nullptr;

SkyXManager* SkyXManager::getInstance()
{
	if (!instance)
		instance = new SkyXManager();

	return instance;
}

SkyXManager::SkyXManager()
: mSkyX(nullptr),
  mBasicController(nullptr),
  mCurrentPreset(4),
  active(false),
  parentScene(nullptr)
{
	// Pôr do sol: segundo construtor
	mPresets[0] = SkyXSettings(Vector3(8.85f, 7.5f, 20.5f),  
							   -0.08f, 
							   0, 
							   SkyX::AtmosphereManager::Options(9.77501f, 
																10.2963f, 
																0.01f, 
																0.0022f, 
																0.000675f, 
																30, 
																Ogre::Vector3(0.57f, 0.52f, 0.44f), 
																-0.991f, 
																3, 
																4), 
							   false, 
							   true, 
							   300, 
							   false, 
							   Radian(270), 
							   Vector3(0.63f,0.63f,0.7f), 
							   Vector4(0.35, 0.2, 0.92, 0.1), 
							   Vector4(0.4, 0.7, 0, 0), 
							   Vector2(0.8,1));
	// Limpo: terceiro construtor
	mPresets[1] = SkyXSettings(Vector3(17.16f, 7.5f, 20.5f), 
							   0, 
							   0, 
							   SkyX::AtmosphereManager::Options(9.77501f, 
																10.2963f, 
																0.01f, 
																0.0017f, 
																0.000675f, 
																30, 
																Ogre::Vector3(0.57f, 0.54f, 0.44f), 
																-0.991f, 
																2.5f, 
																4), 
							   false);
	// Thunderstorm 1: primeiro construtor
	mPresets[2] = SkyXSettings(Vector3(12.23, 7.5f, 20.5f),  
							   0, 
							   0, 
							   SkyX::AtmosphereManager::Options(9.77501f, 
																10.2963f, 
																0.01f, 
																0.00545f, 
																0.000375f, 
																30, 
																Ogre::Vector3(0.55f, 0.54f, 0.52f), 
																-0.991f, 
																1, 
																4), 
							   false, 
							   true, 
							   300, 
							   false, 
							   Radian(0), 
							   Vector3(0.63f,0.63f,0.7f), 
							   Vector4(0.25, 0.4, 0.5, 0.1), 
							   Vector4(0.45, 0.3, 0.6, 0.1), 
							   Vector2(1,1), 
							   true, 
							   0.5, 
							   Vector3(1,0.976,0.92), 
							   2);
	// Thunderstorm 2: primeiro construtor
	mPresets[3] = SkyXSettings(Vector3(10.23, 7.5f, 20.5f),  
							   0, 
							   0, 
							   SkyX::AtmosphereManager::Options(9.77501f, 
																10.2963f, 
																0.01f, 
																0.00545f, 
																0.000375f, 
																30, 
																Ogre::Vector3(0.55f, 0.54f, 0.52f), 
																-0.991f, 
																0.5, 
																4), 
							   false, 
							   true, 
							   300, 
							   false, 
							   Radian(0), 
							   Vector3(0.63f,0.63f,0.7f), 
							   Vector4(0, 0.02, 0.34, 0.24), 
							   Vector4(0.29, 0.3, 0.6, 1), 
							   Vector2(1,1), 
							   true, 
							   0.5, 
							   Vector3(0.95,1,1), 
							   2);
	// Deserto: terceiro construtor
	mPresets[4] = SkyXSettings(Vector3(5.5f, 6.0f, 18.0f), 
							   0.0, 
							   -0.8f, 
							   SkyX::AtmosphereManager::Options(9.77501f, 
															    10.2963f, 
																0.01f, 
																0.0072f, 
																0.000925f, 
																30, 
																Ogre::Vector3(0.71f, 0.59f, 0.53f), 
																-0.997f, 
																2.5f, 
																1), 
							   true);
	// Night: terceiro construtor
	mPresets[5] = SkyXSettings(Vector3(21.5f, 7.5, 20.5), 
							   0.03, 
							   -0.25, 
							   SkyX::AtmosphereManager::Options(), 
							   true);
}

void SkyXManager::initSky(RenderView *renderView, Camera *mRenderingCamera, Vector2 &east, const QTime& t, Scene* scene)
{
	// criando o skyX
	parentScene = scene;
	mBasicController = new SkyX::BasicController();
	mSkyX = new SkyX::SkyX(scene->getOgreScene(), mBasicController);
	mSkyX->create();

	// Distance geometry falling is a feature introduced in SkyX 0.2
	// When distance falling is enabled, the geometry linearly falls with the distance and the
	// amount of falling in world units is determinated by the distance between the cloud field "plane"
	// and the camera height multiplied by the falling factor.
	// For this demo, a falling factor of two is good enough for the point of view we're using. That means that if the camera
	// is at a distance of 100 world units from the cloud field, the fartest geometry will fall 2*100 = 200 world units.
	// This way the cloud field covers a big part of the sky even if the camera is in at a very low altitude.
	// The second parameter is the max amount of falling distance in world units. That's needed when for example, you've an 
	// ocean and you don't want to have the volumetric cloud field geometry falling into the water when the camera is underwater.
	// -1 means that there's not falling limit.
	mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));

	// Register SkyX listeners
	Rendering->getRoot()->addFrameListener(mSkyX);
	renderView->getRenderWindow()->addListener(mSkyX);

	east.normalise();
	setPreset(mCurrentPreset, mRenderingCamera, east, t);

	setActive(true);
}

SkyXManager::~SkyXManager()
{
	mSkyX->remove();

	delete mSkyX;
	mSkyX = nullptr;
}

void SkyXManager::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

void SkyXManager::setPreset(unsigned nPreset, Camera *mRenderingCamera, Vector2 &east, const QTime& t)
{
	mCurrentPreset = nPreset;
	const SkyXSettings & preset = mPresets[nPreset];

	mSkyX->setTimeMultiplier(preset.timeMultiplier);

	//não permitindo que sejam selecionáveis, pois usamos 1<<1 pra objetos selecionáveis
	unsigned layer = LayerMask::nameToLayer("IgnoreRaycast");
	mSkyX->getMeshManager()->getEntity()->setQueryFlags( layer );
	mSkyX->getMoonManager()->getMoonBillboard()->setQueryFlags ( layer );

	mBasicController->setTime(Convert::toOgre(preset.time));
	float h = t.hour() + t.minute() * 0.016f + t.second() / 3600.0f;
	setTime(h);

	mBasicController->setMoonPhase(preset.moonPhase);
	mBasicController->setEastDirection(Convert::toOgre(east));

	mSkyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

	// Layered clouds
	if (preset.layeredClouds)
	{
		// Create layer cloud
		if (mSkyX->getCloudsManager()->getCloudLayers().empty())
		{
			mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
		}
	}
	else
	{
		// Remove layer cloud
		if (!mSkyX->getCloudsManager()->getCloudLayers().empty())
		{
			mSkyX->getCloudsManager()->removeAll();
		}
	}

	mSkyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
	mSkyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

	SkyX::VClouds::VClouds* vclouds = mSkyX->getVCloudsManager()->getVClouds();

	vclouds->setWindDirection(Convert::toOgre(preset.vcWindDir));
	vclouds->setAmbientColor(Convert::toOgre(preset.vcAmbientColor));
	vclouds->setLightResponse(Convert::toOgre(preset.vcLightResponse));
	vclouds->setAmbientFactors(Convert::toOgre(preset.vcAmbientFactors));
	vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

	if (preset.volumetricClouds)
	{
		// Create VClouds
		if (!mSkyX->getVCloudsManager()->isCreated())
		{
			// SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
			mSkyX->getVCloudsManager()->create(mSkyX->getMeshManager()->getSkydomeRadius(mRenderingCamera->getOgreCamera()));
		}
	}
	else
	{
		// Remove VClouds
		if (mSkyX->getVCloudsManager()->isCreated())
		{
			mSkyX->getVCloudsManager()->remove();
		}
	}

	vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
	vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
	vclouds->getLightningManager()->setLightningColor(Convert::toOgre(preset.vcLightningsColor));
	vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);

	//para informar qual céu está ativo
	//mTextArea->setCaption(buildInfoStr());

	// Reset camera position/orientation
	//mRenderingCamera->setPosition(0,0,0);
	//mRenderingCamera->setDirection(0,0,1);

	mSkyX->update(0);

}

unsigned SkyXManager::getPreset()
{
	return mCurrentPreset;
}

SkyX::SkyX* SkyXManager::getSkyX()
{
	return mSkyX;
}

void SkyXManager::setTime(float hour)
{
	Ogre::Vector3 time = mBasicController->getTime();
	time.x = hour;
	mBasicController->setTime(time);
}

void SkyXManager::updateLightDir(GameObject* lightObject)
{
	Ogre::Vector3 time = mBasicController->getTime();
	Ogre::Vector3 dir;
	float luscofusco = 0.5f;

	if (time.x > time.y + luscofusco && time.x < time.z - luscofusco)
	{
		lightObject->light->setCastShadows(true);
		parentScene->setShadowColor(0.6, 0.6, 0.6);
		lightObject->light->setDiffuseColor(0.7, 0.7, 0.7);
		dir = mBasicController->getSunDirection() * -1;
	}
	else if (time.x < time.y - luscofusco || time.x > time.z + luscofusco)
	{
		lightObject->light->setCastShadows(true);
		parentScene->setShadowColor(0.8, 0.8, 0.8);
		lightObject->light->setDiffuseColor(0.5, 0.5, 0.5);
		dir = mBasicController->getMoonDirection() * -1;
	}
	else
	{
		lightObject->light->setCastShadows(false);
	}

	lightObject->transform->setForward(Convert::toKernel(dir));
 }

void SkyXManager::setEastDirection(Vector2 &dir)
{
	dir.normalise();
	mBasicController->setEastDirection(Convert::toOgre(dir));
}

void SkyXManager::updateAll(GameObject* lightObject, const QTime& t)
{
	updateLightDir(lightObject);

	float h = t.hour() + t.minute() * 0.016f + t.second() / 3600.0f;
	setTime(h);
}

void SkyXManager::setActive(bool act)
{
	active = act;
}

bool SkyXManager::isActive()
{
	return active;
}

void SkyXManager::forceUpdate()
{
	mSkyX->update(0);
}

//-----------------------------------------------------------------------------
void SkyXManager::updateSkyXTime(GameObject* lightObject, const QTime &t)
{
	updateAll(lightObject, t);
	forceUpdate();
}

//-----------------------------------------------------------------------------
void SkyXManager::updateSkyXCurrentTime(GameObject* lightObject)
{
	QTime t = Time->getDateTime().time();
	updateAll(lightObject,t);
	forceUpdate();
}