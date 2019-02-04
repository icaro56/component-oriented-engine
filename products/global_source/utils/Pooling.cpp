///////////////////////////////////////////////////////////
//  Pooling.cpp
//  Implementation of the Class Pooling
//  Created on:      23-jun-2012 06:38:22
//  Original author: Leandro
///////////////////////////////////////////////////////////

#include "Pooling.h"
//#include "engine/RenderView.h"
//#include "engine/Engine.h"
#include <QDateTime>

//-----------------------------------------------------------------------------
Pooling::Pooling()
{	
	interval = 0;
	initialTime = 0;
	activated = false;
}

//-----------------------------------------------------------------------------
Pooling::~Pooling()
{	
}

//-----------------------------------------------------------------------------
Pooling::Pooling(const Pooling& v)
{
	activated = v.activated;
    interval = v.interval;
    initialTime = v.initialTime;
}

//-----------------------------------------------------------------------------
Pooling& Pooling::operator= (const Pooling& v)
{
	if (&v == this)
        return *this;
    activated = v.activated;
    interval = v.interval;
    initialTime = v.initialTime;	
	return *this;
}

//-----------------------------------------------------------------------------
int Pooling::getInterval() const
{	
	return interval;
}

//-----------------------------------------------------------------------------
void Pooling::setInterval(int v)
{	
	interval = v;
}

//-----------------------------------------------------------------------------
void Pooling::start()
{	
	activated = true;
	initialTime = QDateTime::currentDateTime().toTime_t();
}

//-----------------------------------------------------------------------------
void Pooling::stop()
{	
	activated = false;
}

//-----------------------------------------------------------------------------
void Pooling::exec()
{
	if(activated)
	{
		int t = QDateTime::currentDateTime().toTime_t();
		if(t-initialTime >= interval)
		{
			initialTime = t;
			pooling();
		}
	}
}

void Pooling::setInitialTime(int time)
{
	initialTime = time;
}

int Pooling::getInitialTime() const
{
	return initialTime;
}

void Pooling::setActivated(bool active)
{
	this->activated = active;
}

bool Pooling::getActivated() const
{
	return activated;
}

void Pooling::swap(Pooling& other)
{
	std::swap(initialTime, other.initialTime);
	std::swap(interval, other.interval);
	std::swap(activated, other.activated);
}
