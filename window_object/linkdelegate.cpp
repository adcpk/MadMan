#include "linkdelegate.h"
#include <QDebug>
#include <QLineEdit>
#include "widgetlink.h"
#include "widgetobject.h"

LinkDelegate::LinkDelegate(QObject* parent):
	QStyledItemDelegate (parent)
{
}

QWidget* LinkDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QWidget* w;
	if(!index.data(Qt::UserRole).toString().isEmpty())
	{
		w = new WidgetLink(parent);
		WidgetObject* wo = qobject_cast<WidgetObject*>(parent->parent()->parent());
        connect(qobject_cast<WidgetLink*>(w), &WidgetLink::db_key,
                wo, &WidgetObject::db_key);
        connect(qobject_cast<WidgetLink*>(w), &WidgetLink::cdb_key,
                wo, &WidgetObject::cdb_key);
	}
	else
	{
		w = new QLineEdit(parent);
	}
	return w;
}

void LinkDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
	if(!index.data(Qt::UserRole).toString().isEmpty())
	{
		WidgetLink* w = static_cast<WidgetLink*>(editor);
		w->set_types(index.data(Qt::UserRole).toString());
		w->set_names(index.data().toString());
	}
	else
	{
		QLineEdit* w = static_cast<QLineEdit*>(editor);
		w->setText(index.data().toString());
	}
}

void LinkDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

void LinkDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex &index) const
{
	if(!index.data(Qt::UserRole).toString().isEmpty())
	{
		WidgetLink* w = static_cast<WidgetLink*>(editor);
        model->setData(index, w->get_names().join(","));
	}
	else
	{
		QLineEdit* w = static_cast<QLineEdit*>(editor);
		model->setData(index, w->text());
	}
}
