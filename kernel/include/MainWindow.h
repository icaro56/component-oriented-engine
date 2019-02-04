#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <QMainWindow>
#include "KernelPrerequisites.h"

class QResizeEvent;
class RenderView;

class KERNEL_VS_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

    size_t getWindowHandler();

	virtual void update() = 0;
	virtual void init() = 0;

	void createRenderView();

signals:
	void resizeEventOcurred(int w, int h);
	void activationWindow(bool active);
	void moveEventOcurred();

protected:

	virtual void resizeEvent(QResizeEvent *evt);
	
	/*!
     * \brief 
	 * \param 
     */
    virtual void changeEvent(QEvent *e);

	/*!
     * \brief 
	 * \param 
     */
	virtual void moveEvent(QMoveEvent *evt);

protected:
	bool mIsInitialised;

	RenderView* renderView;
};

#endif // APPLICATION_H_INCLUDED
