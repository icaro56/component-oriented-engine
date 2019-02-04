#include "Close.h"
//#include "moc_Close.cpp"

#include <QMessageBox>
#include <QPushButton>
//#include "ui/UiUserLog.h"

//-----------------------------------------------------------------------------
Close::Close()
{	
}

//-----------------------------------------------------------------------------
Close::~Close()
{
	
}
#include <iostream>
//-----------------------------------------------------------------------------
bool Close::closeView()
{
	QMessageBox msgBox;
	msgBox.setModal(true);
    QPushButton *butYes = msgBox.addButton(tr("&Sim"), QMessageBox::YesRole);
    QPushButton *butNo = msgBox.addButton(tr("&Não"), QMessageBox::NoRole);
    msgBox.setDefaultButton(butNo);

    msgBox.setWindowTitle(tr("Projeto X"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("Você está tentando finalizar o programa.\n")
                   + tr("Deseja continuar?"));
    msgBox.exec();

	return (msgBox.clickedButton() == butYes);
}

//-----------------------------------------------------------------------------
void Close::show(bool)
{
	if(closeView())
	{
		emit closeApplication();
	}
}
