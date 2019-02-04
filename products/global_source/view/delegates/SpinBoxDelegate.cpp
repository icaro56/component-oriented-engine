#include "SpinBoxDelegate.h"
#include "moc_SpinBoxDelegate.cpp"

#include <QtGui>

//-----------------------------------------------------------------------------
SpinBoxDelegate::SpinBoxDelegate(QObject *parent) 
	: QItemDelegate(parent)
{
	min = 0;
	max = 0;
}

//-----------------------------------------------------------------------------
void SpinBoxDelegate::setRange(unsigned int v1,unsigned int v2)
{
	min = v1;
	max = v2;
}

//-----------------------------------------------------------------------------
QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const
        QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(min);
    editor->setMaximum(max);

    return editor;
}

//-----------------------------------------------------------------------------
void SpinBoxDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

//-----------------------------------------------------------------------------
void SpinBoxDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

//-----------------------------------------------------------------------------
void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

