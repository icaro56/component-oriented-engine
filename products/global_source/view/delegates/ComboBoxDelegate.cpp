#include "ComboBoxDelegate.h"
#include "moc_ComboBoxDelegate.cpp"

#include <QtGui>
#include <QtWidgets>

ComboBoxDelegate::ComboBoxDelegate(const QStringList& l,
        QObject *parent) : QItemDelegate(parent), list(l)
{

}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const
        QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(list);
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->clear();
    QStringList auxlist = list;
    int s = auxlist.size();
    for(int i=0;i<s;i++)
    {
        if(value == auxlist[i])
        {
            auxlist.removeAt(i);
            auxlist.push_front(value);
            comboBox->addItems(auxlist);
            return;
        }
    }
    comboBox->addItems(list);
}

void ComboBoxDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

