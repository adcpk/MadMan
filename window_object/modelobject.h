#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include <QAbstractTableModel>

class Data_Object;
class ModelObject : public QAbstractTableModel
{
	Q_OBJECT

  public:
	explicit ModelObject(QObject* parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  public:
    void setLinkData(Data_Object *v);
  private:
    Data_Object *linkdata;
};

#endif // MODELOBJECT_H
