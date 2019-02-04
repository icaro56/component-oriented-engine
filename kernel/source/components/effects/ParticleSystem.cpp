#include "effects/ParticleSystem.h"

#include "engine/Engine.h"
#include "engine/Scene.h"
#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"
#include "transform/Transform.h"

#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"
#include "utils/KernelConverter.h"

ParticleSystem::ParticleSystem()
	: Component("ParticleSystem", "ParticleSystem"),
	  mParticleSystem(nullptr),
      mParticleEmitter(nullptr),
	  templateName("")
{
	
}

ParticleSystem::ParticleSystem(const ComponentDescMap& d)
	: Component("ParticleSystem", "ParticleSystem"),
	  mParticleSystemDescMap(d),
	  mParticleSystem(nullptr),
      mParticleEmitter(nullptr),
	  templateName("")
{

}

ParticleSystem::~ParticleSystem()
{
	if (mParticleSystem)
	{
		mParticleSystem->detachFromParent();
		getGameObject()->getParentScene()->getOgreScene()->destroyParticleSystem(mParticleSystem);
		mParticleSystem = nullptr;
	}
}

void ParticleSystem::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void ParticleSystem::init(const std::string& name)
{
	if (!mParticleSystem)
	{
		if (templateName != "")
		{
			mParticleSystem = getGameObject()->getParentScene()->getOgreScene()->createParticleSystem(name + "_PS", templateName);

			mParticleEmitter = mParticleSystem->getEmitter(0);

			if (mParticleEmitter)
			{
				unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
				setLayer(layer);
			}

			if (transform())
			{
				transform()->_attachObject(mParticleSystem);

				initialized = true;

				if (mParticleSystemDescMap.size())
					applyDescription(mParticleSystemDescMap);
			}
		}
	}
}

void ParticleSystem::applyDescription(ComponentDescMap& mComponentDescMap)
{
	if (mComponentDescMap.count(PSD_Scale))
	{
		setScale(Convert::parseInt(mComponentDescMap[PSD_Scale]));
		mComponentDescMap.erase(PSD_Scale);
	}

	Component::applyDescription(mComponentDescMap);
}

void ParticleSystem::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case PSD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case PSD_EnableEmission:
		setEnabled(Convert::parseBool(value));
		break;

	case PSD_Angle:
		setAngle(Convert::parseReal(value));
		break;

	case PSD_StartColor:
		setStartColor(Convert::parseColor(value));
		break;

	case PSD_MinColor:
		setMinColor(Convert::parseColor(value));
		break;

	case PSD_MaxColor:
		setMaxColor(Convert::parseColor(value));
		break;

	case PSD_Duration:
		setDuration(Convert::parseReal(value));
		break;

	case PSD_DurationMax:
		setDurationMax(Convert::parseReal(value));
		break;

	case PSD_EmissionRate:
		setEmissionRate(Convert::parseReal(value));
		break;

	case PSD_StartLifeTime:
		setTimeToLive(Convert::parseBool(value));
		break;

	case PSD_EndLifeTime:
		setMaxTimeToLive(Convert::parseBool(value));
		break;

	case PSD_Material:
		setMaterialName(value);
		break;

	case PSD_SpeedFactor:
		setSpeedFactor(Convert::parseReal(value));
		break;

	case PSD_Width:
		setWidth(Convert::parseReal(value));
		break;

	case PSD_Height:
		setHeight(Convert::parseReal(value));
		break;
	
	case PSD_QueryFlags:
		setLayer(Convert::parseUnsignedInt(value));
		break;

	default:
		break;
	}
}

void ParticleSystem::setScale(float scale)
{
	if (initialized)
	{
		if( mParticleSystem != 0 && ( scale < 0.999 || scale > 1.001 ) )
		{
			scaleBy( scale );
		}
	}
	else
	{
		mParticleSystemDescMap[PSD_Scale] = Convert::toString(scale);
	}
}

void ParticleSystem::scaleBy( Ogre::Real factor )
{
	Ogre::Real temp;
	temp = mParticleSystem->getDefaultWidth();
	temp *= factor;
	mParticleSystem->setDefaultWidth( temp );
	temp = mParticleSystem->getDefaultHeight();
	temp *= factor;
	mParticleSystem->setDefaultHeight( temp );

	emitter_params_t tempEP;
	for(unsigned short i=0; i<mParticleSystem->getNumEmitters(); ++i)
	{
		Ogre::ParticleEmitter* emitt = mParticleSystem->getEmitter(i);
		tempEP.pos = emitt->getPosition();
		tempEP.ParticleVelocity = emitt->getParticleVelocity();
		tempEP.MinParticleVelocity = emitt->getMinParticleVelocity();
		tempEP.MaxParticleVelocity = emitt->getMaxParticleVelocity();
		tempEP *= factor;
		emitt->setPosition(tempEP.pos);
		emitt->setParticleVelocity(tempEP.ParticleVelocity);
		emitt->setMinParticleVelocity(tempEP.MinParticleVelocity);
		emitt->setMaxParticleVelocity(tempEP.MaxParticleVelocity);
	}
}

void ParticleSystem::setEnabled(bool enable)
{
	if (initialized)
	{
		mParticleEmitter->setEnabled(enable);
		mParticleSystem->setVisible(enable);
	}
	else
	{
		mParticleSystemDescMap[PSD_Enabled] = Convert::toString(enable);
	}
}

bool ParticleSystem::isEnabled()
{
	if (initialized)
	{
		return mParticleSystem->getVisible();
	}
	else
	{
		if (mParticleSystemDescMap.count(PSD_Enabled))
		{
			return Convert::parseBool(mParticleSystemDescMap[PSD_Enabled]);
		}

		return true;
	}
}

void ParticleSystem::setEnableEmission(const bool enable)
{
	if (initialized)
	{
		mParticleSystem->setEmitting(enable);
	}
	else
	{
		mParticleSystemDescMap[PSD_EnableEmission] = Convert::toString(enable);
	}
}

//Configura o mâximo ângulo fora que a partícula possa tomar na direção.
void ParticleSystem::setAngle(double angle)
{
	if (initialized)
	{
		mParticleEmitter->setAngle(Ogre::Radian(angle));
	}
	else
	{
		mParticleSystemDescMap[PSD_Angle] = Convert::toString(angle);
	}
}

//Configura a cor da partícula
void ParticleSystem::setStartColor(double r,double g, double b)
{
	if (initialized)
	{
		mParticleEmitter->setColour(Ogre::ColourValue(r,g,b));
	}
	else
	{
		mParticleSystemDescMap[PSD_StartColor] = Convert::toString(Color(r, g, b));
	}
}
void ParticleSystem::setStartColor(const Color& colour)
{
	if (initialized)
	{
		mParticleEmitter->setColour(Convert::toOgre(colour));
	}
	else
	{
		mParticleSystemDescMap[PSD_StartColor] = Convert::toString(colour);
	}
}

//Configura a cor inicial e final da partícula, onde haverá uma
// interpolação de cor.
void ParticleSystem::setRangeColor(double rS,double gS, double bS,
                               double rE, double gE, double bE)
{
	if (initialized)
	{
		mParticleEmitter->setColour(Ogre::ColourValue(rS,gS,bS),
									Ogre::ColourValue(rE, gE, bE));
	}
	else
	{
		mParticleSystemDescMap[PSD_MinColor] = Convert::toString(Color(rS, gS, bS));
		mParticleSystemDescMap[PSD_MaxColor] = Convert::toString(Color(rE, gE, bE));
	}
}
void ParticleSystem::setRangeColor(const Color& colourStart,
                               const Color& colourEnd)
{
	if (initialized)
	{
		mParticleEmitter->setColour(Convert::toOgre(colourStart), Convert::toOgre(colourEnd));
	}
	else
	{
		mParticleSystemDescMap[PSD_MinColor] = Convert::toString(colourStart);
		mParticleSystemDescMap[PSD_MaxColor] = Convert::toString(colourEnd);
	}
}

//Configura a cor final que a particula pode se tornar.
void ParticleSystem::setMaxColor(double r,double g, double b)
{
	if (initialized)
	{
		mParticleEmitter->setColourRangeEnd(Ogre::ColourValue(r,g,b));
	}
	else
	{
		mParticleSystemDescMap[PSD_MaxColor] = Convert::toString(Color(r, g, b));
	}
}
void ParticleSystem::setMaxColor(const Color& colourEnd)
{
	if (initialized)
	{
		mParticleEmitter->setColourRangeEnd(Convert::toOgre(colourEnd));
	}
	else
	{
		mParticleSystemDescMap[PSD_MaxColor] = Convert::toString(colourEnd);
	}
}

//Configura a cor inicial que a particula pode se tornar.
void ParticleSystem::setMinColor(double r,double g, double b)
{
	if (initialized)
	{
		mParticleEmitter->setColourRangeStart(Ogre::ColourValue(r,g,b));
	}
	else
	{
		mParticleSystemDescMap[PSD_MinColor] = Convert::toString(Color(r, g, b));
	}
}
void ParticleSystem::setMinColor(const Color& colourStart)
{
	if (initialized)
	{
		mParticleEmitter->setColourRangeStart(Convert::toOgre(colourStart));
	}
	else
	{
		mParticleSystemDescMap[PSD_MinColor] = Convert::toString(colourStart);
	}
}

//Configura a duração que será emitido as partículas
void ParticleSystem::setDuration(double duration)
{
	if (initialized)
	{
		mParticleEmitter->setDuration(duration);
	}
	else
	{
		mParticleSystemDescMap[PSD_Duration] = Convert::toString(duration);
	}
}
void ParticleSystem::setDuration(double min, double max)
{
	if (initialized)
	{
		mParticleEmitter->setDuration(min, max);
	}
	else
	{
		mParticleSystemDescMap[PSD_Duration] = Convert::toString(min);
		mParticleSystemDescMap[PSD_DurationMax] = Convert::toString(max);
	}
}

void ParticleSystem::setDurationMax(double max)
{
	if (initialized)
	{
		mParticleEmitter->setMaxDuration(max);
	}
	else
	{
		mParticleSystemDescMap[PSD_DurationMax] = Convert::toString(max);
	}
}

//Configura a quantidade de partícular que será emitido
void ParticleSystem::setEmissionRate(double rate)
{
	if (initialized)
	{
		mParticleEmitter->setEmissionRate(rate);
	}
	else
	{
		mParticleSystemDescMap[PSD_EmissionRate] = Convert::toString(rate);
	}
}

double ParticleSystem::getEmissionRate()
{
    return (double)mParticleEmitter->getEmissionRate();
}

//Configura o tempo que as partículas vivem.
void ParticleSystem::setTimeToLive(double Ttl)
{
	if (initialized)
	{
		mParticleEmitter->setTimeToLive(Ttl);
	}
	else
	{
		mParticleSystemDescMap[PSD_StartLifeTime] = Convert::toString(Ttl);
	}
}
void ParticleSystem::setTimeToLive(double minTtl, double maxTtl)
{
	if (initialized)
	{
		mParticleEmitter->setTimeToLive(minTtl, maxTtl);
	}
	else
	{
		mParticleSystemDescMap[PSD_StartLifeTime] = Convert::toString(minTtl);
		mParticleSystemDescMap[PSD_EndLifeTime] = Convert::toString(maxTtl);
	}
}

void ParticleSystem::setMaxTimeToLive(double maxTtl)
{
	if (initialized)
	{
		mParticleEmitter->setMaxTimeToLive(maxTtl);
	}
	else
	{
		mParticleSystemDescMap[PSD_EndLifeTime] = Convert::toString(maxTtl);
	}
}

//Configura um material para a partícula
void ParticleSystem::setMaterialName(const std::string &material)
{
	if (initialized)
	{
		mParticleSystem->setMaterialName(material);
	}
	else
	{
		mParticleSystemDescMap[PSD_Material] = material;
	}
}

//Configura se partículas farão sombra
void ParticleSystem::setCastShadows(bool shadows)
{
	if (initialized)
	{
		mParticleSystem->setCastShadows(shadows);
	}
	else
	{
		mParticleSystemDescMap[PSD_CastShadows] = Convert::toString(shadows);
	}
}

//Retorna se partículas farão sombra
bool ParticleSystem::getCastShadows()
{
    return mParticleSystem->getCastShadows();
}

void ParticleSystem::setSpeedFactor(double speed)
{
	if (initialized)
	{
		mParticleSystem->setSpeedFactor(speed);
	}
	else
	{
		mParticleSystemDescMap[PSD_SpeedFactor] = Convert::toString(speed);
	}
}

double ParticleSystem::getSpeedFactor()
{
    return (double)mParticleSystem->getSpeedFactor();
}

void ParticleSystem::clear()
{
	if (initialized)
	{
		mParticleSystem->clear();
	}
}

void ParticleSystem::setDimensions(float width, float height)
{
	if (initialized)
	{
		mParticleSystem->setDefaultDimensions(width, height);
	}
	else
	{
		mParticleSystemDescMap[PSD_Width] = Convert::toString(width);
		mParticleSystemDescMap[PSD_Height] = Convert::toString(height);
	}
}

void ParticleSystem::setWidth(float width)
{
	if (initialized)
	{
		mParticleSystem->setDefaultWidth(width);
	}
	else
	{
		mParticleSystemDescMap[PSD_Width] = Convert::toString(width);
	}
}

void ParticleSystem::setHeight(float height)
{
	if (initialized)
	{
		mParticleSystem->setDefaultHeight(height);
	}
	else
	{
		mParticleSystemDescMap[PSD_Height] = Convert::toString(height);
	}
}

void ParticleSystem::setLayer(unsigned flags)
{
	if (initialized)
	{
		mParticleSystem->setQueryFlags(flags);
	}
	else
	{
		mParticleSystemDescMap[PSD_QueryFlags] = Convert::toString(flags);
	}
}

unsigned ParticleSystem::getLayer()
{
	if (initialized)
	{
		return mParticleSystem->getQueryFlags();
	}

	return 1;
}