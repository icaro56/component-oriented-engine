#include "ColorDelegate.h"
#include "moc_ColorDelegate.cpp"

#include <QtGui>
#include <QtWidgets>

ColorDelegate::ColorDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *ColorDelegate::createEditor(QWidget *parent, const
        QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const
{
    QColorDialog *editor = new QColorDialog(parent);
    return editor;
}

void ColorDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
}

void ColorDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    QColorDialog *colorDialog = static_cast<QColorDialog*>(editor);
    QColor value = colorDialog->currentColor();
    model->setData(index, value, Qt::BackgroundRole);
}

void ColorDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

