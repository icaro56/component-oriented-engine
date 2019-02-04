#include "ProcessorLog.h"
#include "LogManager.h"
#include "EventLog.h"

//-----------------------------------------------------------------------------
ProcessorLog::ProcessorLog() 
{
	LogManager* lm = LogManager::getInstance();
	lm->registerProcessor(this);
	id = lm->getNumProcessors();
	active = true;
}

//-----------------------------------------------------------------------------
ProcessorLog::~ProcessorLog() 
{
	LogManager* lm = LogManager::getInstance();
	lm->removeProcessor(id);
}

//-----------------------------------------------------------------------------
void ProcessorLog::receives(EventLog& ev)
{
	ev.setProcessor(this);
	LogManager* lm = LogManager::getInstance();
	lm->registerEvent(ev);
}

//-----------------------------------------------------------------------------
int ProcessorLog::getId() const
{
	return id;
}

//-----------------------------------------------------------------------------
QString ProcessorLog::getDir() const
{
	return dir;
}

//-----------------------------------------------------------------------------
void ProcessorLog::setDir(const QString& v)
{
	dir = v;
}

//-----------------------------------------------------------------------------
void ProcessorLog::setActive(bool v)
{
	active = v;
}

//-----------------------------------------------------------------------------
bool ProcessorLog::isActive() const
{
	return active;
}
