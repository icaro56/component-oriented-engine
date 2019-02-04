///////////////////////////////////////////////////////////
//  EventLog.h
//  Implementation of the Interface EventLog
//  Created on:     
///////////////////////////////////////////////////////////

#if !defined(EVENT_LOG_H)
#define EVENT_LOG_H

#include <QString>
#include <QHash>

class ProcessorLog;

class EventLog
{

public:

	/*!
     * \brief 
	 * \return 
     */
	EventLog();

	/*!
     * \brief 
	 * \return 
     */
	virtual ~EventLog();

	/*!
     * \brief 
	 * \return 
     */
	void setProcessor(ProcessorLog*);

	/*!
     * \brief 
	 * \return 
     */
	ProcessorLog* getProcessor() const;		

	/*!
     * \brief 
	 * \return 
     */
	QString getValueProperties(const QString& key) const;
	
	/*!
     * \brief 
	 * \return 
     */
	void setValueProperties(const QString& key,const QString& value);
	
private:
	
	ProcessorLog * processor; 	
	QHash<QString,QString> properties;

};
#endif // !defined(EVENT_ALERT_H)
