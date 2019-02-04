#ifndef ComboBoxDelegate_H
#define ComboBoxDelegate_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QDoubleSpinBox>

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboBoxDelegate(const QStringList&, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QStringList list;
};

#endif // ComboBoxDelegate_H
