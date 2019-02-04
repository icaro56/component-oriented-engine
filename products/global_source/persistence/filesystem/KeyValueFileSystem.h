///////////////////////////////////////////////////////////
//  KeyValueFileSystem.h
//  Implementation of the Class KeyValueFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#if !defined(KEYVALUEFILESYSTEM_H)
#define KEYVALUEFILESYSTEM_H

#include <QMap>
#include <QString>

class KeyValueFileSystem
{

public:	

	//! Construtor
	KeyValueFileSystem(const QString& fn = "", const QString& s = "");

	//! Destrutor
	virtual ~KeyValueFileSystem();

	//! Construtor de copia
    KeyValueFileSystem(const KeyValueFileSystem&);

	//! operator de atribuição
    KeyValueFileSystem& operator = (const KeyValueFileSystem&);

	/*!
     * \brief 
	 * \param 
     */
	void setFileName(const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void setSeparator(const QString&);
	
	/*!
     * \brief 
	 * \param 
     */
	bool read();

	/*!
     * \brief 
	 * \param 
     */
	void write() const;

	/*!
     * \brief 
	 * \param 
     */
	void setAttribute(const QString & key, const QString & value);
    
	
	/*!
     * \brief 
	 * \param 
     */
	const QString getAttribute(const QString & key) const;


protected:	

	QString filename;
	QString separator;
	QMap<QString, QString> attributes;

};
#endif // !defined(EXPORT_FILE_H)
