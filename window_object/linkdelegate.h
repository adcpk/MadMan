#ifndef LINKDELEGATE_H
#define LINKDELEGATE_H

#include <QStyledItemDelegate>

class LinkDelegate : public QStyledItemDelegate
{
	Q_OBJECT
  public:
	LinkDelegate(QObject* parent = nullptr);
  protected:
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget* editor, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex &index) const override;
};

#endif // LINKDELEGATE_H
