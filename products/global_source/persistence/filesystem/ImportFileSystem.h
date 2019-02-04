///////////////////////////////////////////////////////////
//  ImportFileSystem.h
//  Implementation of the Class ImportFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#if !defined(IMPORT_FILE_H)
#define IMPORT_FILE_H

#include <QThread>
#include <QStringList>

class ImportFileSystem:  public QThread
{

public:	
	
	//! Destrutor
	virtual ~ImportFileSystem();
	
	/*!
     * \brief importa o dado
     * \param DataFileSystem 
     * \param QStringList
     */
	void importData(const QStringList& dir);

	/*!
     * \brief importa o dado
     * \param QString 
     */
	void importData(const QString& name);

	/*!
     * \brief 
     */
    void setActiveThread(bool);

	/*!
     * \brief 
     */
    bool isActiveThread() const;

	/*!
     * \brief 
     */
    void start();

	/*!
     * \brief 
     */
    void stop();

	/*!
     * \brief 
     */
    void setTimeSleep(int);

	/*!
     * \brief 
     */
    int getTimeSleep() const;

private:

	/*!
     * \brief 
     */
	void run();

	/*!
     * \brief importa o dado
     * \param QString 
     */
	virtual void read(const QString& name) = 0;


protected:

	//! Construtor
	ImportFileSystem();

	//! Construtor de copia
    ImportFileSystem(const ImportFileSystem&);

	//! operator de atribuição
    ImportFileSystem& operator = (const ImportFileSystem&);

private:

	int timeSleep;
	bool activeThread;
	QStringList listFile;

};
#endif // !defined(IMPORT_FILE_H)
