#include "utils/KernelConverter.h"

#include "utils/StringUtils.h"

//ver a utilidade dos includes abaixo
#include "OgreException.h"
#include "OgrePlatform.h"
#include "OgreStableHeaders.h"

std::string Convert::msDefaultStringLocale = OGRE_DEFAULT_LOCALE;
std::locale Convert::msLocale = std::locale(msDefaultStringLocale.c_str());
bool Convert::msUseLocale = false;

//-----------------------------------------------------------------------
std::string Convert::toString(Real val, unsigned short precision, 
								 unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;

	return stream.str();
}

#if KERNEL_DOUBLE_PRECISION == 1
//-----------------------------------------------------------------------
std::string Convert::toString(float val, unsigned short precision,
						      unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;

	return stream.str();
}

#else

//-----------------------------------------------------------------------
std::string Convert::toString(double val, unsigned short precision,
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;

	return stream.str();
}
#endif

//-----------------------------------------------------------------------
std::string Convert::toString(int val, 
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------

std::string Convert::toString(unsigned int val, 
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(size_t val, 
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;
	return stream.str();
}

//-----------------------------------------------------------------------
std::string Convert::toString(unsigned long val, 
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;
	return stream.str();
}


//-----------------------------------------------------------------------

std::string Convert::toString(long val, 
							  unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;

	if (msUseLocale)
		stream.imbue(msLocale);

	stream.width(width);
	stream.fill(fill);

	if (flags)
		stream.setf(flags);

	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Vector2& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);

	stream << val.x << " " << val.y;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Vector3& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);

	stream << val.x << " " << val.y << " " << val.z;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Vector4& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);

	stream << val.x << " " << val.y << " " << val.z << " " << val.w;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Matrix3& val)
{
	StringStream stream;
	stream.imbue(msLocale);
	stream << val[0][0] << " "
		<< val[0][1] << " "             
		<< val[0][2] << " "             
		<< val[1][0] << " "             
		<< val[1][1] << " "             
		<< val[1][2] << " "             
		<< val[2][0] << " "             
		<< val[2][1] << " "             
		<< val[2][2];

	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(bool val, bool yesNo)
{
	if (val)
	{
		if (yesNo)
		{
			return "yes";
		}
		else
		{
			return "true";
		}
	}
	else
		if (yesNo)
		{
			return "no";
		}
		else
		{
			return "false";
		}
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Matrix4& val)
{
	StringStream stream;
	stream.imbue(msLocale);
	stream << val[0][0] << " "
		<< val[0][1] << " "             
		<< val[0][2] << " "             
		<< val[0][3] << " "             
		<< val[1][0] << " "             
		<< val[1][1] << " "             
		<< val[1][2] << " "             
		<< val[1][3] << " "             
		<< val[2][0] << " "             
		<< val[2][1] << " "             
		<< val[2][2] << " "             
		<< val[2][3] << " "             
		<< val[3][0] << " "             
		<< val[3][1] << " "             
		<< val[3][2] << " "             
		<< val[3][3];
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Quaternion& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);

	stream  << val.w << " " << val.x << " " << val.y << " " << val.z;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const Color& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);
	stream << val.r << " " << val.g << " " << val.b << " " << val.a;
	return stream.str();
}
//-----------------------------------------------------------------------
std::string Convert::toString(const StringVector& val)
{
	StringStream stream;
	if (msUseLocale)
		stream.imbue(msLocale);
	StringVector::const_iterator i, iend, ibegin;
	ibegin = val.begin();
	iend = val.end();
	for (i = ibegin; i != iend; ++i)
	{
		if (i != ibegin)
			stream << " ";

		stream << *i; 
	}
	return stream.str();
}
//-----------------------------------------------------------------------
Real Convert::parseReal(const std::string& val, Real defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	Real ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
int Convert::parseInt(const std::string& val, int defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	int ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
unsigned int Convert::parseUnsignedInt(const std::string& val, unsigned int defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	unsigned int ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
long Convert::parseLong(const std::string& val, long defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	long ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
unsigned long Convert::parseUnsignedLong(const std::string& val, unsigned long defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	unsigned long ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
size_t Convert::parseSizeT(const std::string& val, size_t defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	size_t ret = defaultValue;
	if( !(str >> ret) )
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
bool Convert::parseBool(const std::string& val, bool defaultValue)
{
	//FIXME Returns both parsed value and error in same value - ambiguous.
	// Suggested alternatives: implement exception handling or make either
	// error or parsed value a parameter.
	if ((Ogre::StringUtil::startsWith(val, "true") || Ogre::StringUtil::startsWith(val, "yes")
		|| Ogre::StringUtil::startsWith(val, "1") ||  Ogre::StringUtil::startsWith(val, "on")))
		return true;
	else if ((Ogre::StringUtil::startsWith(val, "false") || Ogre::StringUtil::startsWith(val, "no")
		|| Ogre::StringUtil::startsWith(val, "0") ||  Ogre::StringUtil::startsWith(val, "off")))
		return false;
	else
		return defaultValue;
}
//-----------------------------------------------------------------------
Vector2 Convert::parseVector2(const std::string& val, const Vector2& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 2)
	{
		return defaultValue;
	}
	else
	{
		return Vector2(parseReal(vec[0], defaultValue[0]), parseReal(vec[1], defaultValue[1]));
	}
}
//-----------------------------------------------------------------------
Vector3 Convert::parseVector3(const std::string& val, const Vector3& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 3)
	{
		return defaultValue;
	}
	else
	{
		return Vector3(parseReal(vec[0], defaultValue[0]),
			parseReal(vec[1], defaultValue[1]),
			parseReal(vec[2], defaultValue[2]));
	}
}
//-----------------------------------------------------------------------
Vector4 Convert::parseVector4(const std::string& val, const Vector4& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 4)
	{
		return defaultValue;
	}
	else
	{
		return Vector4(parseReal(vec[0], defaultValue[0]),
			parseReal(vec[1], defaultValue[1]),
			parseReal(vec[2], defaultValue[2]),
			parseReal(vec[3], defaultValue[3]));
	}
}
//-----------------------------------------------------------------------
Matrix3 Convert::parseMatrix3(const std::string& val, const Matrix3& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 9)
	{
		return defaultValue;
	}
	else
	{
		return Matrix3(parseReal(vec[0], defaultValue[0][0]),
			parseReal(vec[1], defaultValue[0][1]),
			parseReal(vec[2], defaultValue[0][2]),

			parseReal(vec[3], defaultValue[1][0]),
			parseReal(vec[4], defaultValue[1][1]),
			parseReal(vec[5], defaultValue[1][2]),

			parseReal(vec[6], defaultValue[2][0]),
			parseReal(vec[7], defaultValue[2][1]),
			parseReal(vec[8], defaultValue[2][2]));
	}
}
//-----------------------------------------------------------------------
Matrix4 Convert::parseMatrix4(const std::string& val, const Matrix4& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 16)
	{
		return defaultValue;
	}
	else
	{
		return Matrix4(parseReal(vec[0], defaultValue[0][0]),
			parseReal(vec[1], defaultValue[0][1]),
			parseReal(vec[2], defaultValue[0][2]),
			parseReal(vec[3], defaultValue[0][3]),

			parseReal(vec[4], defaultValue[1][0]),
			parseReal(vec[5], defaultValue[1][1]),
			parseReal(vec[6], defaultValue[1][2]),
			parseReal(vec[7], defaultValue[1][3]),

			parseReal(vec[8], defaultValue[2][0]),
			parseReal(vec[9], defaultValue[2][1]),
			parseReal(vec[10], defaultValue[2][2]),
			parseReal(vec[11], defaultValue[2][3]),

			parseReal(vec[12], defaultValue[3][0]),
			parseReal(vec[13], defaultValue[3][1]),
			parseReal(vec[14], defaultValue[3][2]),
			parseReal(vec[15], defaultValue[3][3]));
	}
}
//-----------------------------------------------------------------------
Quaternion Convert::parseQuaternion(const std::string& val, const Quaternion& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() != 4)
	{
		return defaultValue;
	}
	else
	{
		return Quaternion(parseReal(vec[0], defaultValue[0]),
			parseReal(vec[1], defaultValue[1]),
			parseReal(vec[2], defaultValue[2]),
			parseReal(vec[3], defaultValue[3]));
	}
}
//-----------------------------------------------------------------------
Color Convert::parseColor(const std::string& val, const Color& defaultValue)
{
	// Split on space
	StringVector vec = StringUtil::split(val);

	if (vec.size() == 4)
	{
		return Color(parseReal(vec[0], defaultValue[0]),
			parseReal(vec[1], defaultValue[1]),
			parseReal(vec[2], defaultValue[2]),
			parseReal(vec[3], defaultValue[3]));
	}
	else if (vec.size() == 3)
	{
		return Color(parseReal(vec[0], defaultValue[0]),
			parseReal(vec[1], defaultValue[1]),
			parseReal(vec[2], defaultValue[2]),
			1.0f);
	}
	else
	{
		return defaultValue;
	}
}
//-----------------------------------------------------------------------
StringVector Convert::parseStringVector(const std::string& val)
{
	return StringUtil::split(val);
}
//-----------------------------------------------------------------------
bool Convert::isNumber(const std::string& val)
{
	StringStream str(val);
	if (msUseLocale)
		str.imbue(msLocale);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}
//-----------------------------------------------------------------------
/*std::string Convert::toString(ColourBufferType val)
{
	StringStream stream;
	switch (val)
	{
	case CBT_BACK:
		stream << "Back";
		break;
	case CBT_BACK_LEFT:
		stream << "Back Left";
		break;
	case CBT_BACK_RIGHT:
		stream << "Back Right";
		break;
	default:
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Unsupported colour buffer value", "Convert::toString(const ColourBufferType& val)");
	}

	return stream.str();
}*/
//-----------------------------------------------------------------------
/*ColourBufferType Convert::parseColourBuffer(const String& val, ColourBufferType defaultValue)
{
	ColourBufferType result = defaultValue;
	if (val.compare("Back") == 0)
	{
		result = CBT_BACK;
	}
	else if (val.compare("Back Left") == 0)
	{
		result = CBT_BACK_LEFT;
	}
	else if (val.compare("Back Right") == 0)
	{
		result = CBT_BACK_RIGHT;
	}		

	return result;
}*/
//-----------------------------------------------------------------------
/*String Convert::toString(StereoModeType val)
{
	StringStream stream;
	switch (val)
	{
	case SMT_NONE:
		stream << "None";
		break;
	case SMT_FRAME_SEQUENTIAL:
		stream << "Frame Sequential";
		break;
	default:
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Unsupported stereo mode value", "Convert::toString(const StereoModeType& val)");
	}

	return stream.str();
}*/
//-----------------------------------------------------------------------
/*StereoModeType Convert::parseStereoMode(const String& val, StereoModeType defaultValue)
{
	StereoModeType result = defaultValue;
	if (val.compare("None") == 0)
	{
		result = SMT_NONE;
	}
	else if (val.compare("Frame Sequential") == 0)
	{
		result = SMT_FRAME_SEQUENTIAL;
	}

	return result;
}*/