#include "SurfaceEvents.h"
#include "SurfaceListerner.h"

unsigned int SurfaceEvents::count = 0;

//-----------------------------------------------------------------------------
SurfaceEvents::SurfaceEvents()
{
	count++;
	setId(count);

	SurfaceListerner * tl = SurfaceListerner::getInstance();
	tl->addEvent(this);
}

//-----------------------------------------------------------------------------
SurfaceEvents::SurfaceEvents(const SurfaceEvents& v)
{
	id = v.id;
}

//-----------------------------------------------------------------------------
SurfaceEvents& SurfaceEvents::operator = (const SurfaceEvents& v)
{
	if (&v == this)
        return *this; 
	id = v.id;
	return *this;
}

//-----------------------------------------------------------------------------
SurfaceEvents::~SurfaceEvents()
{
	SurfaceListerner * tl = SurfaceListerner::getInstance();
	tl->removeEvent(this);
}

//-----------------------------------------------------------------------------
void SurfaceEvents::changedLoad()
{
}

//-----------------------------------------------------------------------------
void SurfaceEvents::changedListName()
{
}

//-----------------------------------------------------------------------------
void SurfaceEvents::changedPath()
{
}

//-----------------------------------------------------------------------------
void SurfaceEvents::changedListPath()
{
}

//-----------------------------------------------------------------------------
void SurfaceEvents::changedPainter()
{
}

//-----------------------------------------------------------------------------
unsigned int SurfaceEvents::getId() const
{
	return id;
}

//-----------------------------------------------------------------------------
void SurfaceEvents::setId(unsigned int i)
{
	id = i;
}
