#ifndef CAMCONTROL_H
#define CAMCONTROL_H

#include <QObject>
#include <QVector>
#include "view/UiBasic.h"

class QMenu;
class QToolBar; 

class CamControl : public QObject
{
	Q_OBJECT

public:
	static CamControl* getInstance();

	void destroy();

	/*!
     * \brief 
	 * \param 
     */
	void addActionsInMenu(QMenu*);

	/*!
     * \brief 
	 * \param 
     */
	void addActionsInToolbar(QToolBar*);

private slots: 
	void processingCamActived(unsigned int);

private:
	static CamControl* instance;

private:
	CamControl();
	~CamControl();

	QVector<UiBasic*> camerasActions;

};

#endif