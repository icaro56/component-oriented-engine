#include "math/Vector3.h"

#include "OgreStableHeaders.h"
#include "OgreVector3.h"
#include "OgreMath.h"

const Vector3 Vector3::ZERO( 0, 0, 0 );

const Vector3 Vector3::UNIT_X( 1, 0, 0 );
const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
const Vector3 Vector3::UNIT_Z( 0, 0, 1 );

const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
const Vector3 Vector3::LEFT( Vector3::NEGATIVE_UNIT_X );
const Vector3 Vector3::RIGHT( Vector3::UNIT_X );

const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
const Vector3 Vector3::UP(  Vector3::UNIT_Y );
const Vector3 Vector3::DOWN(  Vector3::NEGATIVE_UNIT_Y );

const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
const Vector3 Vector3::FORWARD( Vector3::UNIT_Z);
const Vector3 Vector3::BACK( Vector3::NEGATIVE_UNIT_Z);

const Vector3 Vector3::UNIT_SCALE(1, 1, 1);
const Vector3 Vector3::ONE(Vector3::UNIT_SCALE);