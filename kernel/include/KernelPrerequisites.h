#ifndef KERNELPREREQUISITES_H
#define KERNELPREREQUISITES_H

class Collider;

#ifdef KERNEL_VS_EXPORTS
		#define KERNEL_VS_EXPORT __declspec (dllexport)
#else
		#define KERNEL_VS_EXPORT __declspec (dllimport)
#endif

#include "OgrePrerequisites.h"
#include "OgreCommon.h"

#if OGRE_DOUBLE_PRECISION == 1
    /** Software floating point type.
    @note Not valid as a pointer to GPU buffers / parameters
    */
    typedef double Real;
	#define KERNEL_DOUBLE_PRECISION 1
#else
    /** Software floating point type.
    @note Not valid as a pointer to GPU buffers / parameters
    */
    typedef float Real;
	#define KERNEL_DOUBLE_PRECISION 0
#endif

	typedef std::vector<std::string> StringVector;
	typedef std::basic_stringstream<char,std::char_traits<char>,std::allocator<char> > StringStream;
	typedef std::vector<Collider*> ColliderVector;

	/**
	* @enum ShadowTechnique
	*/
	enum ShadowTechnique
	{
		SHADOW_NONE = Ogre::SHADOWTYPE_NONE,
		SHADOW_STENCIL = Ogre::SHADOWTYPE_STENCIL_MODULATIVE,
		SHADOW_STENCIL_ADDITIVE = Ogre::SHADOWTYPE_STENCIL_ADDITIVE,
		SHADOW_TEXTURE = Ogre::SHADOWTYPE_TEXTURE_MODULATIVE,
		SHADOW_TEXTURE_ADDITIVE = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE
	};

#endif