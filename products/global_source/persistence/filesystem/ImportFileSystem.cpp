///////////////////////////////////////////////////////////
//  ImportFileSystem.cpp
//  Implementation of the Class ImportFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#include "ImportFileSystem.h"

//-----------------------------------------------------------------------------
ImportFileSystem::ImportFileSystem()
{	
	timeSleep = 500;
	activeThread = false;
}

//-----------------------------------------------------------------------------
ImportFileSystem::~ImportFileSystem()
{
	if(activeThread)
	{
		QThread::terminate();
	}
}

//-----------------------------------------------------------------------------
ImportFileSystem::ImportFileSystem(const ImportFileSystem& v)
{
	activeThread = v.activeThread;
	listFile = v.listFile;
	timeSleep = v.timeSleep;
}

//-----------------------------------------------------------------------------
ImportFileSystem& ImportFileSystem::operator = (const ImportFileSystem& v)
{
	if (&v == this)
        return *this; 
	activeThread = v.activeThread;
	listFile = v.listFile;
	timeSleep = v.timeSleep;
	return *this;
}

//-----------------------------------------------------------------------------
void ImportFileSystem::importData(const QStringList& files)
{	
	if(activeThread)
	{
		listFile = files;
	}
	else
	{
		foreach(QString file, files)
		{		
			importData(file); 
		}
	}
}

//-----------------------------------------------------------------------------
void ImportFileSystem::importData(const QString& name)
{
	if(activeThread)
	{
		listFile << name;
	}
	else
	{
		read(name);
	}
}

//-----------------------------------------------------------------------------
void ImportFileSystem::setActiveThread(bool v)
{
	activeThread = v;
}

//-----------------------------------------------------------------------------
bool ImportFileSystem::isActiveThread() const
{
	return activeThread;
}

//-----------------------------------------------------------------------------
void ImportFileSystem::start()
{
	if(activeThread)
	{
		QThread::start();
	}
}

//-----------------------------------------------------------------------------
void ImportFileSystem::stop()
{
	if(activeThread)
	{
		QThread::terminate();
	}
}

//-----------------------------------------------------------------------------
void ImportFileSystem::setTimeSleep(int v)
{
	timeSleep = v;
}

//-----------------------------------------------------------------------------
int ImportFileSystem::getTimeSleep() const
{
	return timeSleep;
}

//-----------------------------------------------------------------------------
void ImportFileSystem::run()
{
	if(!activeThread) return;
	
	while(!listFile.empty())
	{
		QString &str = listFile.first();
		read(str);
		listFile.pop_front();
	}
}

