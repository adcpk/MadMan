#ifndef WIDGETOBJECT_H
#define WIDGETOBJECT_H

#include <QWidget>
#include <QModelIndex>
#include "container/data_object.h"

namespace Ui
{
	class WidgetObject;
}

class ModelObject;
class KeyFilterProxyModel;
class Data_Index;
class WidgetObject : public QWidget
{
	Q_OBJECT

  public:
	explicit WidgetObject(QWidget* parent = nullptr);
	~WidgetObject();
  protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
  signals:
    void db_key(QStringList type,QString name,QModelIndex namei);
    void cdb_key(QStringList type,QString name,QModelIndex namei);
    void rc_key(QStringList type,QString name,QModelIndex namei);
  public:
    void loadData(QString t, Group tpl,
                  QString rule,QString art, QString n,
                  const QMap<QString,Data_Index> *ind);
    void saveDate(QString rule,QString art);
    void exportDate();
    void setLoaction(QString l);
  private:
	Ui::WidgetObject* ui;

    QString rulepath;
    QString artpath;
    Data_Object dataobject;
	ModelObject* mdlobject;
    KeyFilterProxyModel* mdlproobject;

    const QMap<QString,Data_Index> *indexs = nullptr;
};

#endif // WIDGETOBJECT_H
