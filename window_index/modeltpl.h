#ifndef MODELTPL_H
#define MODELTPL_H

#include <QAbstractTableModel>

class Data_Tpl;
class ModelTpl : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelTpl(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
public:
    void setLinkData(Data_Tpl *v);
private:
    Data_Tpl *linkdata=nullptr;
};

#endif // MODELTPL_H
