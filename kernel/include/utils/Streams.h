/*
This source file is part of OgrePhysX.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once
#include "KernelPrerequisites.h"
#include <OgreDataStream.h>

#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include "common/PxIO.h" 
#include <streambuf>
#include "OgreString.h"

class KERNEL_VS_EXPORT OgrePxInputStream : public physx::PxInputStream
{
private:
	Ogre::DataStreamPtr mOgreDataStream;

public:
	OgrePxInputStream(Ogre::DataStreamPtr ds);
	~OgrePxInputStream();

	virtual physx::PxU32 read(void* dest, physx::PxU32 count);

	Ogre::DataStreamPtr getOgreDataStream();
};

class KERNEL_VS_EXPORT OgrePxOutputStream : public physx::PxOutputStream
{
private:
	Ogre::DataStreamPtr mOgreDataStream;

public:
	OgrePxOutputStream(Ogre::DataStreamPtr ds);
	~OgrePxOutputStream();

	virtual physx::PxU32 write(const void* src, physx::PxU32 count);

	Ogre::DataStreamPtr getOgreDataStream();
};