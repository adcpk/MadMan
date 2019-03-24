#ifndef WIDGETLINK_H
#define WIDGETLINK_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class WidgetLink;
}

class QMenu;
class WidgetLink : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLink(QWidget *parent = nullptr);
    ~WidgetLink();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void db_key(QStringList type,QString name,QModelIndex namei);
    void cdb_key(QStringList type,QString name,QModelIndex namei);
    void rc_key(QStringList type,QString name,QModelIndex namei);
public:
    void set_names(QString n);
    void set_types(QString t);
    void re_name(int i);
    void add_name(int i);
    void del_name(int i);
    void cal_conmenu(QStringList type,QString name,QModelIndex namei);
    QStringList get_names();
private:
    Ui::WidgetLink *ui;
    QMenu *conmenu;

    QStringList types;
    QStringList names;
};

#endif // WIDGETLINK_H
