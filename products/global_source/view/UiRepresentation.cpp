#include "UiRepresentation.h"
#include "UiRepresentationManager.h"

#include <QMenu>


//-----------------------------------------------------------------------------
UiRepresentation::UiRepresentation(unsigned int id):
type(id),
isActive(true)
{
	UiRepresentationManager::getInstance()->addUi(this);
}

//-----------------------------------------------------------------------------
UiRepresentation::~UiRepresentation()
{
	UiRepresentationManager::getInstance()->removeUi(this);
}

//-----------------------------------------------------------------------------
unsigned int UiRepresentation::getType() const
{
	return type;
}

//-----------------------------------------------------------------------------
void UiRepresentation::setEditedRepresentation(bool cond)
{	
	UiRepresentationManager::getInstance()->setEdition(this,cond);
}

//-----------------------------------------------------------------------------
void UiRepresentation::setActiveRepresentation(bool a)
{
	isActive = a;
}

bool UiRepresentation::isActiveRepresentation()
{
	return isActive;
}






