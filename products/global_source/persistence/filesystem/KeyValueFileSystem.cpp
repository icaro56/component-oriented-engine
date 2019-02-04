///////////////////////////////////////////////////////////
//  KeyValueFileSystem.cpp
//  Implementation of the Class KeyValueFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#include "KeyValueFileSystem.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

//-----------------------------------------------------------------------------
KeyValueFileSystem::KeyValueFileSystem(const QString& fn, const QString& s)
{	
	filename = fn;
	separator = s;
}

//-----------------------------------------------------------------------------
KeyValueFileSystem::~KeyValueFileSystem()
{
}

//-----------------------------------------------------------------------------
KeyValueFileSystem::KeyValueFileSystem(const KeyValueFileSystem& v)
{
	filename = v.filename;
	separator = v.separator;
	attributes = v.attributes;
}

//-----------------------------------------------------------------------------
KeyValueFileSystem& KeyValueFileSystem::operator = (const KeyValueFileSystem& v)
{
	if (&v == this)
        return *this; 

	filename = v.filename;
	separator = v.separator;
	attributes = v.attributes;

	return *this; 
}

//-----------------------------------------------------------------------------
void KeyValueFileSystem::setFileName(const QString& v)
{
	filename = v;
}

//-----------------------------------------------------------------------------
void KeyValueFileSystem::setSeparator(const QString& v)
{
	separator = v;
}

//-----------------------------------------------------------------------------
bool KeyValueFileSystem::read()
{
    QFile file(filename);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

    QTextStream in(&file);

	bool cond = false;
    while (!in.atEnd())
    {
        QStringList list = in.readLine().split(separator);
		if(list.size() == 2)
		{
			attributes.insert(list.at(0), list.at(1));
		}
		else
		{
			cond = true;
		}
    }

	return cond;
}

//-----------------------------------------------------------------------------
void KeyValueFileSystem::write() const
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    QStringList keys = attributes.keys();
    for (int i = 0; i < keys.size(); i++ )
        out << keys.at(i) << separator << attributes.value(keys.at(i)) << "\n";
}

//-----------------------------------------------------------------------------
void KeyValueFileSystem::setAttribute(const QString & key, const QString & value)
{
    attributes.insert(key, value);
}

//-----------------------------------------------------------------------------
const QString KeyValueFileSystem::getAttribute(const QString & key) const
{
    return attributes.value(key);
}