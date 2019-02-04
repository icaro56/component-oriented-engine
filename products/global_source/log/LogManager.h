///////////////////////////////////////////////////////////
//  LogManager.h
//  Implementation of the Class LogManager
//  Created on:     
///////////////////////////////////////////////////////////

#if !defined(LOG_MANAGER_H)
#define LOG_MANAGER_H

#include <QHash>
#include <QList>
#include <QMutex>
#include <QThread>

class ProcessorLog;
class EventLog;
class ReaderLog;

class LogManager: public QThread
{

public:	

	/*!
     * \brief 
	 * \return 
     */
	static LogManager * getInstance();	

	/*!
     * \brief 
	 * \return 
     */
	int getNumProcessors() const;	

	/*!
     * \brief 
	 * \return 
     */
	int getNumReaders() const;	

	/*!
     * \brief 
	 * \return 
     */
	void registerEvent(const EventLog&);	

	/*!
     * \brief 
	 * \return 
     */
	void registerProcessor(ProcessorLog*);

	/*!
     * \brief 
	 * \return 
     */
	void registerReader(ReaderLog*);

	/*!
     * \brief 
	 * \return 
     */
	void removeProcessor(int);

	/*!
     * \brief 
	 * \return 
     */
	void removeReader(int);

	/*!
     * \brief 
	 * \return 
     */
	void swapTurn();	

private:

	//! Construtor 
	LogManager();

	/*!
     * \brief 
	 * \return 
     */
	virtual ~LogManager();

	//! Construtor de copia
    LogManager(const LogManager&);

	//! operator de atribuição
    LogManager& operator = (const LogManager&);

	/*!
     * \brief 
	 * \return 
     */
	void run();

private:

	static LogManager* instance;	 //!<	

	int numProcessors; //!<
	int numReaders;
	bool canAdd; //!<
	QList<EventLog> events; //!<
	QHash<int,ProcessorLog*> processors; //!<
	QHash<int,ReaderLog*> readers; //!<
	QMutex mutexEvent; //!<	
};
#endif // !defined(ALERT_MANAGER_H)
