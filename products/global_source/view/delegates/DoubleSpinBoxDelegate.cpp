#include "DoubleSpinBoxDelegate.h"
#include "moc_DoubleSpinBoxDelegate.cpp"

#include <QtGui>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(double max, double min,
        QObject *parent) : QItemDelegate(parent), max(max), min(min)
{

}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const
        QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(min);
    editor->setMaximum(max);

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

