#include "ConfigTerrain.h"
#include "ui_ConfigTerrain.h"
//#include "moc_ConfigTerrain.cpp"

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
#include <QDate>
#include <QTextCharFormat>
#include <QCheckBox>

//-----------------------------------------------------------------------------
ConfigTerrain::ConfigTerrain(QWidget *parent)
    : QDialog(parent)
{
	idCurrent = "-";
	lastDate = QDate();

	window = new Ui::ConfigTerrain();

    window->setupUi(this);

	//Não escreveremos mais o terreno atual na barra de título
	//setWindowTitle(tr("Terreno Atual: " + terrainName));
		
	//colocando ícones
	
	window->butFile->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_abrir.png"));
	window->butFtpConn->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_abrir_nuvem.png"));
	window->butDelete->setIcon(QIcon( GlobalConfig::ICONPATH +"ico_excluir_tudo.png"));

	connects();
}

//-----------------------------------------------------------------------------
ConfigTerrain::~ConfigTerrain()
{
	
}

//-----------------------------------------------------------------------------
void ConfigTerrain::init()
{
	
}

//-----------------------------------------------------------------------------
void ConfigTerrain::setDataInfo(const QString& c,const QHash<int,QStringList>& data)
{
	window->butDelete->setEnabled(false);
	window->butExport->setEnabled(false);
	window->butConfirm->setEnabled(false);

	QFont f = window->lblTerrainSelected->font();
	f.setBold(true);
	window->lblTerrainSelected->setFont(f);
	window->lblTerrainSelected->setText(tr("Nenhum terreno foi carregado"));
	window->lblTerrainSelected->wordWrap();

	QTextCharFormat tcf = window->calendar->dateTextFormat(QDate());
	QList<QDate> datelist = window->calendar->dateTextFormat().keys();
	foreach(QDate d, datelist)
	{
		window->calendar->setDateTextFormat(d,tcf);
	}

	window->treeDate->clear();
	window->tableDescription->setWordWrap(true);
	window->tableDescription->clear();	
	window->tableDescription->setSelectionBehavior(QAbstractItemView::SelectRows);	
	QStringList list;
	list << tr("");	
	list << tr("Nome");
	list << tr("Registro no Sistema");
	list << tr("Notas");
	list << tr("DATE");
	list << tr("ID");
	window->tableDescription->setColumnCount(list.size());
	window->tableDescription->setHorizontalHeaderLabels(list);
	window->tableDescription->verticalHeader()->hide();	

	window->tableDescription->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	window->tableDescription->setRowCount(0);
    
	window->tableDescription->hideColumn(list.size()-2);
	window->tableDescription->hideColumn(list.size()-1);

	int i = 0;
	QHash<int, QStringList>::const_iterator it;
	for(it=data.constBegin();it!=data.constEnd();it++)
	{
		QStringList list = it.value();
		insertDataTreeDate(list[list.size()-2]);
		insertDataCalendar(list[list.size()-2]);
		insertDataTableDescription(i,list);
		i++;
	}		
	
	int s = window->tableDescription->rowCount();
	for(int i=0;i<s;i++)
	{
		window->tableDescription->hideRow(i);
		window->tableDescription->item(i,0)->setCheckState(Qt::Unchecked);
	}
	
	setCurrentTerrain(c);
}

//-----------------------------------------------------------------------------
void ConfigTerrain::insertDataTableDescription(int ir,const QStringList& list)
{
	window->tableDescription->insertRow(ir);
	int s = list.size();
	for(int i=0;i<s;i++)
	{
		QTableWidgetItem *newItem = new QTableWidgetItem("");
		if(i != 0)
		{
			newItem->setText(list[i]);
			window->tableDescription->setItem(ir, i, newItem);
		}
		else
		{
			newItem->setCheckState(Qt::Unchecked);
			window->tableDescription->setItem(ir, i, newItem);
		}
	}
	
}

//-----------------------------------------------------------------------------
void ConfigTerrain::insertDataCalendar(const QString& v)
{
	QDate date = QDate::fromString(v,"dd/MM/yyyy");

	QTextCharFormat dtcf = window->calendar->dateTextFormat(date);
    QFont font = dtcf.font();
    font.setBold(true);
    font.setPointSize(12);
    //font.setItalic(true);
    dtcf.setFont(font);
    dtcf.setForeground(Qt::blue);

	window->calendar->setDateTextFormat(date,dtcf);
}

//-----------------------------------------------------------------------------
void ConfigTerrain::currentTexDataCalendar(const QDate& date)
{
	QTextCharFormat dtcf = window->calendar->dateTextFormat(date);
    QFont font = dtcf.font();
    font.setBold(true);
    font.setPointSize(12);
    //font.setItalic(true);
    dtcf.setFont(font);
    dtcf.setForeground(Qt::green);

	window->calendar->setDateTextFormat(date,dtcf);
}

//-----------------------------------------------------------------------------
void ConfigTerrain::insertDataTreeDate(const QString& v)
{
	QTreeWidgetItem *year = 0, *month = 0;

	QDate date = QDate::fromString(v,"dd/MM/yyyy");
		
	QString yearTxt = date.toString("yyyy");

	QList<QTreeWidgetItem *> exist = 
		window->treeDate->findItems(yearTxt, Qt::MatchExactly);
    if (exist.isEmpty())
    {			
		year = new QTreeWidgetItem(window->treeDate);
		year->setText(0, yearTxt);
    }
    else
    {
        year = exist.at(0);
    }

    year->setExpanded(true);
		
	QString strMonth = date.toString("MMMM");

	bool cond = true;
	int s = year->childCount();
	for(int i=0;i<s;i++)
	{
		QTreeWidgetItem * item = year->child(i);
		if(strMonth == item->text(0))
		{
			cond = false;
			break;
		}
	}

	if(cond)
	{
		month = new QTreeWidgetItem(year);
		month->setText(0, strMonth);
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrain::setPageCalendar(const QString& y,const QString& m)
{
	int year = y.toInt();
	int month = 0;
	for(int i=1;i<13;i++)
	{
		if(m == QDate::longMonthName(i))
		{
			window->calendar->setCurrentPage(year,i);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrain::connects()
{
	//-----------

	connect(window->butFile,SIGNAL(clicked(bool)),this,SIGNAL(showDir(bool)));

	//-----------

	connect(window->butClose,SIGNAL(clicked(bool)),
		this,SLOT(setVisible(bool)));

	//-----------

	connect(window->butDelete,SIGNAL(clicked(bool)),
		this,SLOT(removeTerrain(bool)));

	//-----------

	connect(window->butExport,SIGNAL(clicked(bool)),
		this,SLOT(exportTerrain(bool)));

	//-----------

	connect(window->butConfirm,SIGNAL(clicked(bool)),
		this,SLOT(loadTerrain(bool)));

	//-----------

	connect(window->butFtpConn,SIGNAL(clicked(bool)),this,SIGNAL(showFtp(bool)));	

	//-----------

	connect(
		window->calendar,SIGNAL(clicked(const QDate&)),
		this,SLOT(clickedCalendar(const QDate&)));

	connect(
		window->calendar,SIGNAL(currentPageChanged(int, int)),
		this,SLOT(currentPageChangedCalendar(int, int)));

	//-----------

	connect(
		window->tableDescription,SIGNAL(itemChanged(QTableWidgetItem *)),
		this,SLOT(itemChangedTableDescription(QTableWidgetItem *)));

	connect(
		window->tableDescription,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
		this,SLOT(itemDoubleClickedTableDescription(QTableWidgetItem *)));

	//-----------

	connect(
		window->treeDate,SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem *)),
		this,SLOT(changedTreeDate(QTreeWidgetItem*, QTreeWidgetItem *)));
}

//-----------------------------------------------------------------------------
bool ConfigTerrain::event(QEvent *e)
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

//-----------------------------------------------------------------------------
void ConfigTerrain::removeTerrain(bool)
{
	QStringList r;
	
	int rc = window->tableDescription->rowCount();
	int cc = window->tableDescription->columnCount();

	for(int i=0;i<rc;i++)
	{
		if(!window->tableDescription->isColumnHidden(i))
		{
			QTableWidgetItem * item = window->tableDescription->item(i,0);
			if(item->checkState() == Qt::Checked)
			{
				item = window->tableDescription->item(i,cc-1);
				r << item->text();
			}
		}
	}

	emit removeTerrain(r);
}

//-----------------------------------------------------------------------------
void ConfigTerrain::exportTerrain(bool)
{
	QStringList r;
	int rc = window->tableDescription->rowCount();
	int cc = window->tableDescription->columnCount();

	for(int i=0;i<rc;i++)
	{
		if(!window->tableDescription->isColumnHidden(i))
		{
			QTableWidgetItem * item = window->tableDescription->item(i,0);
			if(item->checkState() == Qt::Checked)
			{
				item = window->tableDescription->item(i,cc-1);
				r << item->text();
			}
		}
	}
	emit exportTerrain(r);

	for(int i=0;i<rc;i++)
	{
		if(!window->tableDescription->isColumnHidden(i))
		{
			QTableWidgetItem * item = window->tableDescription->item(i,0);
			if(item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrain::loadTerrain(bool)
{
	QStringList r;	
	

	int rc = window->tableDescription->rowCount();
	int cc = window->tableDescription->columnCount();

	for(int i=0;i<rc;i++)
	{
		if(!window->tableDescription->isColumnHidden(i))
		{
			QTableWidgetItem * item = window->tableDescription->item(i,0);
			if(item->checkState() == Qt::Checked)
			{
				item = window->tableDescription->item(i,cc-1);
				r << item->text();
			}
		}
	}

	emit loadTerrain(r);
}

//-----------------------------------------------------------------------------
void ConfigTerrain::changedTreeDate(QTreeWidgetItem* item, QTreeWidgetItem *)
{
	if(item)
	{
		QTreeWidgetItem* parent = item->parent(); 
		if(parent)
		{		
			setPageCalendar(parent->text(0),item->text(0));
		}
	}		
}

//-----------------------------------------------------------------------------
void ConfigTerrain::clickedCalendar(const QDate & date)
{
	if(lastDate != date)
	{
		lastDate = date;
		updateTableDescription(date);
	}	
}

//-----------------------------------------------------------------------------
void ConfigTerrain::updateTableDescription(const QDate & date)
{
	window->butDelete->setEnabled(false);
	window->butExport->setEnabled(false);
	window->butConfirm->setEnabled(false);

	int s = window->tableDescription->rowCount();
	for(int i=0;i<s;i++)
	{
		window->tableDescription->hideRow(i);
		window->tableDescription->item(i,0)->setCheckState(Qt::Unchecked);
	}

	QString aux = date.toString("dd/MM/yyyy");
	QList<QTableWidgetItem *> list  = 
		window->tableDescription->findItems(aux,Qt::MatchExactly);

	s = window->tableDescription->columnCount() - 2;

	bool cond = false;
	foreach(QTableWidgetItem * v,list)
	{
		if((v->column() == s))
		{
			cond = true;
			window->tableDescription->showRow(v->row());
		}
	}

	if(cond)
	{
		setCurrentTreeDate(date);
	}

	window->tableDescription->resizeColumnToContents(0);
	window->tableDescription->resizeColumnToContents(1);
	window->tableDescription->resizeColumnToContents(2);
	window->tableDescription->resizeRowsToContents();
}

//-----------------------------------------------------------------------------
void ConfigTerrain::setCurrentTreeDate(const QDate & date)
{
	QString r = date.toString("MMMM");

	QString yearTxt = date.toString("yyyy");
	QList<QTreeWidgetItem *> exist = 
		window->treeDate->findItems(yearTxt, Qt::MatchExactly);	

	if(!exist.isEmpty())
	{
		QTreeWidgetItem * parent = exist[0];
		int s = parent->childCount();
		for(int i=0;i<s;i++)
		{
			QTreeWidgetItem * item = parent->child(i);
			QString aux = item->text(0);
			if(aux == r)
			{
				window->treeDate->setCurrentItem(item);				
			}
		} 
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrain::setCurrentTerrain(const QString& id)
{
	idCurrent = id;

	QList<QTableWidgetItem *> list  = 
		window->tableDescription->findItems(id,Qt::MatchExactly);

	int s = window->tableDescription->columnCount();

	foreach(QTableWidgetItem * v,list)
	{
		if((v->column() == s-1))
		{
			for(int i=0;i<s;i++ )
			{
				window->tableDescription->item(v->row(),i)->setBackgroundColor(Qt::green);
			}

			QString aux = window->tableDescription->item(v->row(),s-2)->text();
			QDate daux = QDate::fromString(aux,"dd/MM/yyyy");
			window->calendar->setSelectedDate(daux);
			updateTableDescription(daux);
			currentTexDataCalendar(daux);
			QFont f = window->lblTerrainSelected->font();
			f.setBold(true);
			window->lblTerrainSelected->setFont(f);
			window->lblTerrainSelected->setText(aux + ": " +
				window->tableDescription->item(v->row(),1)->text());
			window->lblTerrainSelected->wordWrap();
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrain::itemChangedTableDescription(QTableWidgetItem * item)
{
	if(!isVisible())
	{
		return;
	}

	int s = window->tableDescription->rowCount();
	int c = window->tableDescription->columnCount();

	switch(item->column())
	{
		case 0:
		{
			int r = 0;
			bool condLoad = false;
			for(int i=0;i<s;i++)
			{				
				bool cond = !window->tableDescription->isRowHidden(i);
				if(cond)
				{
					QTableWidgetItem* item = 
								window->tableDescription->item(i,0);
					if(item->checkState() == Qt::Checked)
					{						
						if(window->tableDescription->item(i,c-1)->text()
							== idCurrent)
						{
							condLoad = true;
						}
						r++;
					}
				}
			}
			
			if(!r)
			{
				window->butDelete->setEnabled(false);
				window->butExport->setEnabled(false);
				window->butConfirm->setEnabled(false);
			}
			else if(r == 1)
			{
				window->butDelete->setEnabled(true);				
				window->butExport->setEnabled(true);				
				window->butConfirm->setEnabled(!condLoad);
			}
			else
			{	
				window->butDelete->setEnabled(true);
				window->butExport->setEnabled(true);
				window->butConfirm->setEnabled(false);
			}
		}
		break;
		case 1:
		{		
			QTableWidgetItem * auxItem = window->tableDescription->item(item->row(),c-1);
			if(auxItem)
			{
				int id = auxItem->text().toInt();
				emit nameChanged(id,item->text());					
			}
		}
		break;
		case 2:
		{
			QTableWidgetItem * auxItem = window->tableDescription->item(item->row(),c-1);
			if(auxItem)
			{
				int id = auxItem->text().toInt();
				emit descriptionChanged(id,item->text());
			}
		}
		break;
	};
}

//-----------------------------------------------------------------------------
void ConfigTerrain::currentPageChangedCalendar(int year, int month)
{
	window->butDelete->setEnabled(false);
	window->butExport->setEnabled(false);
	window->butConfirm->setEnabled(false);

	int s = window->tableDescription->rowCount();
	for(int i=0;i<s;i++)
	{
		window->tableDescription->hideRow(i);
		window->tableDescription->item(i,0)->setCheckState(Qt::Unchecked);
	}

	lastDate = QDate();
}

//-----------------------------------------------------------------------------
void ConfigTerrain::itemDoubleClickedTableDescription(QTableWidgetItem * item)
{
	if(!item)
	{
		return;
	}

	int r = item->row();
	int c = item->column();
	
	switch(c)
	{
		case 1:
		{
			window->tableDescription->editItem(item);
		}
		break;
		case 3:
		{
			window->tableDescription->editItem(item);
		}
		break;
	}
}	