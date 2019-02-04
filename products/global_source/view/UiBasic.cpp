#include "UiBasic.h"
#include "UiBasicManager.h"
#include <QAction>
#include <QMenu>

//-----------------------------------------------------------------------------
UiBasic::UiBasic(unsigned int id):
type(id)
{
	UiBasicManager::getInstance()->addUi(this);
}

//-----------------------------------------------------------------------------
UiBasic::~UiBasic()
{
	UiBasicManager::getInstance()->removeUi(this);
}

//-----------------------------------------------------------------------------
unsigned int UiBasic::getType() const
{
	return type;
}

//-----------------------------------------------------------------------------
void UiBasic::setFunctionalityEnabledUi(unsigned int,bool)
{

}

//-----------------------------------------------------------------------------
void UiBasic::setFunctionalityCheckedUi(unsigned int,bool)
{

}

//-----------------------------------------------------------------------------
void UiBasic::setFunctionalityVisibleUi(unsigned int,bool)
{

}

//-----------------------------------------------------------------------------
void UiBasic::addActionsInMenu(QMenu*)
{
}

//-----------------------------------------------------------------------------
void UiBasic::addActionsInToolbar(QToolBar*)
{
}

//-----------------------------------------------------------------------------
void UiBasic::addDockWidget(QMainWindow*)
{
}



