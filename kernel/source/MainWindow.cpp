#include "MainWindow.h"
#include "moc_MainWindow.cpp"

#include <QResizeEvent>
#include "engine/RenderView.h"

MainWindow::MainWindow(QWidget* parent)
    :QMainWindow(parent), 
	 mIsInitialised(false),
	 renderView(nullptr)
{

}

MainWindow::~MainWindow() 
{

}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
	if (evt->isAccepted())
	{
		emit resizeEventOcurred(width(), height());
	}

	QMainWindow::resizeEvent(evt);
}

//-----------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange)
	{
		if (isActiveWindow())
		{
			//std::cerr << "\nAtivo";
			activationWindow(true);
		}
		else
		{
			//std::cerr << "\nNão Ativo";
			activationWindow(false);
		}
	}

	QMainWindow::changeEvent(e);
}


//-----------------------------------------------------------------------------
void MainWindow::moveEvent(QMoveEvent *evt)
{
	if (evt->isAccepted())
	{
		emit moveEventOcurred();
	}

	QMainWindow::moveEvent(evt);
}

size_t MainWindow::getWindowHandler()
{
	return (size_t)winId();
}

void MainWindow::createRenderView()
{
	//iniciando renderview principal
	renderView = new RenderView("render1", this);
	//só podemos inicializar o RenderView após criar tudo. Evitando o erro de lost device directx. Permitindo usar PerfHUD
	renderView->initialise(true);
	mIsInitialised = true;
	this->setCentralWidget(renderView); 
}