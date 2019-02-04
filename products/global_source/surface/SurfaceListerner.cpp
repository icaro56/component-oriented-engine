#include "SurfaceListerner.h"

SurfaceListerner* SurfaceListerner::instance = 0;

//-----------------------------------------------------------------------------
SurfaceListerner::SurfaceListerner()
{

}

//-----------------------------------------------------------------------------
SurfaceListerner::~SurfaceListerner()
{
	list.clear();
}	

//-----------------------------------------------------------------------------
SurfaceListerner* SurfaceListerner::getInstance()
{
	if(!instance)
	{
		instance = new SurfaceListerner();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void SurfaceListerner::destroy()
{
	delete instance;
	instance = 0;
}

//-----------------------------------------------------------------------------
void SurfaceListerner::init()
{
	//falta
}

//-----------------------------------------------------------------------------
void SurfaceListerner::addEvent(SurfaceEvents* te)
{
	unsigned int k = te->getId();
	if(!list.contains(k))
	{
		list.insert(k,te);
	}
}

//-----------------------------------------------------------------------------
void SurfaceListerner::removeEvent(SurfaceEvents* te)
{
	unsigned int k = te->getId();
	if(list.contains(k))
	{
		list.remove(k);
	}
}


//-----------------------------------------------------------------------------
void SurfaceListerner::notify(TypeSurfaceEvent type)
{
	QHash<unsigned int ,SurfaceEvents*>::iterator it;
	for(it=list.begin();it!=list.end();it++)
	{
		SurfaceEvents* ev = it.value();
		switch(type)
		{
		case TypeSurfaceEvent::ChangedLoad:
			{
				ev->changedLoad();
			}
			break;
			case TypeSurfaceEvent::ChangedListName:
			{
				ev->changedListName();
			}
			break;
			case TypeSurfaceEvent::ChangedPath:
			{
				ev->changedPath();
			}
			break;
			case TypeSurfaceEvent::ChangedListPath:
			{
				ev->changedListPath();
			}
			break;
			case TypeSurfaceEvent::ChangedPainter:
			{
				ev->changedPainter();
			}
			break;
			case TypeSurfaceEvent::None:
			{
			}
			break;
		}	
	}
}

