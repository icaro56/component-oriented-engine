///////////////////////////////////////////////////////////
//  UiUserLog.h
//  Implementation of the Class UiUserLog
//  Created on:      
///////////////////////////////////////////////////////////

#if !defined(UI_USER_LOG_H)
#define UI_USER_LOG_H

#include "log/ProcessorLog.h"

class UiUserLog: public ProcessorLog
{

public:	
	
	//! Destrutor
	virtual ~UiUserLog();	

	/*!
     * \brief 
	 * \param 
     */
	static UiUserLog * getInstance();	

	/*!
     * \brief 
	 * \return 
     */
	void write(const EventLog&);

	/*!
     * \brief 
	 * \return 
     */
	void swapTurn();

	/*!
     * \brief 
	 * \return 
     */
	void reset();
			
private:

	//! Construtor
	UiUserLog();
	
	//! Construtor de copia
    UiUserLog(const UiUserLog&);

	//! operator de atribuição
    UiUserLog& operator = (const UiUserLog&);

	/*!
     * \brief 
	 * \return 
     */
	void init();	

	/*!
     * \brief 
	 * \return 
     */
	bool name();

	/*!
     * \brief 
	 * \return 
     */
	void head();

public:

	static const QString Type;
	static const QString Id;
	static const QString CurrentState;
	static const QString LastState;
	static const QString TimeLog;
	
private:

	static UiUserLog* instance;

};
#endif // !defined(PROCESSOR_LOG_GPSSIGNAL_FILE_H)
