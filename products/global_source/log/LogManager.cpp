#include "LogManager.h"
#include "EventLog.h"
#include "ProcessorLog.h"
#include "ReaderLog.h"

LogManager* LogManager::instance = 0;

//-----------------------------------------------------------------------------
LogManager::LogManager()
{
	numProcessors = 0;
	numReaders = 0;
	canAdd = true;
	start();
}

//-----------------------------------------------------------------------------
LogManager::~LogManager()
{
	processors.clear();
	readers.clear();
	events.clear();
	terminate();
}

//-----------------------------------------------------------------------------
LogManager* LogManager::getInstance()
{
	if(!instance)
	{
		instance = new LogManager();
	}
	return instance;
}

//-----------------------------------------------------------------------------
int LogManager::getNumProcessors() const
{
	return numProcessors;
}

//-----------------------------------------------------------------------------
int LogManager::getNumReaders() const
{
	return numReaders;
}

//-----------------------------------------------------------------------------
void LogManager::registerEvent(const EventLog& ev)
{
	mutexEvent.lock();
	events.push_back(ev);
	mutexEvent.unlock();
	if(canAdd && isFinished())
	{
		start();
	}
}

//-----------------------------------------------------------------------------
void LogManager::registerProcessor(ProcessorLog* sa)
{
	numProcessors++;
	processors.insert(numProcessors,sa);
}

//-----------------------------------------------------------------------------
void LogManager::registerReader(ReaderLog* sa)
{
	numReaders++;
	readers.insert(numReaders,sa);
}

//-----------------------------------------------------------------------------
void LogManager::removeProcessor(int id)
{
	processors.remove(id);
}

//-----------------------------------------------------------------------------
void LogManager::removeReader(int id)
{
	readers.remove(id);
}

//-----------------------------------------------------------------------------
void LogManager::run()
{	
	mutexEvent.lock();
	while(!events.empty())
	{		
		EventLog &ev = events.front();		
		ev.getProcessor()->write(ev);		

		events.pop_front();		
	}	
	mutexEvent.unlock();
}

//-----------------------------------------------------------------------------
void LogManager::swapTurn()
{
	canAdd = false;

	if(!isFinished())
	{
		terminate();
	}
	
	events.clear();

	foreach(ProcessorLog* log,processors)
	{
		log->swapTurn();
	}

	foreach(ReaderLog* log, readers)
	{
		log->swapTurn();
	}

	canAdd = true;
}
