///////////////////////////////////////////////////////////
//  Time.h
//  Implementation of the Class Time
//  Created on:      23-jun-2012 06:38:22
//  Original author: Leandro
///////////////////////////////////////////////////////////

#ifndef TIME_INCLUDED_
#define TIME_INCLUDED_

#include <QDateTime>
#include "KernelPrerequisites.h"

#define Time TimeClass::getInstance()

namespace Ogre
{
	class Timer;
}

class KERNEL_VS_EXPORT TimeClass
{
public:	

	/*!
     * \brief 
	 * \return 
     */
	static TimeClass * getInstance();

	/*!
     * \brief 
	 * \return 
     */
	void init(Ogre::Timer* tm);

	double getFixedTime();
	void setFixedTime(double ftime);

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	double getTime() const;

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	QDateTime getDateTime() const;
	
	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setTime(double);

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setTime(const QDateTime&);

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	double getRealtimeSinceStartup() const;

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	QDateTime getDateRealtimeSinceStartup() const;
	
	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setRealtimeSinceStartup(double);

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setRealtimeSinceStartup(const QDateTime&);

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	double getDeltaTime() const;

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setDeltaTime(const double&);

	unsigned long getMilliseconds();

    unsigned long getMicroseconds();

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	double getFixedDeltaTime() const;

	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void setFixedDeltaTime(const double& fDt);

	void updateFixedTime();

	/*!
	* \brief The scale at which the time is passing. This can be used for slow motion effects.
	*	When timeScale is 1.0 the time is passing as fast as realtime. When timeScale is 0.5 the time is passing 2x slower than realtime.
     * \warning If you lower timeScale it is recommended to also lower Time.fixedDeltaTime by the same amount.
	 * Time.fixedDeltaTime = 0.02F * Time.timeScale;
     */
	void setTimeScale(double scale);
	double getTimeScale();

private:
	/*!
     * \brief 
     * \param 
	 * \return 
     */
	void update();
	

	/*!
     * \brief 
	 * \return 
     */
	virtual ~TimeClass();

	/*!
     * \brief 
	 * \return 
     */
	TimeClass();

	//! Construtor de copia
    TimeClass(const TimeClass&);

	//! operator de atribuição
    TimeClass& operator = (const TimeClass&);


private:

	static TimeClass* instance; //!<	

	double dt; //!<
	double fixedDeltaTime;

	double time; //!<
	double fixedTime;
	double realtimeSinceStartup;

	double timeScale;

	/// Timer from Ogre
    Ogre::Timer *mTimer;
	
	/// Usado no calculo do deltaTime
    unsigned long lastTime;
    unsigned long lastTimeReal;

	friend class Core;
};

#endif // !defined(TIME_INCLUDED_)
