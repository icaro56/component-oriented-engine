#include "UiUserLog.h"

#include "system/ProjectConfig.h"
#include "log/EventLog.h"

UiUserLog * UiUserLog::instance = 0;

const QString UiUserLog::Type = "Tipo";
const QString UiUserLog::Id = "Id";
const QString UiUserLog::CurrentState = "CurrentState";
const QString UiUserLog::LastState = "LastState";
const QString UiUserLog::TimeLog = "Time";

//-----------------------------------------------------------------------------
UiUserLog::UiUserLog()
{		
	init();
}

//-----------------------------------------------------------------------------
UiUserLog::~UiUserLog()
{
	file.close();
}

//-----------------------------------------------------------------------------
UiUserLog * UiUserLog::getInstance()
{
	if(!instance)
	{
		instance = new UiUserLog();
	}
	return instance;
}

//-----------------------------------------------------------------------------
UiUserLog::UiUserLog(const UiUserLog& v)
{		
}

//-----------------------------------------------------------------------------
UiUserLog& UiUserLog::operator = (const UiUserLog& v)
{
	if (&v == this)
        return *this; 	 
	return *this;
}

//-----------------------------------------------------------------------------
void UiUserLog::init()
{
	bool cond = name();

	if (file.isOpen()) file.close();	

    bool r = (file.open(QIODevice::Append | QIODevice::Text));
    if(!r)
    {
        file.setFileName("");
    }

	if(!cond)
		head();
}

//-----------------------------------------------------------------------------
void UiUserLog::reset()
{
}


//-----------------------------------------------------------------------------
bool UiUserLog::name()
{
	//falta
	/*
	bool cond = true;
	QDateTime time = QDateTime::currentDateTime();
    QString path = Globals::SIGNAL_LOG_DIR + "/";
	QString name = "log";
    name += time.toString("yyMMdd");
	name += ReadShiftFile::getTurn(time);
    name += ".txt";
    QDir dir;
    if(!dir.exists(path + name))
    {
        dir.mkdir(Globals::DATAPATH);
        dir.mkdir(Globals::SIGNAL_LOG_DIR);
		cond = false;
    }
	file.setFileName(path + name);
	return cond;
	*/
	return "falta";
}

//-----------------------------------------------------------------------------
void UiUserLog::head()
{
}

//-----------------------------------------------------------------------------
void UiUserLog::write(const EventLog& ev)
{
	QString str;
	str += ev.getValueProperties(Type) + ";";
	str += ev.getValueProperties(Id) + ";";
	str += ev.getValueProperties(TimeLog) + ";";
	str += ev.getValueProperties(CurrentState) + ";";
	str += ev.getValueProperties(LastState) + ";";	
	str += "\n";
	file.write(str.toStdString().c_str());
    file.flush();
}

//-----------------------------------------------------------------------------
void UiUserLog::swapTurn()
{
	init();
}
