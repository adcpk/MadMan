#include "keyfilterproxymodel.h"
#include <QStandardItemModel>
#include <QDebug>

KeyFilterProxyModel::KeyFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{}

void KeyFilterProxyModel::set_ishowdef(bool is)
{
    ishowdef = is;
    invalidate();
}

bool KeyFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool show = sourceModel()->index(source_row,0,source_parent).data(Qt::UserRole).toBool();
    bool set = sourceModel()->index(source_row,1,source_parent).data(Qt::CheckStateRole)==Qt::Checked;
    return (ishowdef?true:set) &&
            show &&
            QSortFilterProxyModel::filterAcceptsRow(source_row,source_parent);
}
