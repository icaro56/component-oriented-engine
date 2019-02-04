#ifndef ColorDelegate_H
#define ColorDelegate_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QDoubleSpinBox>

class ColorDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ColorDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QColor color;
};


 #endif // ColorDelegate_H
