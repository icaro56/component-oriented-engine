///////////////////////////////////////////////////////////
//  Close.h
//  Implementation of the Class Close
///////////////////////////////////////////////////////////

#if !defined(CLOSE_H)
#define CLOSE_H

#include <QObject>

class QEvent;
class QDateTime;

class Close : public QObject
{
    Q_OBJECT

public:

    //! Construtor
    Close();

    //! Destrutor
    ~Close();

	/*!
     * \brief 
	 * \param 
     */
	bool closeView();

public slots:

	/*!
     * \brief 
	 * \param 
     */
	void show(bool=true);

signals:

	/*!
     * \brief 
	 * \param 
     */
    void closeApplication();

};

#endif // CLOSE_H
