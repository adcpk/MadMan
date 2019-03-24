#ifndef KEYFILTERPROXYMODEL_H
#define KEYFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

class KeyFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
  public:
	KeyFilterProxyModel(QObject* parent = nullptr);
    void set_ishowdef(bool is);
  protected:
    bool ishowdef=false;
	bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // KEYFILTERPROXYMODEL_H
