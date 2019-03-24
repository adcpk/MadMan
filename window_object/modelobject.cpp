#include "modelobject.h"
#include <QDebug>
#include "container/data_object.h"

ModelObject::ModelObject(QObject* parent)
	: QAbstractTableModel(parent)
{
}

int ModelObject::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()||linkdata==nullptr)
        return 0;

    return linkdata->getData().size();
}

int ModelObject::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()||linkdata==nullptr)
        return 0;

    return 2;
}

QVariant ModelObject::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Vertical || role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return "Key";
		case 1:
			return "Value";
		default:
			return "";
		}
	}
	else
	{
		return QAbstractTableModel::headerData(section, orientation, role);
	}
}

Qt::ItemFlags ModelObject::flags(const QModelIndex &index) const
{
    if (!index.isValid()||linkdata==nullptr)
        return Qt::NoItemFlags;

	switch (index.column())
	{
	case 0:
		return QAbstractTableModel::flags(index);
	case 1:
		return Qt::ItemIsUserCheckable |
		       Qt::ItemIsEditable |
		       QAbstractTableModel::flags(index);
	default:
		return Qt::NoItemFlags;
	}
}

QVariant ModelObject::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()||linkdata==nullptr)
        return QVariant();

    switch (index.column()) {
    case 0: {
        switch (role) {
        case Qt::DisplayRole:
            return linkdata->getData()[index.row()].key;
        case Qt::ToolTipRole:
            return linkdata->getData()[index.row()].link.join(",");
        case Qt::UserRole:
            return linkdata->getData()[index.row()].isshow;
        case Qt::UserRole+1:
            return linkdata->getData()[index.row()].sort;
        default:
            return QVariant();
        }
    }
    case 1: {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return linkdata->getData()[index.row()].value;
        case Qt::CheckStateRole:
            return linkdata->getData()[index.row()].isset?Qt::Checked:Qt::Unchecked;
        case Qt::ToolTipRole:
            return linkdata->getData()[index.row()].defvalue.prepend("default: ");
        case Qt::UserRole:
            return linkdata->getData()[index.row()].link.join(",");
        case Qt::SizeHintRole:
            if(!linkdata->getData()[index.row()].link.isEmpty()) {return QSize(200,25);}
            else {return QVariant();}
        default:
            return QVariant();
        }
    }
    default:
        return QVariant();
    }
}

bool ModelObject::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        switch (index.column()) {
        case 1: {
            switch (role) {
            case Qt::EditRole:
                linkdata->setItem(index.row(),IK::value,value);
                break;
            case Qt::CheckStateRole:
                linkdata->setItem(index.row(),IK::isset,value);
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

void ModelObject::setLinkData(Data_Object *v)
{
    beginResetModel();
    linkdata=v;
    endResetModel();
    emit dataChanged(index(0,0),index(rowCount()-1,rowCount()-1));
}
