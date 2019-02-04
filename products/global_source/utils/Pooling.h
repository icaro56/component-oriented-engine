///////////////////////////////////////////////////////////
//  Pooling.h
//  Implementation of the Class Pooling
//  Created on:      23-jun-2012 06:38:22
//  Original author: Leandro
///////////////////////////////////////////////////////////

#ifndef POOLING_H
#define POOLING_H

//#include <QDateTime>

class Pooling
{

public:		

	/*!
     * \brief 
	 * \return 
     */
	Pooling();

	/*!
     * \brief 
	 * \return 
     */
	virtual ~Pooling();

	//! Construtor de copia
    Pooling(const Pooling&);

	//! operator de atribuição
    Pooling& operator = (const Pooling&);

	/*!
     * \brief 
	 * \return 
     */
	int getInterval() const;

	/*!
     * \brief 
	 * \return 
     */
	void setInterval(int);

	/*!
     * \brief 
	 * \return 
     */
	void start();

	/*!
     * \brief 
	 * \return 
     */
	void stop();

	/*!
     * \brief 
	 * \return 
     */
	void exec();

	/*!
     * \brief 
	 * \return 
     */
	virtual void pooling() = 0;

	void setInitialTime(int time);

	int getInitialTime() const;

	void setActivated(bool active);

	bool getActivated() const;

	void swap(Pooling& other);

private:
	
	bool activated;
	int initialTime;
	int interval; //!<
	
};
#endif // !defined(Pooling_H)
