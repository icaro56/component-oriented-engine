///////////////////////////////////////////////////////////
//  SendAlert.h
//  Implementation of the Interface SendsAlert
//  Created on:     
///////////////////////////////////////////////////////////

#if !defined(PROCESSOR_LOG_H)
#define PROCESSOR_LOG_H

#include <QFile>

class EventLog;

class ProcessorLog
{

public:	

	/*!
     * \brief 
	 * \return 
     */
	virtual ~ProcessorLog();

	/*!
     * \brief 
	 * \return 
     */
	void receives(EventLog& event);

	/*!
     * \brief 
	 * \return 
     */
	void setActive(bool);

	/*!
     * \brief 
	 * \return 
     */
	bool isActive() const;

	/*!
     * \brief 
	 * \return 
     */
	void setDir(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getDir() const;

	/*!
     * \brief 
	 * \return 
     */
	int getId() const;	

	/*!
     * \brief 
	 * \return 
     */
	virtual void write(const EventLog&)=0;

	/*!
     * \brief 
	 * \return 
     */
	virtual void swapTurn()=0;

	/*!
     * \brief 
	 * \return 
     */
	virtual void reset()=0;

protected:

	/*!
     * \brief 
	 * \return 
     */
	ProcessorLog();

private:

	/*!
     * \brief 
	 * \return 
     */
	virtual void init()=0;	

	/*!
     * \brief 
	 * \return 
     */
	virtual bool name()=0;

	/*!
     * \brief 
	 * \return 
     */
	virtual void head()=0;

protected:

	QFile file; //!<
	
private:

	bool active; //!<
	QString dir;//!<
	int id;//!<
	
};
#endif // !defined(SENDER_ALERT_H)
