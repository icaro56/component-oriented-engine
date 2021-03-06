#ifndef SpinBoxDelegate_H
#define SpinBoxDelegate_H
 
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    SpinBoxDelegate(QObject *parent = 0);

	void setRange(unsigned int,unsigned int);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;
		 
private:

   unsigned int max, min;
};

#endif // SpinBoxDelegate_H
