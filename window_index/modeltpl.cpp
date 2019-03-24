#include "modeltpl.h"
#include "container/data_tpl.h"
#include <QDebug>

ModelTpl::ModelTpl(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int ModelTpl::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()||linkdata==nullptr)
        return 0;

    return linkdata->getData().size();
}

int ModelTpl::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()||linkdata==nullptr)
        return 0;

    return 4;
}

QVariant ModelTpl::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Vertical||role==Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "s";
        case 1:
            return "Key";
        case 2:
            return "Link";
        case 3:
            return "Default";
        default:
            return "";
        }
    }
    else {
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}

Qt::ItemFlags ModelTpl::flags(const QModelIndex &index) const
{
    if (!index.isValid()||linkdata==nullptr)
        return Qt::NoItemFlags;

    switch (index.column()) {
    case 0:
        return Qt::ItemIsEditable|QAbstractTableModel::flags(index);
    case 1:
        return Qt::ItemIsEditable|Qt::ItemIsUserCheckable|QAbstractTableModel::flags(index);
    case 2:
        return Qt::ItemIsEditable|QAbstractTableModel::flags(index);
    case 3:
        return Qt::ItemIsEditable|QAbstractTableModel::flags(index);
    default:
        return Qt::NoItemFlags;
    }
}

QVariant ModelTpl::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()||linkdata==nullptr)
        return QVariant();

    switch (index.column()) {
    case 0: {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return linkdata->getData()[index.row()].sort;
        default:
            return QVariant();
        }
    }
    case 1: {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return linkdata->getData()[index.row()].key;
        case Qt::CheckStateRole:
            return linkdata->getData()[index.row()].isshow?Qt::Checked:Qt::Unchecked;
        case Qt::UserRole:
            return linkdata->getData()[index.row()].sort;
        default:
            return QVariant();
        }
    }
    case 2: {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return linkdata->getData()[index.row()].link.join(",");
        default:
            return QVariant();
        }
    }
    case 3: {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return linkdata->getData()[index.row()].defvalue;
        default:
            return QVariant();
        }
    }
    default:
        return QVariant();
    }
}

bool ModelTpl::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int mi,ma,dis,old;
    if (data(index, role) != value) {
        switch (index.column()) {
        case 0: {
            switch (role) {
            case Qt::EditRole:
                old=index.data().toInt();
                dis=value.toInt();
                if(dis<1||dis>rowCount()+1) {return false;}
                mi=index.row();
                ma=index.row();
                if(dis<old) {
                    linkdata->setItem(index.row(),IK::sort,0);
                    for(int i=0;i!=rowCount();++i) {
                        int is = linkdata->getData()[i].sort;
                        if(is>=dis && is<old) {
                            linkdata->setItem(i,IK::sort,is+1);
                            mi = i<mi?i:mi;
                            ma = i>ma?i:ma;
                        }
                    }
                    linkdata->setItem(index.row(),IK::sort,value);
                }
                else {
                    linkdata->setItem(index.row(),IK::sort,rowCount()+2);
                    for(int i=0;i!=rowCount();++i) {
                        int is = linkdata->getData()[i].sort;
                        if(is>old && is<=dis) {
                            linkdata->setItem(i,IK::sort,is-1);
                            mi = i<mi?i:mi;
                            ma = i>ma?i:ma;
                        }
                    }
                    linkdata->setItem(index.row(),IK::sort,value);
                }
                emit dataChanged(this->index(mi,0), this->index(ma,0), QVector<int>() << role);
                return true;
            default:
                return false;
            }
        }break;
        case 1: {
            switch (role) {
            case Qt::EditRole:
                linkdata->setItem(index.row(),IK::key,value);
                break;
            case Qt::CheckStateRole:
                linkdata->setItem(index.row(),IK::isshow,value);
                break;
            default:
                return false;
            }
        }break;
        case 2: {
            switch (role) {
            case Qt::EditRole:
                linkdata->setItem(index.row(),IK::link,value.toString().split(",",QString::SkipEmptyParts));
                break;
            default:
                return false;
            }
        }break;
        case 3: {
            switch (role) {
            case Qt::EditRole:
                linkdata->setItem(index.row(),IK::defvalue,value);
                break;
            default:
                return false;
            }
        }break;
        default:
            return false;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

bool ModelTpl::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row+count-1);
    for (int i=0;i!=count;++i) {
        linkdata->removeItem(row);
    }
    endRemoveRows();
    return true;
}

bool ModelTpl::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    for (int i=0;i!=count;++i) {
        Item_KeyValue temp;
        temp.sort = rowCount()+1;
        linkdata->addItem(row,temp);
    }
    endInsertRows();
    return true;
}

void ModelTpl::setLinkData(Data_Tpl *v)
{
    beginResetModel();
    linkdata = v;
    endResetModel();
}
