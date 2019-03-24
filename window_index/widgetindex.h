#ifndef WIDGETINDEX_H
#define WIDGETINDEX_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class WidgetIndex;
}

class Data_Index;
class Data_Tpl;
class ModelIndex;
class QSortFilterProxyModel;
class ModelTpl;
class QMenu;
class WidgetIndex : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetIndex(QWidget *parent = nullptr);
    ~WidgetIndex();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void db_key(QStringList type,QString name,QModelIndex namei);
    void cdb_key(QStringList type,QString name,QModelIndex namei);
    void rc_key(QStringList type,QString name,QModelIndex namei);
public:
    void setTpl(Data_Tpl *t,QString s);
    void setIndex(Data_Index *i,QString r,QString a);
    void editTpl();
    void add_name();
    void copy_name(QString i);
    void del_name(QString i);
    void cal_conmenu(QStringList type,QString name,QModelIndex namei);
    void resetindex();
private:
    Ui::WidgetIndex *ui;
    QMenu *conmenu;

    QString setpath;
    QString rulepath;
    QString artpath;
    Data_Index *dataind = nullptr;
    ModelIndex *mdlind = nullptr;
    QSortFilterProxyModel *mdlproind;

    Data_Tpl *datatpl = nullptr;
    ModelTpl *mdltpl = nullptr;
};

#endif // WIDGETINDEX_H
