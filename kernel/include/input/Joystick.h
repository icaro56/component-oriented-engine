#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "OISJoyStick.h"
#include "KernelPrerequisites.h"

///////////////////////////////////////////////////////////
// CLASS Joystick
///////////////////////////////////////////////////////////
/**
 * Input JoyStick.
 *
 * Manages JoyStick input.
 * @see OIS::JoyStickListener
 */
class Joystick : public OIS::JoyStickListener
{
public:

    Joystick(OIS::JoyStick *j);

    ~Joystick();

    bool buttonPressed(const OIS::JoyStickEvent &arg, int button);

    bool buttonReleased(const OIS::JoyStickEvent &arg, int button);

    bool axisMoved(const OIS::JoyStickEvent &arg, int axis);

    bool povMoved(const OIS::JoyStickEvent &arg, int pov);

    bool joyDown(const int button);

    int joyHit(const int button);

    double joyAxis(const int axis);

    int joyPOV(const int pov);

    std::string getName();

    int getID();

    void capture();

    int numButtons();

    int numAxis();

    int numPOVs();

private:

    bool *mJoyDown;
    unsigned short *mJoyHitCount;
    int mJoyButtons, mJoyAxis, mJoyPOVs;
    OIS::JoyStick *mJoyStick;
    int mMinAxis, mMaxAxis;
};

#endif
