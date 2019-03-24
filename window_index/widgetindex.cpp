#include "widgetindex.h"
#include "ui_widgetindex.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QInputDialog>
#include "container/data_index.h"
#include "container/data_tpl.h"
#include "modelindex.h"
#include "modeltpl.h"
#include "dialogtpl.h"

WidgetIndex::WidgetIndex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetIndex)
{
    ui->setupUi(this);

    mdlind = new ModelIndex(this);
    mdlproind = new QSortFilterProxyModel(this);
    mdlproind->setSourceModel(mdlind);
    mdlproind->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->listView->setModel(mdlproind);
    connect(ui->lineEdit,&QLineEdit::textChanged,
            mdlproind,&QSortFilterProxyModel::setFilterWildcard);
    connect(ui->lineEdit,&QLineEdit::textChanged,this,[&](){
        ui->label_count->setText(QString::number(mdlproind->rowCount()));
    });
    ui->listView->viewport()->installEventFilter(this);

    mdltpl = new ModelTpl(this);
    connect(ui->pushButton_tpl,&QPushButton::clicked,this,&WidgetIndex::editTpl);

    conmenu = new QMenu(this);
    QAction *act;
    act = new QAction(QIcon(":/image/Add"),"Add",this);
    connect(act,&QAction::triggered,this,&WidgetIndex::add_name);
    conmenu->addAction(act);
    act = new QAction(QIcon(":/image/Copy"),"Copy",this);
    conmenu->addAction(act);
    act = new QAction(QIcon(":/image/Delete"),"Delete",this);
    conmenu->addAction(act);
    connect(this,&WidgetIndex::rc_key,this,&WidgetIndex::cal_conmenu);
}

WidgetIndex::~WidgetIndex()
{
    delete ui;
}

bool WidgetIndex::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->listView->viewport()&&dataind!=nullptr) {
        if(event->type()==QEvent::MouseButtonDblClick) {
            QMouseEvent *ee = static_cast<QMouseEvent *>(event);
            if(ee->button()==Qt::LeftButton) {
                if(QApplication::keyboardModifiers()==Qt::ControlModifier) {
                    QModelIndex ind = ui->listView->indexAt(ee->localPos().toPoint());
                    QString name = ind.isValid()?ind.data().toString():"";
                    emit cdb_key(QStringList(dataind->getType()),name,ind);
                    return true;
                }
                else {
                    QModelIndex ind = ui->listView->indexAt(ee->localPos().toPoint());
                    QString name = ind.isValid()?ind.data().toString():"";
                    emit db_key(QStringList(dataind->getType()),name,ind);
                    return true;
                }
            }
        }
        else if(event->type()==QEvent::MouseButtonPress) {
            QMouseEvent *ee = static_cast<QMouseEvent *>(event);
            if(ee->button()==Qt::RightButton) {
                QModelIndex ind = ui->listView->indexAt(ee->localPos().toPoint());
                QString name = ind.isValid()?ind.data().toString():"";
                emit rc_key(QStringList(dataind->getType()),name,ind);
                return true;
            }
        }
    }
    return false;
}

void WidgetIndex::setTpl(Data_Tpl *t, QString s)
{
    setpath = s;
    datatpl=t;
    mdltpl->setLinkData(datatpl);
}

void WidgetIndex::setIndex(Data_Index *i, QString r, QString a)
{
    rulepath = r;
    artpath = a;
    dataind=i;
    mdlind->setLinkData(dataind);
    ui->label_count->setText(QString::number(dataind->getData().size()));
}

void WidgetIndex::editTpl()
{
    DialogTpl *di = new DialogTpl(this);
    di->setData(datatpl);
    if(di->exec()==QDialog::Accepted&&di->isWindowModified()) {
        di->saveData(setpath);
        *datatpl = di->getDatatpl();
    }
    di->deleteLater();
}

void WidgetIndex::add_name()
{
    QString n = QInputDialog::getText(this,"add","input name:");
    if (!n.isEmpty()) {
        if(dataind->addIndex(rulepath,artpath,n)) {
            mdlind->setLinkData(dataind);
            ui->label_count->setText(QString::number(dataind->getData().size()));
            emit db_key(QStringList(dataind->getType()),n,QModelIndex());
        }
        else {
            QMessageBox::critical(this,"error","exist same item in [rulemd] or [artmd]");
        }
    }
}

void WidgetIndex::copy_name(QString i)
{
    QString n = QInputDialog::getText(this,"add","input new name:");
    if (!n.isEmpty()) {
        if(dataind->copyIndex(rulepath,artpath,i,n)) {
            mdlind->setLinkData(dataind);
            ui->label_count->setText(QString::number(dataind->getData().size()));
        }
        else {
            QMessageBox::critical(this,"error","exist same item in [rulemd] or [artmd]");
        }
    }
}

void WidgetIndex::del_name(QString i)
{
    dataind->delIndex(rulepath,artpath,i);
    mdlind->setLinkData(dataind);
}

void WidgetIndex::cal_conmenu(QStringList type, QString name, QModelIndex namei)
{
    conmenu->actions()[1]->setEnabled(namei.isValid());
    conmenu->actions()[2]->setEnabled(namei.isValid());
    QString i = namei.isValid()?namei.data().toString():"";
    disconnect(conmenu->actions()[1],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[1],&QAction::triggered,this,[&,i](){this->copy_name(i);});
    disconnect(conmenu->actions()[2],nullptr,nullptr,nullptr);
    connect(conmenu->actions()[2],&QAction::triggered,this,[&,i](){this->del_name(i);});
    conmenu->exec(QCursor::pos());
}

void WidgetIndex::resetindex()
{
    dataind = nullptr;
    mdlind->setLinkData(dataind);
    ui->label_count->setText("0");
}
