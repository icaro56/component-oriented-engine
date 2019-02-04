#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainWindow.h"
#include "ProjectPrerequisites.h"

class MyMainWindow : public MainWindow
{
	Q_OBJECT

public:
	static MyMainWindow *getInstance();

    static void destroy();  

	virtual void update();
	virtual void init();

	void setFullScreen(bool fullscreen);

	void loadMyStatus(bool geometry, bool state);

	void saveMyStatus();

protected:

    void closeEvent(QCloseEvent *e);

	void dragEnterEvent(QDragEnterEvent *evt);

	void dropEvent(QDropEvent *evt);

private:
	MyMainWindow(QWidget* parent = 0);
	virtual ~MyMainWindow();

    void createDockWidgets(); 

    void createMenus();

    void createStatusBar();

    void createToolBars();

private:
	static MyMainWindow *instance; 

	QMenu *navigationMenu, *visualizeMenu, *toolMenu;
	QToolBar *visualizeToolBar, *camToolBar, *toolsToolBar;
};

#endif