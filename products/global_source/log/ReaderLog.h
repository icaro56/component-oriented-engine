///////////////////////////////////////////////////////////
//  SendAlert.h
//  Implementation of the Interface SendsAlert
//  Created on:     
///////////////////////////////////////////////////////////

#if !defined(READER_LOG_H)
#define READER_LOG_H

#include <QFile>
#include <QStringList>
#include <QThread>
#include <QFileSystemWatcher>

class EventLog;

class ReaderLog: public QThread
{

	Q_OBJECT

public:
	/*!
		\brief Destrutor da classe abstrata
	*/
	virtual ~ReaderLog();
	
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
	void setPrefix(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getPrefix() const;

	/*!
     * \brief 
	 * \return 
     */
	void setSeparator(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getSeparator() const;

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
	void setExtension(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getExtension() const;

	/*!
     * \brief 
	 * \return 
     */
	void setDateFormat(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getDateFormat() const;

	/*!
     * \brief 
	 * \return 
     */
	int getId() const;	

	/*!
     * \brief 
	 * \return 
     */
	virtual void read(const QStringList&) = 0;

	/*!
     * \brief 
	 * \return 
     */
	void swapTurn();
	
	/*!
     * \brief 
	 * \return 
     */
	QString getName() const;

	/*!
     * \brief 
	 * \return 
     */
	void reset(const QString &dir="", const QString &prefix="", const QString &dateformat="",
		const QString &extension="", const QString &sep="");
	

protected:	

	/*!
		\brief Construtor da classe abstrata
	*/
	ReaderLog(const QString &dir="", const QString &prefix="", const QString &dateformat="",
		const QString &extension="", const QString &sep="");

	/*!
     * \brief 
	 * \return 
     */
	void setAppend(bool=true);

	/*!
     * \brief 
	 * \return 
     */
	bool isAppend() const;

private:

	/*!
     * \brief 
	 * \return 
     */
	void run();

	/*!
     * \brief 
	 * \return 
     */
	void clearWatcher();

	/*!
     * \brief 
	 * \return 
     */
	void readerAppend();

	/*!
     * \brief 
	 * \return 
     */
	void readerWriteOnly();

private slots:

	/*!
     * \brief 
	 * \return 
     */
	void fileChanged(const QString &);

private:

	QFileSystemWatcher watcher;//!<

	QFile file;
	bool active; //!<
	bool append; //!<
	QString prefix;//!<
	QString sep;//!<
	QString dir;//!<
	QString dateformat;//!<
	QString extension;//!<
	int id;//!<
};

#endif