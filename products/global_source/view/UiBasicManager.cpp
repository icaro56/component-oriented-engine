#include "UiBasicManager.h"
#include "UiBasic.h"
#include "UiException.h"

#include <iostream>

#include <QObject>

UiBasicManager* UiBasicManager::instance = 0;

//-----------------------------------------------------------------------------
UiBasicManager::UiBasicManager()
{
}

//-----------------------------------------------------------------------------
UiBasicManager::~UiBasicManager()
{
}

//-----------------------------------------------------------------------------
UiBasicManager* UiBasicManager::getInstance()
{
	if (!instance)
	{
		instance = new UiBasicManager();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void UiBasicManager::destroy()
{
	listUi.clear();
}

//-----------------------------------------------------------------------------
void UiBasicManager::setEnableUi(unsigned int id,bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setEnabledUi(cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::setCheckedUi(unsigned int id,bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setCheckedUi(cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::setVisibleUi(unsigned int id,bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setVisibleUi(cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::setFunctionalityEnabledUi(unsigned int id,unsigned int idf,
															bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setFunctionalityEnabledUi(idf,cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::setFunctionalityCheckedUi(unsigned int id,unsigned int idf,
															bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setFunctionalityCheckedUi(idf,cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::setFunctionalityVisibleUi(unsigned int id,unsigned int idf,
															bool cond)
{
	if(listUi.contains(id))
	{
		listUi[id]->setFunctionalityVisibleUi(idf,cond);
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::addUi(UiBasic* v)
{
	if(!listUi.contains(v->getType()))
	{
		listUi.insert(v->getType(),v);
	}
	else
	{
		QString str = QObject::tr("Enumerador replicado.");
		std::cout << str.toStdString() << std::endl;
		UiException e;
		e.setMessage(str);
		throw e;
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::removeUi(UiBasic* v)
{
	if(listUi.contains(v->getType()))
	{
		listUi.remove(v->getType());
	}
}

//-----------------------------------------------------------------------------
void UiBasicManager::init()
{	
	foreach(UiBasic* v,listUi)
	{
		v->setCheckedUi(false);
	}

	setCheckedUi(Ui_CamFree,true);
	setEnableUi(Ui_CamOrbital,false);
	setCheckedUi(Ui_Minimap,true);
	setCheckedUi(Ui_Clock,true);
	setCheckedUi(Ui_Terrain,true);
	setCheckedUi(Ui_IndicatorAll,true);
	setCheckedUi(Ui_StatusBar,true);
}

//-----------------------------------------------------------------------------
QList<int> UiBasicManager::getUsedTypesUiBasics()
{
	QList<int> list;

	if (!listUi.empty())
	{
		list.append(listUi.keys());
	}

	return list;
}

//-----------------------------------------------------------------------------
void UiBasicManager::disableAllTools()
{
	foreach(UiBasic* ub, listUi)
	{
		ub->setEnabledUi(false);
	}
}