#ifndef DoubleSpinBoxDelegate_H
#define DoubleSpinBoxDelegate_H
 
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QDoubleSpinBox>

class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    DoubleSpinBoxDelegate(double max = 100.0, double min = 0.0, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;
		 
private:
    double max, min;
};

#endif // DoubleSpinBoxDelegate_H
