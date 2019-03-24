#ifndef MODELINDEX_H
#define MODELINDEX_H

#include <QAbstractTableModel>

class Data_Index;
class ModelIndex : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelIndex(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public:
    void setLinkData(Data_Index *v);
private:
    Data_Index *linkdata=nullptr;
};

#endif // MODELINDEX_H
