#include "StatusBar.h"

#include "system/ProjectConfig.h"
#include "ProjectPrerequisites.h"

#include <QStatusBar>
#include <QComboBox>
#include <QLabel>
#include <QIcon>
#include <QProgressBar>
#include <QFrame>
#include <QHBoxLayout>
#include <QObject>

#define COORD_DEBUG

#include "view/UiUserLog.h"

//-----------------------------------------------------------------------------
StatusBar::StatusBar()
{
	statusBar = 0;
	lblPosGizmo = 0; 
	comboBox = 0; 
}

//-----------------------------------------------------------------------------
StatusBar::~StatusBar()
{
	
}

//-----------------------------------------------------------------------------
void StatusBar::init()
{
	
}

//-----------------------------------------------------------------------------
QString StatusBar::getTypeCoordinated()
{
	if(comboBox)
	{
		return comboBox->currentText();
	}
	return QObject::tr(" Coordenada Real ");
}

//-----------------------------------------------------------------------------
void StatusBar::setupUi(QStatusBar * sb)
{
	if(sb == statusBar)
	{
		return;
	}

	statusBar = sb;

#ifdef COORD_DEBUG
	comboBox = new QComboBox(sb);
	comboBox->addItem(QObject::tr(" Coordenada Real "));
	comboBox->addItem(QObject::tr(" Coordenada 3D "));
	statusBar->addPermanentWidget(comboBox);

	comboBox->setCurrentIndex(1);
	comboBox->setCurrentIndex(0);

#endif

	lblDefault = new QLabel(" ");
    lblDefault->setVisible(false);
    statusBar->addPermanentWidget(lblDefault, 1);

    lblPosCamera = new QLabel(QObject::tr(" Personagem: X: 0000.00 - Y: 0000.00 - Altitude: 0000.00 "));
    lblPosCamera->setMinimumSize(lblPosCamera->sizeHint());
    lblPosCamera->setFrameShape(QFrame::Panel);
    lblPosCamera->setFrameShadow(QFrame::Sunken);
    statusBar->addPermanentWidget(lblPosCamera);

    lblPosGizmo = new QLabel(" Gizmo: X: 0000.00 - Y: 0000.00 - Altitude: 0000.00 ");
    lblPosGizmo->setFrameShape(QFrame::Panel);
    lblPosGizmo->setFrameShadow(QFrame::Sunken);
    lblPosGizmo->setMinimumSize(lblPosGizmo->sizeHint() );
    statusBar->addPermanentWidget(lblPosGizmo);

	QString tip("");

#ifdef SIGNAL_STATUS_SMARTMINE
    tip = QObject::tr("<p>Número de Equipamentos em manutenção.<br/>Seu estado pode ser "
             "normal, simulado, falha ou indeterminado.</p>");
#else
	tip = QObject::tr("<p>Número de Equipamentos em manutenção.<br/>Seu estado pode ser "
             "normal, simulado ou falha.</p>");
#endif

    frmtenanceIcons = new QFrame(statusBar);
    frmtenanceIcons->setFrameShape(QFrame::Panel);
    frmtenanceIcons->setFrameShadow(QFrame::Sunken);

    laytenanceIcons = new QHBoxLayout; //(frmtenanceIcons);
    laytenanceIcons->setMargin(0);
    frmtenanceIcons->setLayout(laytenanceIcons);

    lbltenanceIco = new QLabel(frmtenanceIcons);
    
    lbltenanceIco->setToolTip(tip);
    laytenanceIcons->addWidget(lbltenanceIco);
	QIcon icontenance ( GlobalConfig::ICONPATH + "ico_manutencao.png");
	lbltenanceIco->setPixmap(icontenance.pixmap( QSize(22, 22) ) );

    lbltenanceTxt = new QLabel(" 0000 ", frmtenanceIcons);
    lbltenanceTxt->setAlignment(Qt::AlignCenter);
    lbltenanceTxt->setMinimumSize(lbltenanceTxt->sizeHint() );
    lbltenanceTxt->setToolTip(tip);
    lbltenanceTxt->setText("  0 ");
    laytenanceIcons->addWidget(lbltenanceTxt);

#ifdef FUNC_STATE_SIGNAL
    //-- adiciona o frame na barra de status
    addPermanentWidget(frmtenanceIcons);
#else	
	frmtenanceIcons->setVisible(false);	
#endif

    // ----- //

    lblTotalEquip = new QLabel(" Total: 0000 " );
    lblTotalEquip->setAlignment(Qt::AlignCenter);
    lblTotalEquip->setToolTip(QObject::tr("Total de equipamentos atualmente na mina."));
    lblTotalEquip->setFrameShape(QFrame::Panel);
    lblTotalEquip->setFrameShadow(QFrame::Sunken);
    lblTotalEquip->setMinimumSize(lblTotalEquip->sizeHint() );
    lblTotalEquip->setText("Total: 0 ");

#ifdef FUNC_STATE_SIGNAL
    //-- adiciona o frame na barra de status
    addPermanentWidget(lblTotalEquip);
#else	
	lblTotalEquip->setVisible(false);	
#endif

    // ------ //

    tip = QObject::tr("<p>Número de Equipamentos em estado normal.</p>");

    frmNormalIcons = new QFrame(statusBar);
    frmNormalIcons->setFrameShape(QFrame::Panel);
    frmNormalIcons->setFrameShadow(QFrame::Sunken);

    layNormalIcons = new QHBoxLayout; //(frmNormalIcons);
    layNormalIcons->setMargin(0);
    frmNormalIcons->setLayout(layNormalIcons);

    lblNormalIco = new QLabel(frmNormalIcons);
    
    lblNormalIco->setToolTip(tip);
    layNormalIcons->addWidget(lblNormalIco);  //-- adiciona o label no frame;

    lblNormalTxt = new QLabel(" 0000 ", frmNormalIcons);
    lblNormalTxt->setAlignment(Qt::AlignCenter);
    lblNormalTxt->setMinimumSize(lblNormalTxt->sizeHint() );
    lblNormalTxt->setToolTip(tip);
    lblNormalTxt->setText("  0 ");
	QIcon iconActive ( GlobalConfig::ICONPATH + "ico_gps_on.png");
	lblNormalIco->setPixmap(iconActive.pixmap(QSize(22, 22)));
    layNormalIcons->addWidget(lblNormalTxt);  //-- adiciona o label no frame

#ifdef FUNC_STATE_SIGNAL
    //-- adiciona o frame na barra de status
    addPermanentWidget(frmNormalIcons);
#else	
	frmNormalIcons->setVisible(false);	
#endif
    // ------ //

    tip = QObject::tr("<p>Número de Equipamentos em estado simulado.</p>");

    frmSimulIcons = new QFrame(statusBar);
    frmSimulIcons->setFrameShape(QFrame::Panel);
    frmSimulIcons->setFrameShadow(QFrame::Sunken);

    laySimulIcons = new QHBoxLayout; //(frmSimulIcons);
    laySimulIcons->setMargin(0);
    frmSimulIcons->setLayout(laySimulIcons);

    lblSimulIco = new QLabel(frmSimulIcons);
    
    lblSimulIco->setToolTip(tip);
    laySimulIcons->addWidget(lblSimulIco);  //-- adiciona o label no frame;
	QIcon iconSimulated ( GlobalConfig::ICONPATH + "ico_gps_simulacao.png");
	lblSimulIco->setPixmap(iconSimulated.pixmap(QSize(22, 22)));

    lblSimulTxt = new QLabel(" 0000 ", frmSimulIcons);
    lblSimulTxt->setAlignment(Qt::AlignCenter);
    lblSimulTxt->setMinimumSize(lblSimulTxt->sizeHint() );
    lblSimulTxt->setToolTip(tip);
    lblSimulTxt->setText("  0 ");
    laySimulIcons->addWidget(lblSimulTxt);  //-- adiciona o label no frame

#ifdef FUNC_STATE_SIGNAL
    // -- adiciona o frame na barra de status
    addPermanentWidget(frmSimulIcons);
#else	
	frmSimulIcons->setVisible(false);	
#endif
    // ------ //

    tip = QObject::tr("<p>Número de Equipamentos em estado de falha.</p>");

    frmFailIcons = new QFrame(statusBar);
    frmFailIcons->setFrameShape(QFrame::Panel);
    frmFailIcons->setFrameShadow(QFrame::Sunken);

    layFailIcons = new QHBoxLayout; //(frmFailIcons);
    layFailIcons->setMargin(0);
    frmFailIcons->setLayout(layFailIcons);

    lblFailIco = new QLabel(frmFailIcons);
    
    lblFailIco->setToolTip(tip);
    layFailIcons->addWidget(lblFailIco);
	QIcon iconFail ( GlobalConfig::ICONPATH + "ico_gps_off.png");
	lblFailIco->setPixmap(iconFail.pixmap(QSize(22, 22)));

    lblFailTxt = new QLabel(" 0000 ", frmFailIcons);
    lblFailTxt->setAlignment(Qt::AlignCenter);
    lblFailTxt->setMinimumSize(lblFailTxt->sizeHint() );
    lblFailTxt->setToolTip(tip);
    lblFailTxt->setText("  0 ");
    layFailIcons->addWidget(lblFailTxt);

#ifdef FUNC_STATE_SIGNAL
    addPermanentWidget(frmFailIcons);
#else	
	frmFailIcons->setVisible(false);	
#endif   

	// ------ //

    tip = QObject::tr("<p>Número de Equipamentos em estado indeterminado.</p>");

    frmUnknowIcons = new QFrame(statusBar);
    frmUnknowIcons->setFrameShape(QFrame::Panel);
    frmUnknowIcons->setFrameShadow(QFrame::Sunken);

    layUnknowIcons = new QHBoxLayout; //(frmUnknowIcons);
    layUnknowIcons->setMargin(0);
    frmUnknowIcons->setLayout(layUnknowIcons);

    lblUnknowIco = new QLabel(frmUnknowIcons);
    
    lblUnknowIco->setToolTip(tip);
    layUnknowIcons->addWidget(lblUnknowIco);
	QIcon iconUnknow ( GlobalConfig::ICONPATH + "ico_gps_unknow.png");
	lblUnknowIco->setPixmap(iconUnknow.pixmap(QSize(22, 22)));

    lblUnknowTxt = new QLabel(" 0000 ", frmUnknowIcons);
    lblUnknowTxt->setAlignment(Qt::AlignCenter);
    lblUnknowTxt->setMinimumSize(lblUnknowTxt->sizeHint() );
    lblUnknowTxt->setToolTip(tip);
    lblUnknowTxt->setText("  0 ");
    layUnknowIcons->addWidget(lblUnknowTxt);

#ifdef FUNC_STATE_SIGNAL
    addPermanentWidget(frmUnknowIcons);
#else	
	frmUnknowIcons->setVisible(false);	
#endif

#ifndef SIGNAL_STATUS_SMARTMINE	
		frmUnknowIcons->setVisible(false);	
#endif

    // Troca de terrenos
    lblChangeTerrain = 0;
    progress = 0;
}

//-----------------------------------------------------------------------------
void StatusBar::setCameraCoordinated(const QString& x, const QString& y, 
		const QString& z)
{
	QString r;
	r += QObject::tr(" Câmera: X: ");
	r += x;
	r += QObject::tr(" Y: ");
	r += y;
	r += " - ";
	r += QObject::tr(" Altitude: ");
	r += z;

	if(lblPosCamera)
		lblPosCamera->setText(r);
}

//-----------------------------------------------------------------------------
void StatusBar::setGizmoCoordinated(QString x, QString y, 
		QString z)
{
	QString r;
	r += QObject::tr(" Gizmo: X: ");
	r += x;
	r += " - ";
	r += QObject::tr(" Y: ");
	r += y;
	r += " - ";
	r += QObject::tr(" Altitude: ");
	r += z;	

	if(lblPosGizmo)
		lblPosGizmo->setText(r);
	
}