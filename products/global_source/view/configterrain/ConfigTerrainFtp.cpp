#include "ConfigTerrainFtp.h"
#include "ui_ConfigTerrainFtp.h"
//#include "moc_ConfigTerrainFtp.cpp"

#include "system/ProjectConfig.h"

#include "ProjectPrerequisites.h"

#include <QFrame>
#include <QPushButton>
#include <QTreeWidget>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QEvent>
#include <QFileDialog>
#include <QTextStream>

//-----------------------------------------------------------------------------
ConfigTerrainFtp::ConfigTerrainFtp(QWidget *parent)
    : QDialog(parent)
{
	window = new Ui::ConfigTerrainFtp();

    window->setupUi(this);

	//Não escreveremos mais o terreno atual na barra de título
	//setWindowTitle(tr("Terreno Atual: " + terrainName));

	//escondendo opções avançadas
	oldSize = QSize(397, 268);
	window->btnOpenAdvanceOption->setChecked(false);
		
	//colocando ícones
	window->butFile->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_abrir.png"));
	window->butFtpConn->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_abrir_nuvem.png"));
	window->butDelete->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_excluir_tudo.png"));
}

//-----------------------------------------------------------------------------
ConfigTerrainFtp::~ConfigTerrainFtp()
{
	
}

//-----------------------------------------------------------------------------
void ConfigTerrainFtp::init()
{
	
}

//-----------------------------------------------------------------------------
bool ConfigTerrainFtp::event(QEvent *e)
{
    switch(e->type())
    {
    case (QEvent::Hide)     :
    case (QEvent::Close)    :
        emit visibilityChanged(false);
        break;

    case (QEvent::Show)     :
        emit visibilityChanged(true);
        break;

    default                 :
        break;
    }
    return QWidget::event(e);
}