#include "EventLog.h"

//-----------------------------------------------------------------------------
EventLog::EventLog() 
{
	processor = 0;	
}

//-----------------------------------------------------------------------------
EventLog::~EventLog() 
{
}

//-----------------------------------------------------------------------------
ProcessorLog* EventLog::getProcessor() const
{
	return processor;
}

//-----------------------------------------------------------------------------	
void EventLog::setProcessor(ProcessorLog* value)
{
	processor = value;
}

//-----------------------------------------------------------------------------	
QString EventLog::getValueProperties(const QString& key) const
{
	return properties[key];
}

//-----------------------------------------------------------------------------	
void EventLog::setValueProperties(const QString& key,const QString& value)
{
	if(properties.contains(key))
	{
		properties[key] = value;
	}
	else
	{
		properties.insert(key,value);
	}
}
