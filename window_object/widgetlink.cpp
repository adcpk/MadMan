#include "widgetlink.h"
#include "ui_widgetlink.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QInputDialog>
#include <QClipboard>

WidgetLink::WidgetLink(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetLink)
{
    ui->setupUi(this);
    ui->listWidget->viewport()->installEventFilter(this);
    conmenu = new QMenu(this);
    QAction *act;
    act = new QAction(QIcon(":/image/Rename"),"Rename",this);
    conmenu->addAction(act);
    act = new QAction(QIcon(":/image/Add"),"Add",this);
    conmenu->addAction(act);
    act = new QAction(QIcon(":/image/Delete"),"Delete",this);
    conmenu->addAction(act);
    act = new QAction("Copy Text",this);
    conmenu->addAction(act);
    connect(this,&WidgetLink::rc_key,this,&WidgetLink::cal_conmenu);
}

WidgetLink::~WidgetLink()
{
    delete ui;
}

bool WidgetLink::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->listWidget->viewport()) {
        if(event->type()==QEvent::MouseButtonDblClick) {
            QMouseEvent *ee = static_cast<QMouseEvent *>(event);
            if(ee->button()==Qt::LeftButton) {
                if(QApplication::keyboardModifiers()==Qt::ControlModifier) {
                    QModelIndex ind = ui->listWidget->indexAt(ee->localPos().toPoint());
                    QString name = ind.isValid()?ind.data().toString():"";
                    emit cdb_key(types,name,ind);
                    return true;
                }
                else {
                    QModelIndex ind = ui->listWidget->indexAt(ee->localPos().toPoint());
                    QString name = ind.isValid()?ind.data().toString():"";
                    emit db_key(types,name,ind);
                    return true;
                }
            }
        }
        else if(event->type()==QEvent::MouseButtonPress) {
            QMouseEvent *ee = static_cast<QMouseEvent *>(event);
            if(ee->button()==Qt::RightButton) {
                QModelIndex ind = ui->listWidget->indexAt(ee->localPos().toPoint());
                ui->listWidget->selectionModel()->select(ind,QItemSelectionModel::ClearAndSelect);
                QString name = ind.isValid()?ind.data().toString():"";
                emit rc_key(types,name,ind);
                return true;
            }
        }
    }
    return false;
}

void WidgetLink::set_names(QString n)
{
    names = n.split(",",QString::SkipEmptyParts);
    ui->listWidget->addItems(names);
}

void WidgetLink::set_types(QString t)
{
    types = t.split(",",QString::SkipEmptyParts);
}

void WidgetLink::re_name(int i)
{
    QString n = QInputDialog::getText(this,"rename","input name:");
    if (!n.isEmpty()) {
        names[i] = n;
        ui->listWidget->item(i)->setText(n);
    }
}

void WidgetLink::add_name(int i)
{
    QString n = QInputDialog::getText(this,"add","input name:");
    if (!n.isEmpty()) {
        names.insert(i,n);
        ui->listWidget->insertItem(i,n);
    }
}

void WidgetLink::del_name(int i)
{
    names.removeAt(i);
    ui->listWidget->takeItem(i);
}

void WidgetLink::cal_conmenu(QStringList type, QString name, QModelIndex namei)
{
    conmenu->actions()[0]->setEnabled(namei.isValid());
    conmenu->actions()[2]->setEnabled(namei.isValid());
    conmenu->actions()[3]->setEnabled(namei.isValid());
    int i = namei.isValid()?namei.row():names.size();
    disconnect(conmenu->actions()[0],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[0],&QAction::triggered,this,[&,i](){this->re_name(i);});
    disconnect(conmenu->actions()[1],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[1],&QAction::triggered,this,[&,i](){this->add_name(i);});
    disconnect(conmenu->actions()[2],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[2],&QAction::triggered,this,[&,i](){this->del_name(i);});
    disconnect(conmenu->actions()[3],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[3],&QAction::triggered,this,[name](){
        QApplication::clipboard()->setText(name);
    });
    conmenu->exec(QCursor::pos());
}

QStringList WidgetLink::get_names()
{
    return names;
}
