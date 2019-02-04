#include "ReaderLog.h"
//#include "moc_ReaderLog.cpp"

#include <QReadWriteLock>

#include "LogManager.h"
#include "EventLog.h"

//-----------------------------------------------------------------------------
ReaderLog::ReaderLog(const QString &d, const QString &p, const QString &da,
		 const QString &e, const QString &s)
{
	append = true;
	dir = d;
	prefix = p;
	dateformat = da;
	extension = e;
	extension = extension.remove(".");
	sep = s;
	LogManager* lm = LogManager::getInstance();
	lm->registerReader(this);
	id = lm->getNumReaders();
	active = true;	

	connect(&watcher, SIGNAL(fileChanged(const QString&)),
            this, SLOT(fileChanged(const QString&)));

	watcher.addPath(getName());
	QThread::start();
}

//-----------------------------------------------------------------------------
ReaderLog::~ReaderLog()
{	
	clearWatcher();
	QThread::quit();
	LogManager* lm = LogManager::getInstance();
	lm->removeReader(id);
}

//-----------------------------------------------------------------------------
void ReaderLog::setActive(bool v)
{
	active = v;
}

//-----------------------------------------------------------------------------
bool ReaderLog::isActive() const
{
	return active;
}

//-----------------------------------------------------------------------------
int ReaderLog::getId() const
{
	return id;
}

//-----------------------------------------------------------------------------
void ReaderLog::setDir(const QString& v)
{
	clearWatcher();
	dir = v;
	QThread::quit();
	file.close();
	watcher.addPath(getName());
}

//-----------------------------------------------------------------------------
QString ReaderLog::getDir() const
{
	return dir;
}

//-----------------------------------------------------------------------------
void ReaderLog::setPrefix(const QString& v)
{
	clearWatcher();
	prefix = v;
	QThread::quit();
	file.close();
	watcher.addPath(getName());
}

//-----------------------------------------------------------------------------
QString ReaderLog::getPrefix() const
{
	return prefix;
}

//-----------------------------------------------------------------------------
void ReaderLog::setExtension(const QString& v)
{
	clearWatcher();
	extension = v;
	extension = extension.remove(".");
	QThread::quit();
	file.close();
	watcher.addPath(getName());
}

//-----------------------------------------------------------------------------
QString ReaderLog::getExtension() const
{
	return extension;
}

//-----------------------------------------------------------------------------
void ReaderLog::setDateFormat(const QString& v)
{
	clearWatcher();
	dateformat = v;
	QThread::quit();
	file.close();
	watcher.addPath(getName());
}

//-----------------------------------------------------------------------------
QString ReaderLog::getDateFormat() const
{
	return dateformat;
}

//-----------------------------------------------------------------------------
QString ReaderLog::getName() const
{
	/*
	QDateTime dt = QDateTime::currentDateTime();
	QString turn = ReadShiftFile::specTurnFile.getTurn(dt);
	QString time = dt.toString(dateformat);
	return dir + "\\" + prefix + time + turn + "." + extension;*/
	return "falta";
}

//-----------------------------------------------------------------------------
void ReaderLog::setSeparator(const QString& v)
{
	sep = v;
}

//-----------------------------------------------------------------------------
QString ReaderLog::getSeparator() const
{
	return sep;
}


//-----------------------------------------------------------------------------
void ReaderLog::swapTurn()
{
	clearWatcher();
	QThread::quit();
	file.close();	
	watcher.addPath(getName());
}

//-----------------------------------------------------------------------------
void ReaderLog::fileChanged(const QString&)
{
	if(active)
	{
		if(QThread::isFinished())
		{
			QThread::start();
		}
	}
}

//-----------------------------------------------------------------------------
void ReaderLog::setAppend(bool v)
{
	append = v;
}

//-----------------------------------------------------------------------------
bool ReaderLog::isAppend() const
{
	return append;
}

//-----------------------------------------------------------------------------
void ReaderLog::readerAppend()
{
	if(!file.isOpen())
	{		
		file.setFileName(getName());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		file.atEnd();

		QString line;
		QHash<QString,QString> list;
		while(!file.atEnd())
		{
			line = file.readLine();			
			list.insert(line.split(sep)[1],line);
		}

		foreach(QString str,list)
		{
			str = str.trimmed();
			read(str.split(sep));
		}
	}
	
	QString line;

	while(!file.atEnd())
	{
		line = file.readLine();
		line = line.trimmed();
		read(line.split(sep));
	}	
}

//-----------------------------------------------------------------------------
void ReaderLog::readerWriteOnly()
{
	file.setFileName(getName());
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	
	QString line;
	QStringList list;
	while(!file.atEnd())
	{
		line = file.readLine();	
		list << line.trimmed();
	}
	
	read(list);
	
	file.close();
}

//-----------------------------------------------------------------------------
void ReaderLog::run()
{	
	QReadWriteLock lock;
    lock.lockForWrite();
	if(isAppend())
	{
		readerAppend();
	}
	else
	{
		readerWriteOnly();
	}
	lock.unlock();
}

//-----------------------------------------------------------------------------
void ReaderLog::reset(const QString &d, const QString &p, const QString &da,
		const QString &e, const QString &s)
{
	clearWatcher();
	QThread::quit();
	file.close();
	dir = d;
	prefix = p;
	dateformat = da;
	extension = e;
	extension = extension.remove(".");
	sep = s;
	watcher.addPath(getName());
	QThread::start();
}

//-----------------------------------------------------------------------------
void ReaderLog::clearWatcher()
{
	watcher.removePaths(watcher.files());	
}

//-----------------------------------------------------------------------------
void ReaderLog::read(const QStringList&)
{
}

