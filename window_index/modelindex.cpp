#include "modelindex.h"
#include "container/data_index.h"
#include <QDebug>

ModelIndex::ModelIndex(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int ModelIndex::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()||linkdata==nullptr)
        return 0;

    return linkdata->getData().size();
}

int ModelIndex::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant ModelIndex::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical || role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return "Key";
        case 1:
            return "Name";
        default:
            return "";
        }
    }
    else
    {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
}

QVariant ModelIndex::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()||linkdata==nullptr)
        return QVariant();

    switch (index.column()) {
    case 0: {
        switch (role) {
        case Qt::DisplayRole:
            return linkdata->getData().keys()[index.row()];
        case Qt::UserRole:
            return linkdata->getData().values()[index.row()];
        default:
            return QVariant();
        }
    }
    case 1: {
        switch (role) {
        case Qt::DisplayRole:
            return QVariant();
        default:
            return QVariant();
        }
    }
    default:
        return QVariant();
    }
}

void ModelIndex::setLinkData(Data_Index *v)
{
    beginResetModel();
    linkdata=v;
    endResetModel();
}
