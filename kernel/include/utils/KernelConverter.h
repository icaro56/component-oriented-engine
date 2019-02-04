#ifndef KERNEL_CONVERTER
#define KERNEL_CONVERTER

#include "foundation/PxQuat.h"
#include "foundation/PxMat44.h"
#include "foundation/PxMat33.h"
#include "foundation/PxVec3.h"
#include "foundation/PxBounds3.h"
#include "characterkinematic/PxExtended.h"

#include "OgreVector4.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreQuaternion.h"
#include "OgreMatrix4.h"
#include "OgreColourValue.h"
#include "OgreRay.h"

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Quaternion.h"
#include "math/Bounds.h"
#include "math/KernelMath.h"
#include "math/Matrix4.h"
#include "math/Color.h"
#include "math/Ray.h"

#include "KernelPrerequisites.h"

#include <string.h>
#include <vector>

//ver a utilidade dos includes abaixo.
#include "OgreCommon.h"
#include "OgreStringVector.h"
#include "OgreColourValue.h"

class KERNEL_VS_EXPORT Convert
{
public:
	//---------------------------- To Ogre -----------------------------------------------------
	static Ogre::Vector2 toOgre(const Vector2 &vec3)
	{
		return Ogre::Vector2(vec3.x, vec3.y);
	}

	static Ogre::Vector3 toOgre(const Vector3 &vec3)
	{
		return Ogre::Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Ogre::Vector3 toOgre(const physx::PxVec3 &vec3)
	{
		return Ogre::Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Ogre::Vector3 toOgre(const physx::PxExtendedVec3 &vec3)
	{
		return Ogre::Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Ogre::Quaternion toOgre(const Quaternion &q)
	{
		return Ogre::Quaternion(q.w, q.x, q.y, q.z);
	}

	static Ogre::Quaternion toOgre(const physx::PxQuat &q)
	{
		return Ogre::Quaternion(q.w, q.x, q.y, q.z);
	}

	static Ogre::AxisAlignedBox toOgre(const Bounds &b)
	{
		return Ogre::AxisAlignedBox(toOgre(b.getMinimum()), toOgre(b.getMaximum()));
	}

	static Ogre::AxisAlignedBox toOgre(const physx::PxBounds3 &b)
	{
		return Ogre::AxisAlignedBox(toOgre(b.minimum), toOgre(b.maximum));
	}

	static Ogre::Degree toOgre(const Degree &d)
	{
		return Ogre::Degree(d.valueDegrees());
	}

	static Ogre::Radian toOgre(const Radian &d)
	{
		return Ogre::Radian(d.valueRadians());
	}

	static Ogre::Vector4 toOgre(const Vector4 &v)
	{
		return Ogre::Vector4(v.x, v.y, v.z, v.w);
	}

	static Ogre::Matrix4 toOgre(const Matrix4 &m)
	{

		Ogre::Matrix4 mat4(   m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
			m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
			m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
			m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

		return mat4;
	}

	static Ogre::ColourValue toOgre(const Color& c)
	{
		return Ogre::ColourValue(c.r, c.g, c.b, c.a);
	}

	static Ogre::Ray toOgre(const Ray& c)
	{
		return Ogre::Ray(toOgre(c.getOrigin()), toOgre(c.getDirection()));
	}

	//------------------------------- To Kernel ------------------------------------------------
	static Vector3 toKernel(const Ogre::Vector3 &vec3)
	{
		return Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Vector3 toKernel(const physx::PxVec3 &vec3)
	{
		return Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Vector3 toKernel(const physx::PxExtendedVec3 &vec3)
	{
		return Vector3(vec3.x, vec3.y, vec3.z);
	}

	static Quaternion toKernel(const physx::PxQuat &q)
	{
		return Quaternion(q.w, q.x, q.y, q.z);
	}

	static Quaternion toKernel(const Ogre::Quaternion &q)
	{
		return Quaternion(q.w, q.x, q.y, q.z);
	}

	static Bounds toKernel(const Ogre::AxisAlignedBox &b)
	{
		return Bounds(toKernel(b.getMinimum()), toKernel(b.getMaximum()));
	}

	static Bounds toKernel(const physx::PxBounds3 &b)
	{
		return Bounds(toKernel(b.minimum), toKernel(b.maximum));
	}

	static const Matrix4 toKernel(const Ogre::Matrix4 &m)
	{

		const Matrix4 mat4(   m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
			m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
			m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
			m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

		return mat4;
	}

	static Color toKernel(const Ogre::ColourValue& c)
	{
		return Color(c.r, c.g, c.b, c.a);
	}

	static Ray toKernel(const Ogre::Ray& c)
	{
		return Ray(toKernel(c.getOrigin()), toKernel(c.getDirection()));
	}

	//------------------------------------- To PhysX --------------------------------------------
	static physx::PxVec3 toPx(const Ogre::Vector3 &vec3)
	{
		return physx::PxVec3(vec3.x, vec3.y, vec3.z);
	}

	static physx::PxVec3 toPx(const Vector3 &vec3)
	{
		return physx::PxVec3(vec3.x, vec3.y, vec3.z);
	}

	static physx::PxExtendedVec3 toPx(const Ogre::Vector3 &vec3, bool extended)
	{
		return physx::PxExtendedVec3(vec3.x, vec3.y, vec3.z);
	}

	static physx::PxExtendedVec3 toPx(const Vector3 &vec3, bool extended)
	{
		return physx::PxExtendedVec3(vec3.x, vec3.y, vec3.z);
	}

	static physx::PxQuat toPx(const Ogre::Quaternion &q)
	{
		physx::PxQuat pxq; pxq. w = q.w; pxq.x = q.x; pxq.y = q.y; pxq.z = q.z;
		return pxq;
	}

	static physx::PxQuat toPx(const Quaternion &q)
	{
		physx::PxQuat pxq; pxq. w = q.w; pxq.x = q.x; pxq.y = q.y; pxq.z = q.z;
		return pxq;
	}

	static physx::PxBounds3 toPx(const Ogre::AxisAlignedBox &b)
	{
		physx::PxBounds3 Pxb;
		Pxb.minimum = toPx(b.getMinimum());
		Pxb.maximum = toPx(b.getMaximum());
		return Pxb;
	}

	static physx::PxBounds3 toPx(const Bounds &b)
	{
		physx::PxBounds3 Pxb;
		Pxb.minimum = toPx(b.getMinimum());
		Pxb.maximum = toPx(b.getMaximum());
		return Pxb;
	}

	static physx::PxTransform toPxTransform(const Vector3 &pos, const Quaternion &rot)
	{
		return physx::PxTransform(toPx(pos), toPx(rot));
	}

	static physx::PxMat44 toPx(const Ogre::Vector3 &pos, const Ogre::Quaternion &rot)
	{
		return physx::PxMat44(physx::PxMat33(toPx(rot)), toPx(pos));
	}

	static physx::PxMat44 toPx(const Vector3 &pos, const Quaternion &rot)
	{
		return physx::PxMat44(physx::PxMat33(toPx(rot)), toPx(pos));
	}

	//-------------------------------------- String
	/** Converts a Real to a String. */
    static std::string toString(Real val, unsigned short precision = 6, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

#if KERNEL_DOUBLE_PRECISION == 1
    /** Converts a float to a String. */
    static std::string toString(float val, unsigned short precision = 6,
								unsigned short width = 0, char fill = ' ',
								std::ios::fmtflags flags = std::ios::fmtflags(0));
#else
    /** Converts a double to a String. */
    static String toString(double val, unsigned short precision = 6,
                            unsigned short width = 0, char fill = ' ',
                            std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif

    /** Converts a Radian to a String. */
    static std::string toString(Radian val, unsigned short precision = 6, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0))
    {
        return toString(val.valueAngleUnits(), precision, width, fill, flags);
    }

    /** Converts a Degree to a String. */
    static std::string toString(Degree val, unsigned short precision = 6, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0))
    {
        return toString(val.valueAngleUnits(), precision, width, fill, flags);
    }

    /** Converts an int to a String. */
    static std::string toString(int val, unsigned short width = 0, 
								char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

	/** Converts an unsigned int to a String. */
    static std::string toString(unsigned int val, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

    /** Converts a size_t to a String. */
    static std::string toString(size_t val, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

    /** Converts an unsigned long to a String. */
    static std::string toString(unsigned long val, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

    /** Converts a long to a String. */
    static std::string toString(long val, 
								unsigned short width = 0, char fill = ' ', 
								std::ios::fmtflags flags = std::ios::fmtflags(0));

    /** Converts a boolean to a String. 
    @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
    */
    static std::string toString(bool val, bool yesNo = false);

    /** Converts a Vector2 to a String. 
    @remarks
        Format is "x y" (i.e. 2x Real values, space delimited)
    */
    static std::string toString(const Vector2& val);

    /** Converts a Vector3 to a String. 
    @remarks
        Format is "x y z" (i.e. 3x Real values, space delimited)
    */
    static std::string toString(const Vector3& val);

    /** Converts a Vector4 to a String. 
    @remarks
        Format is "x y z w" (i.e. 4x Real values, space delimited)
    */
    static std::string toString(const Vector4& val);

    /** Converts a Matrix3 to a String. 
    @remarks
        Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
    */
    static std::string toString(const Matrix3& val);

    /** Converts a Matrix4 to a String. 
    @remarks
        Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
        '01' means row 0 column 1 etc.
    */
    static std::string toString(const Matrix4& val);

    /** Converts a Quaternion to a String. 
    @remarks
        Format is "w x y z" (i.e. 4x Real values, space delimited)
    */
    static std::string toString(const Quaternion& val);

    /** Converts a ColourValue to a String. 
    @remarks
        Format is "r g b a" (i.e. 4x Real values, space delimited). 
    */
    static std::string toString(const Color& val);

    /** Converts a StringVector to a string.
    @remarks
        Strings must not contain spaces since space is used as a delimiter in
        the output.
    */
    static std::string toString(const StringVector& val);

    /** Converts a String to a Real. 
    @return
        0.0 if the value could not be parsed, otherwise the Real version of the String.
    */
    static Real parseReal(const std::string& val, Real defaultValue = 0);

    /** Converts a String to a Angle. 
    @return
        0.0 if the value could not be parsed, otherwise the Angle version of the String.
    */
    static inline Radian parseAngle(const std::string& val, Radian defaultValue = Radian(0)) 
	{
        return Angle(parseReal(val, defaultValue.valueRadians()));
    }

    /** Converts a String to a whole number. 
    @return
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static int parseInt(const std::string& val, int defaultValue = 0);

    /** Converts a String to a whole number. 
    @return
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static unsigned int parseUnsignedInt(const std::string& val, unsigned int defaultValue = 0);

    /** Converts a String to a whole number. 
    @return
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static long parseLong(const std::string& val, long defaultValue = 0);

    /** Converts a String to a whole number. 
    @return
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static unsigned long parseUnsignedLong(const std::string& val, unsigned long defaultValue = 0);

    /** Converts a String to size_t. 
    @return
        defaultValue if the value could not be parsed, otherwise the numeric version of the String.
    */
    static size_t parseSizeT(const std::string& val, size_t defaultValue = 0);

    /** Converts a String to a boolean. 
    @remarks
        Returns true if case-insensitive match of the start of the string
        matches "true", "yes", "1", or "on", false if "false", "no", "0" 
        or "off".
    */
    static bool parseBool(const std::string& val, bool defaultValue = 0);

    /** Parses a Vector2 out of a String.
    @remarks
        Format is "x y" ie. 2 Real components, space delimited. Failure to parse returns
        Vector2::ZERO.
    */
    static Vector2 parseVector2(const std::string& val, const Vector2& defaultValue = Vector2::ZERO);

    /** Parses a Vector3 out of a String.
    @remarks
        Format is "x y z" ie. 3 Real components, space delimited. Failure to parse returns
        Vector3::ZERO.
    */
    static Vector3 parseVector3(const std::string& val, const Vector3& defaultValue = Vector3::ZERO);

    /** Parses a Vector4 out of a String.
    @remarks
        Format is "x y z w" ie. 4 Real components, space delimited. Failure to parse returns
        Vector4::ZERO.
    */
    static Vector4 parseVector4(const std::string& val, const Vector4& defaultValue = Vector4::ZERO);

    /** Parses a Matrix3 out of a String.
    @remarks
        Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
        Failure to parse returns Matrix3::IDENTITY.
    */
    static Matrix3 parseMatrix3(const std::string& val, const Matrix3& defaultValue = Matrix3::IDENTITY);

    /** Parses a Matrix4 out of a String.
    @remarks
        Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
        '01' means row 0 column 1 etc. Failure to parse returns Matrix4::IDENTITY.
    */
    static Matrix4 parseMatrix4(const std::string& val, const Matrix4& defaultValue = Matrix4::IDENTITY);

    /** Parses a Quaternion out of a String. 
    @remarks
        Format is "w x y z" (i.e. 4x Real values, space delimited). 
        Failure to parse returns Quaternion::IDENTITY.
    */
    static Quaternion parseQuaternion(const std::string& val, const Quaternion& defaultValue = Quaternion::IDENTITY);

    /** Parses a ColourValue out of a String. 
    @remarks
        Format is "r g b a" (i.e. 4x Real values, space delimited), or "r g b" which implies
        an alpha value of 1.0 (opaque). Failure to parse returns ColourValue::Black.
    */
    static Color parseColor(const std::string& val, const Color& defaultValue = Color::Black);

    /** Parses a StringVector from a string.
    @remarks
        Strings must not contain spaces since space is used as a delimiter in
        the output.
    */
    static StringVector parseStringVector(const std::string& val);

    /** Checks the String is a valid number value. */
    static bool isNumber(const std::string& val);

	/** Converts a ColourBufferType to a String.
	@remarks
		String output format is "Back", "Back Left", "Back Right", etc.
	*/
	//static std::string toString(ColourBufferType val);

	/** Converts a String to a ColourBufferType.
	@remarks
		String input format should be "Back", "Back Left", "Back Right", etc.
	*/
	//static ColourBufferType parseColourBuffer(const String& val, ColourBufferType defaultValue = CBT_BACK);

	/** Converts a StereoModeType to a String
	@remarks
		String output format is "None", "Frame Sequential", etc.
	*/
	//static std::string toString(StereoModeType val);

	/** Converts a String to a StereoModeType
	@remarks
		String input format should be "None", "Frame Sequential", etc.
	*/
	//static StereoModeType parseStereoMode(const std::string& val, StereoModeType defaultValue = SMT_NONE);
		
    //-----------------------------------------------------------------------
    static void setDefaultStringLocale(const std::string &loc)
    {
        msDefaultStringLocale = loc;
        msLocale = std::locale(msDefaultStringLocale.c_str());
    }

    //-----------------------------------------------------------------------
    static std::string getDefaultStringLocale(void) { return msDefaultStringLocale; }

    //-----------------------------------------------------------------------
    static void setUseLocale(bool useLocale) { msUseLocale = useLocale; }

    //-----------------------------------------------------------------------
    static bool isUseLocale() { return msUseLocale; }
    //-----------------------------------------------------------------------

protected:
    static std::string msDefaultStringLocale;
    static std::locale msLocale;
    static bool msUseLocale;
};



#endif // !OGRE_PHYSX_CONVERTER
