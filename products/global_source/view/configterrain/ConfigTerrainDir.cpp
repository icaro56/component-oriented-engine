#include "ConfigTerrainDir.h"
#include "ui_ConfigTerrainDir.h"
//#include "moc_ConfigTerrainDir.cpp"

#include "system/ProjectConfig.h"
#include "view/delegates/SpinBoxDelegate.h"

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
#include <QSpinBox>

//-----------------------------------------------------------------------------
ConfigTerrainDir::ConfigTerrainDir(QWidget *parent)
    : QDialog(parent)
{
	inAddition = false;

	window = new Ui::ConfigTerrainDir();

    window->setupUi(this);
	
	window->pushButtonAdd->setEnabled(false);
	window->pushButtonRemove->setEnabled(false);

	connects();

	spinBoxDelegate = 0;		
	spinBoxDelegatePort = 0;		
}

//-----------------------------------------------------------------------------
ConfigTerrainDir::~ConfigTerrainDir()
{
	
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::setPaths(const QList<QStringList>& list)
{
	paths.clear();
	foreach(QStringList value,list)
	{
		if(value.size())
		{
			int id = value.last().toInt();
			paths.insert(id,value);
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::setTypes(const QStringList& list)
{
	window->comboBoxTypeDir->clear();
	window->comboBoxTypeDir2->clear();
	foreach(QString v, list)
	{
		window->comboBoxTypeDir->addItem(v);
		window->comboBoxTypeDir2->addItem(v);
	}
}

//-----------------------------------------------------------------------------
unsigned int ConfigTerrainDir::idType(const QString& t)
{
	if(t.toUpper() == "")
	{
		return 0;
	}
	else if(t.toUpper() == tr("DIRETÓRIO"))
	{
		return 1;
	}
	else if(t.toUpper() == tr("FTP"))
	{
		return 2;
	}
}

//-----------------------------------------------------------------------------
bool ConfigTerrainDir::event(QEvent *e)
{
    switch(e->type())
    {
    case (QEvent::Hide)     :
    case (QEvent::Close)    :
		{
			if(inAddition)
			{
				int ret = QMessageBox::warning(0,
				QObject::tr("Local para monitoramento."),
				QObject::tr("A adição não foi finalizada."));
				e->ignore();
				return true;
			}		
			emit visibilityChanged(false);
		}
    break;

    case (QEvent::Show)     :
        emit visibilityChanged(true);
        break;

    default                 :
        break;
    }

	
	return QWidget::event(e);	
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::addPath(bool)
{
	int r = window->tableWidgetDirs->rowCount();
	int c = window->tableWidgetDirs->columnCount()-2;
	if(!inAddition)
	{	
		disconnect
		(
			window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
			this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
		);

		for(int i=0;i<r;i++)
		{					
			window->tableWidgetDirs->hideRow(i);					
		}

		window->tableWidgetDirs->insertRow(r);
		window->pushButtonAdd->setText(tr("Finalizar adição"));

		addItems(r);

		idType(window->comboBoxTypeDir->currentText());
		
		window->tableWidgetDirs->item(r,c)->setText(
			QString::number(idType(window->comboBoxTypeDir->currentText())));

		window->comboBoxTypeDir->setEnabled(false);

		window->tabWidget->setTabEnabled(0,false);
		window->tabWidget->setTabEnabled(1,false);

		inAddition = true;
	}
	else
	{
		r -= 1;
		QStringList aux;
		bool cond = false;

		window->tableWidgetDirs->item(r,0)->setCheckState(Qt::Unchecked);

		for(int i=1;i<=c;i++)
		{	
			if(!window->tableWidgetDirs->item(r,i))
			{
				cond = true;
				aux << "";
			}
			else
			{
				aux << window->tableWidgetDirs->item(r,i)->text();
			}
		}
		
		if(cond)
		{
			QMessageBox msgBox;
			msgBox.setText(tr("Local para monitoramento."));
			msgBox.setInformativeText(tr("Deseja finalizar a adição do local?"));
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			int ret = msgBox.exec();	
			if(ret == QMessageBox::Cancel)
			{
				connect
				(
					window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
					this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
				);
				return;		
			}
				
		}

		//-----------------------------------------
		for(int i=0;i<r;i++)
		{					
			window->tableWidgetDirs->showRow(i);					
		}
		window->comboBoxTypeDir->setEnabled(true);

		window->pushButtonAdd->setText(tr("Adicionar"));			

		inAddition = false;

		window->tabWidget->setTabEnabled(0,true);
		window->tabWidget->setTabEnabled(1,true);

		aux<< window->comboBoxTypeDir->currentText();
		emit addPath(aux);

		connect
		(
			window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
			this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
		);
	}	
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::removePath(bool)
{
	QStringList aux;

	int r = window->tableWidgetDirs->rowCount();
	int c = window->tableWidgetDirs->columnCount();
	for(int i=0;i<r;i++)
	{					
		QTableWidgetItem* item = 
					window->tableWidgetDirs->item(i,0);
		if(item->checkState() == Qt::Checked)
		{	
			aux << window->tableWidgetDirs->item(i,c-1)->text();
			window->tableWidgetDirs->removeRow(i);
			i--;
			r--;
		}				
	}

	window->pushButtonRemove->setEnabled(false);
	emit removePath(aux);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::download(bool)
{
	int id = window->comboBoxLocal->itemData(window->comboBoxLocal->currentIndex()).toInt();

	QStringList aux;

	int r = window->tableWidgetObtain->rowCount();
	int c = window->tableWidgetObtain->columnCount();
	for(int i=0;i<r;i++)
	{					
		QTableWidgetItem* item = 
					window->tableWidgetObtain->item(i,0);
		if(item->checkState() == Qt::Checked)
		{	
			aux << window->tableWidgetObtain->item(i,c-1)->text();
		}				
	}

	emit download(id,aux);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::itemChangedTableDirs(QTableWidgetItem * item)
{
	if(!isVisible())
	{
		return;
	}

	int r = window->tableWidgetDirs->rowCount();
	int c = window->tableWidgetDirs->columnCount();

	if(!item->column())
	{
		int i = 0;
		bool cond = false;
		for(int i=0;i<r;i++)
		{					
			QTableWidgetItem* item = 
						window->tableWidgetDirs->item(i,0);
			if(item->checkState() == Qt::Checked)
			{		
				cond = true;
			}				
		}

		window->pushButtonRemove->setEnabled(cond);
	}
	else
	{
		QStringList list;
		int r = item->row();
		int c = window->tableWidgetDirs->columnCount();
		for(int i=1;i<c;i++)
		{
			list << window->tableWidgetDirs->item(r,i)->text();
		}

		emit editPath(list);
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::currentIndexChangedComboBoxTypeDir(const QString & t)
{
	int c = window->tableWidgetDirs->columnCount();
	for(int i=0;i<c;i++)
	{
		window->tableWidgetDirs->showColumn(i);
	}

	window->tableWidgetDirs->clear();
	window->tableWidgetDirs->setWordWrap(true);
	window->tableWidgetDirs->setSelectionBehavior(QAbstractItemView::SelectRows);	
	window->tableWidgetDirs->verticalHeader()->hide();	
	window->tableWidgetDirs->setRowCount(0);
	window->tableWidgetDirs->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if(spinBoxDelegate)
	{
		delete spinBoxDelegate;
		spinBoxDelegate = 0;
	}	

	if(spinBoxDelegatePort)
	{
		delete spinBoxDelegatePort;
		spinBoxDelegatePort = 0;
	}	

	switch(idType(t))
	{
		case 0:
		{			
			window->tableWidgetDirs->horizontalHeader()->hide();
			window->pushButtonAdd->setEnabled(false);
			window->pushButtonRemove->setEnabled(false);
		}
		break;
		case 1:
		{
			QStringList list;
			list << tr("");
			list << tr("pasta");
			list << tr("monitoramento (dias)");
			list << tr("TYPE");
			list << tr("ID");
			window->tableWidgetDirs->setColumnCount(list.size());
			window->tableWidgetDirs->setHorizontalHeaderLabels(list);
			window->tableWidgetDirs->hideColumn(list.size()-2);
			window->tableWidgetDirs->hideColumn(list.size()-1);
			setTableWidgetDirs(1);	
			window->tableWidgetDirs->horizontalHeader()->show();
			window->pushButtonAdd->setEnabled(true);
			window->pushButtonRemove->setEnabled(false);
			
			spinBoxDelegate = new SpinBoxDelegate(this);			
			spinBoxDelegate->setRange(0,30);
			
			window->tableWidgetDirs->setItemDelegateForColumn(2,spinBoxDelegate);

		}
		break;
		case 2:
		{
			QStringList list;
			list << tr("");
			list << tr("endereço");
			list << tr("pasta");
			list << tr("porta");
			list << tr("usuário");
			list << tr("senha");
			list << tr("monitoramento (dias)");
			list << tr("TYPE");
			list << tr("ID");
			window->tableWidgetDirs->setColumnCount(list.size());
			window->tableWidgetDirs->setHorizontalHeaderLabels(list);
			window->tableWidgetDirs->hideColumn(list.size()-2);
			window->tableWidgetDirs->hideColumn(list.size()-1);
			setTableWidgetDirs(2);
			window->tableWidgetDirs->horizontalHeader()->show();
			window->pushButtonAdd->setEnabled(true);
			window->pushButtonRemove->setEnabled(false);

			spinBoxDelegatePort = new SpinBoxDelegate(this);			
			spinBoxDelegatePort->setRange(0,10000);
			
			window->tableWidgetDirs->setItemDelegateForColumn(3,spinBoxDelegatePort);

			spinBoxDelegate = new SpinBoxDelegate(this);			
			spinBoxDelegate->setRange(0,30);					
			
			window->tableWidgetDirs->setItemDelegateForColumn(6,spinBoxDelegate);

		}
		break;
	}	

	window->tableWidgetDirs->resizeColumnToContents(0);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::currentIndexChangedComboBoxTypeDirObtain(const QString & v)
{
	int type = idType(v);
	window->comboBoxLocal->clear();

	window->comboBoxLocal->addItem("");

	foreach(const QStringList& v,paths)
	{
		int s = v.size();
		if(v[s-2].toInt() == type)
		{
			QVariant aux; 
			aux = v[s-1].toInt();
			window->comboBoxLocal->addItem(v[0],aux);
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::currentIndexChangedComboBoxLocalObtain(const QString & t)
{	
	int id =  window->comboBoxLocal->currentIndex();
	int idPath = window->comboBoxLocal->itemData(id).toInt();
	emit currentPathObtain(idPath);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::setTableWidgetDirs(int type)
{
	disconnect
	(
		window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
		this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
	);

	int r = 0;
	foreach(QStringList list,paths)
	{
		int s = list.size();
		if(list[s-2].toInt() == type)
		{
			window->tableWidgetDirs->insertRow(r);
			
			QTableWidgetItem *newItem = new QTableWidgetItem("");
			newItem->setCheckState(Qt::Unchecked);
			window->tableWidgetDirs->setItem(r, 0, newItem);

			for(int i=0;i<s;i++)
			{
				QTableWidgetItem *newItem = new QTableWidgetItem("");				
				window->tableWidgetDirs->setItem(r, i+1, newItem);
				if(i+1==5)
				{
					QLineEdit *aux = new QLineEdit();
					aux->setEchoMode(QLineEdit::Password);
					aux->setText(list[i]);
					window->tableWidgetDirs->setCellWidget(r,i+1,aux);
				}
				else
				{
					newItem->setText(list[i]);
				}
			}
			r++;
		}

	}

	connect
	(
		window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
		this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
	);
	
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::addItems(int r)
{
	QTableWidgetItem *newItem = new QTableWidgetItem("");
	newItem->setCheckState(Qt::Unchecked);
	window->tableWidgetDirs->setItem(r, 0, newItem);
	int c = window->tableWidgetDirs->columnCount();

	for(int i=0;i<c;i++)
	{
		QTableWidgetItem *newItem = new QTableWidgetItem("");				
		window->tableWidgetDirs->setItem(r, i+1, newItem);
		if(i+1==5)
		{
			QLineEdit *aux = new QLineEdit();
			aux->setEchoMode(QLineEdit::Password);
			aux->setText("");
			window->tableWidgetDirs->setCellWidget(r,i+1,aux);
		}
		else
		{
			newItem->setText("");
		}
	}	
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::itemDoubleClickedTableDirs(QTableWidgetItem * item)
{
	int c = item->column();
	int r = item->row();

	int s = window->tableWidgetDirs->columnCount();
	int t = window->tableWidgetDirs->item(r,s-2)->text().toInt();

	switch(t)
	{
		case 0:
		{

		}
		break;
		case 1:
		{
			switch(c)
			{
				case 1:
				{						
					QString dir = 
						QFileDialog::getExistingDirectory(this, tr("Escolha um diretório"),
						item->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);					
					if(dir.size())
						item->setText(dir);
				}
				break;
				case 2:
				{
					window->tableWidgetDirs->editItem(item);					
				}
				break;
			}
		}
		break;
		case 2:
		{
			switch(c)
			{
				case 1:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
				case 2:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
				case 3:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
				case 4:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
				case 5:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
				case 6:
				{
					window->tableWidgetDirs->editItem(item);
				}
				break;
			}
		}
		break;
	}

}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::itemChangedTableDirObtain(QTableWidgetItem * item)
{
	int rc = window->tableWidgetObtain->rowCount();

	bool cond = false;
	for(int i=0;i<rc;i++)
	{		
		QTableWidgetItem * item = window->tableWidgetObtain->item(i,0);
		if(item->checkState() == Qt::Checked)
		{
			cond = true;
			break;
		}		
	}

	window->pushButtonDownloadObtain->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::connects()
{
	connect
	(
		window->pushButtonAdd, SIGNAL(clicked(bool)), 
		this, SLOT(addPath(bool))
	);

	//-----

	connect
	(
		window->pushButtonRemove, SIGNAL(clicked(bool)), 
		this, SLOT(removePath(bool))
	);

	//-----

	connect
	(
		window->pushButtonDownloadObtain, SIGNAL(clicked(bool)), 
		this, SLOT(download(bool))
	);

	//-----

	connect
	(
		window->tableWidgetDirs, SIGNAL(itemChanged(QTableWidgetItem *)), 
		this, SLOT(itemChangedTableDirs(QTableWidgetItem *))
	);

	connect
	(
		window->tableWidgetDirs, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), 
		this, SLOT(itemDoubleClickedTableDirs(QTableWidgetItem *))
	);

	//-----

	connect
	(
		window->comboBoxTypeDir, SIGNAL(currentIndexChanged(const QString &)), 
		this, SLOT(currentIndexChangedComboBoxTypeDir(const QString &))
	);

	//-----

	connect
	(
		window->comboBoxTypeDir2, SIGNAL(currentIndexChanged(const QString &)), 
		this, SLOT(currentIndexChangedComboBoxTypeDirObtain(const QString &))
	);

	//-----

	connect
	(
		window->comboBoxLocal, SIGNAL(currentIndexChanged(const QString &)), 
		this, SLOT(currentIndexChangedComboBoxLocalObtain(const QString &))
	);
}

//-----------------------------------------------------------------------------
void ConfigTerrainDir::setInfoObtain(const QList<QStringList >& info)
{
	disconnect(
		window->tableWidgetObtain,SIGNAL(itemChanged(QTableWidgetItem *)),
		this,SLOT(itemChangedTableDirObtain(QTableWidgetItem *)));

	window->tableWidgetObtain->clear();
	window->tableWidgetObtain->verticalHeader()->hide();	
	window->tableWidgetObtain->setSelectionBehavior(QAbstractItemView::SelectRows);	
	window->tableWidgetObtain->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList list;
	list << tr("");
	list << tr("Nome");
	list << tr("Data");
	list << tr("Registro no sistema");
	list << tr("Notas");
	list << tr("ID");
	window->tableWidgetObtain->setColumnCount(list.size());

	window->tableWidgetObtain->setHorizontalHeaderLabels(list);	

	window->pushButtonDownloadObtain->setEnabled(false);	

	int r = 0;
	int c = window->tableWidgetObtain->columnCount();

	foreach(const QStringList& v, info)
	{
		window->tableWidgetObtain->insertRow(r);

		QTableWidgetItem *newItem = new QTableWidgetItem("");
		newItem->setCheckState(Qt::Unchecked);
		window->tableWidgetObtain->setItem(r, 0, newItem);		
		for(int i=1;i<c;i++)
		{
			QTableWidgetItem *newItem = new QTableWidgetItem("");
			newItem->setText(v[i-1]);
			window->tableWidgetObtain->setItem(r, i, newItem);
		}
		r++;
	}

	window->tableWidgetObtain->resizeColumnToContents(0);
	window->tableWidgetObtain->resizeColumnToContents(2);
	window->tableWidgetObtain->resizeColumnToContents(3);
	window->tableWidgetObtain->hideColumn(list.size()-1);
	connect(
		window->tableWidgetObtain,SIGNAL(itemChanged(QTableWidgetItem *)),
		this,SLOT(itemChangedTableDirObtain(QTableWidgetItem *)));
}
