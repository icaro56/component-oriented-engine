#include "Joystick.h"
#include <OISInputManager.h>


///////////////////////////////////////////////////////////
// PUBLIC METHODS
///////////////////////////////////////////////////////////

Joystick::Joystick(OIS::JoyStick *j)
{
    mJoyStick = j;
    mJoyStick->setEventCallback(this);

    mJoyButtons = mJoyStick->getNumberOfComponents(OIS::OIS_Button);
    mJoyHitCount = new unsigned short[mJoyButtons];
    mJoyPOVs = mJoyStick->getNumberOfComponents(OIS::OIS_POV);
    mJoyDown = new bool[mJoyButtons];
    mJoyAxis = mJoyStick->getNumberOfComponents(OIS::OIS_Axis);
    mMinAxis = mJoyStick->MIN_AXIS;
    mMaxAxis = mJoyStick->MAX_AXIS;

    for (int i = 0; i < mJoyButtons; ++i)
    {
        mJoyDown[i] = false;
        mJoyHitCount[i] = 0;
    }
}

//----------------------------------------------------------
Joystick::~Joystick()
{
    mJoyStick->getCreator()->destroyInputObject(mJoyStick);

    if (mJoyDown)
    {
        delete [] mJoyDown;
        mJoyDown = 0;
    }

    if (mJoyHitCount)
    {
        delete [] mJoyHitCount;
        mJoyHitCount = 0;
    }
}

//----------------------------------------------------------
bool Joystick::joyDown(const int button)
{
    if (button >= 1 && button <= mJoyButtons)
        return mJoyDown[button-1];
    else
        return false;
}

//----------------------------------------------------------
int Joystick::joyHit(const int button)
{
    if (button >= 1 && button <= mJoyButtons)
    {
        int count = mJoyHitCount[button-1];
        mJoyHitCount[button-1] = 0;
        return count;
    }
    else
        return 0;
}

//----------------------------------------------------------
double Joystick::joyAxis(const int axis)
{
    if (axis >= 0 && axis <= mJoyAxis)
    {
        int abs = mJoyStick->getJoyStickState().mAxes[axis].abs;

        if (abs > 0)
            return double(abs) / mMaxAxis;
        else if (abs < 0)
            return double(abs) / (-mMinAxis);
    }

    return 0;
}

//----------------------------------------------------------
int Joystick::joyPOV(const int pov)
{
    // 8 1 2
    // 7 0 3
    // 6 5 4

    if (pov >= 0 && pov <= mJoyPOVs)
    {
        int dir = mJoyStick->getJoyStickState().mPOV[pov].direction;

        if (dir & OIS::Pov::Centered)
            return 0;
        else if (dir & OIS::Pov::North)
        {
            if (dir & OIS::Pov::East)
                return 2;
            else if (dir & OIS::Pov::West)
                return 8;
            else
                return 1;
        }
        else if (dir & OIS::Pov::South)
        {
            if (dir & OIS::Pov::East)
                return 4;
            else if (dir & OIS::Pov::West)
                return 6;
            else
                return 5;
        }
        else if (dir & OIS::Pov::East)
            return 3;
        else if (dir & OIS::Pov::West)
            return 7;

    }

    return 0;
}

//----------------------------------------------------------
bool Joystick::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
    if (button <= mJoyButtons)
    {
        ++mJoyHitCount[button];
        mJoyDown[button] = true;
    }

    return true;
}

//----------------------------------------------------------
bool Joystick::buttonReleased(const OIS::JoyStickEvent &arg, int button)
{
    if (button <= mJoyButtons)
        mJoyDown[button] = false;

    return true;
}

//----------------------------------------------------------
bool Joystick::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
    // 0 1 2 3  4  5
    // x y z rx ry rz

    return true;
}

//----------------------------------------------------------
bool Joystick::povMoved(const OIS::JoyStickEvent &arg, int pov)
{
    return true;
}

//----------------------------------------------------------
std::string Joystick::getName()
{
    return mJoyStick->vendor();
}

//----------------------------------------------------------
int Joystick::getID()
{
    return mJoyStick->getID();
}

//----------------------------------------------------------
void Joystick::capture()
{
    mJoyStick->capture();
}

//----------------------------------------------------------
int Joystick::numButtons()
{
    return mJoyButtons;
}

//----------------------------------------------------------
int Joystick::numAxis()
{
    return mJoyAxis;
}

//----------------------------------------------------------
int Joystick::numPOVs()
{
    return mJoyPOVs;
}

