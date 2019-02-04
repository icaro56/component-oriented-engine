///////////////////////////////////////////////////////////
//  Time.cpp
//  Implementation of the Class Time
//  Created on:      23-jun-2012 06:38:22
//  Original author: Leandro
///////////////////////////////////////////////////////////

#include "time/Time.h"

#include "engine/RenderView.h"
#include "engine/Engine.h"

#include <OgreTimer.h>

TimeClass* TimeClass::instance = 0;

//-----------------------------------------------------------------------------
TimeClass::TimeClass()
	: dt(0),
	  time(0),
	  mTimer(0),
	  fixedDeltaTime(1.0 / 60.0),
	  fixedTime(0),
	  timeScale(1.0),
	  realtimeSinceStartup(0)
{
	
}

//-----------------------------------------------------------------------------
void TimeClass::init(Ogre::Timer* tm)
{
	mTimer = tm;
	lastTime = mTimer->getMilliseconds();
	lastTimeReal = lastTime;

	//iniciando current time
	setTime(QDateTime::currentDateTime().toTime_t());

	//iniciando tempo real que não leva em conta TimeScale
	setRealtimeSinceStartup(time);
}

//-----------------------------------------------------------------------------
TimeClass::~TimeClass()
{	
}

//-----------------------------------------------------------------------------
TimeClass* TimeClass::getInstance()
{
	if(!instance)
	{
		instance = new TimeClass();
	}
	return instance;
}

//-----------------------------------------------------------------------------
double TimeClass::getTime() const
{	
	return time;
}

//-----------------------------------------------------------------------------
QDateTime TimeClass::getDateTime() const
{
	return QDateTime::fromTime_t(time);
}

//-----------------------------------------------------------------------------
void TimeClass::setTime(double v)
{
	time = v;
}

//-----------------------------------------------------------------------------
void TimeClass::setTime(const QDateTime& v)
{
	time = v.toTime_t();
}

double TimeClass::getRealtimeSinceStartup() const
{
	return realtimeSinceStartup;
}

QDateTime TimeClass::getDateRealtimeSinceStartup() const
{
	return QDateTime::fromTime_t(realtimeSinceStartup);
}

void TimeClass::setRealtimeSinceStartup(double v)
{
	realtimeSinceStartup = v;
}

void TimeClass::setRealtimeSinceStartup(const QDateTime& v)
{
	realtimeSinceStartup = v.toTime_t();
}

//-----------------------------------------------------------------------------
double TimeClass::getDeltaTime() const
{
	return dt;
}

//-----------------------------------------------------------------------------
void TimeClass::setDeltaTime(const double& v)
{
	dt = v;
}


//-----------------------------------------------------------------------------
void TimeClass::update()
{
	unsigned long now = getMilliseconds();
    dt = (now - lastTime)*0.001;
    lastTime = now;

	time += dt;


	unsigned long now2 = mTimer->getMilliseconds();
	double dt2 = (now2 - lastTimeReal) * 0.001;
	lastTimeReal = now;
	realtimeSinceStartup += dt2;
}

double TimeClass::getFixedTime()
{
	return fixedTime;
}

void TimeClass::setFixedTime(double ftime)
{
	fixedTime = ftime;
}

//-----------------------------------------------------------------------------
unsigned long TimeClass::getMilliseconds()
{
    return mTimer->getMilliseconds() * timeScale;
}

//-----------------------------------------------------------------------------
unsigned long TimeClass::getMicroseconds()
{
    return mTimer->getMicroseconds() * timeScale;
}

double TimeClass::getFixedDeltaTime() const
{
	return fixedDeltaTime;
}

void TimeClass::setFixedDeltaTime(const double& fDt)
{
	fixedDeltaTime = fDt;
}

void TimeClass::updateFixedTime()
{
	fixedTime += fixedDeltaTime;
}

void TimeClass::setTimeScale(double scale)
{
	timeScale = scale;
}

double TimeClass::getTimeScale()
{
	return timeScale;
}
