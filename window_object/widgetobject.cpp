#include "widgetobject.h"
#include "ui_widgetobject.h"
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include "keyfilterproxymodel.h"
#include "modelobject.h"
#include "linkdelegate.h"
#include <QMessageBox>
#include "container/data_index.h"

WidgetObject::WidgetObject(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::WidgetObject)
{
	ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

	mdlobject = new ModelObject(this);
    mdlobject->setLinkData(&dataobject);
    mdlproobject = new KeyFilterProxyModel(this);
    mdlproobject->setSourceModel(mdlobject);
    mdlproobject->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mdlproobject->sort(0);
    mdlproobject->setSortRole(Qt::UserRole+1);
    ui->tableView->setModel(mdlproobject);
    ui->tableView->setItemDelegateForColumn(1,new LinkDelegate(this));

    connect(ui->lineEdit, &QLineEdit::textChanged,
            mdlproobject, &QSortFilterProxyModel::setFilterWildcard);
    connect(ui->toolButton_all, &QToolButton::toggled, this, [&](bool all)
    {
        mdlproobject->set_ishowdef(all);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
        mdlproobject->sort(0);
    });
    connect(ui->toolButton_sort, &QToolButton::toggled, this, [&](bool d)
    {
        int role = d?Qt::UserRole+1:Qt::DisplayRole;
        ui->toolButton_sort->setText(d?"D":"A");
        mdlproobject->setSortRole(role);
    });
    connect(ui->pushButton_Save,&QToolButton::clicked,this,[&](){
        this->saveDate(rulepath,artpath);
    });
    connect(ui->pushButton_Export,&QToolButton::clicked,this,&WidgetObject::exportDate);

	ui->pushButton_Image->installEventFilter(this);
}

WidgetObject::~WidgetObject()
{
	delete ui;
}

bool WidgetObject::eventFilter(QObject* watched, QEvent* event)
{
    if(watched==ui->pushButton_Image) {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* ee = static_cast<QMouseEvent*>(event);
            if(ee->button() == Qt::LeftButton)
            {
                if(QApplication::keyboardModifiers()==Qt::ControlModifier) {
                    emit cdb_key(QStringList("Image"), dataobject.getImage(),QModelIndex());
                }
                else {
                    emit db_key(QStringList("Image"), dataobject.getImage(),QModelIndex());
                }
                return true;
            }
        }
    }
    return false;
}

void WidgetObject::loadData(QString t, Group tpl, QString rule, QString art, QString n, const QMap<QString,Data_Index> *ind)
{
    rulepath = rule;
    artpath = art;
    indexs = ind;
    dataobject.installTpl(t,tpl);
    dataobject.loadData(rulepath,artpath,n,indexs->value(t).getData().contains(n));


    if(dataobject.getType()=="Animation"||
            dataobject.getType()=="Weapon"||
            dataobject.getType()=="Warhead") {
        ui->pushButton_Image->setHidden(true);}
    connect(mdlobject,&ModelObject::dataChanged,this,[&](){
        QString la = dataobject.getName();
        if(!dataobject.getLongname().isEmpty()) {la = la+" : "+dataobject.getLongname();}
        ui->label_type->setText(la);
        QFont ltf = ui->label_type->font();
        ltf.setBold(dataobject.getIsreg());
        ui->label_type->setFont(ltf);
        ui->pushButton_Image->setText(dataobject.getImage());
        if(dataobject.getImageset()) {ui->pushButton_Image->setStyleSheet("color: rgb(65, 205, 82);");}
        else {ui->pushButton_Image->setStyleSheet("color: rgb(0, 0, 0);");}
        ui->pushButton_Save->setEnabled(true);
    });

    mdlobject->setLinkData(&dataobject);
    ui->pushButton_Save->setEnabled(false);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void WidgetObject::saveDate(QString rule, QString art)
{
    if(ui->pushButton_Save->isEnabled()) {
        bool su = dataobject.saveData(rule,art);
        if(su) {
            ui->pushButton_Save->setEnabled(false);
        }
        else {
            QMessageBox::warning(this,"error","can not save: "+dataobject.getName()+" not found");
        }
    }
}

void WidgetObject::exportDate()
{
    QFileInfo info(rulepath);
    QString newf = QFileDialog::getSaveFileName(this,"export",
                                                info.absolutePath(),"INI (*.ini)",nullptr,
                                                QFileDialog::DontConfirmOverwrite);
    if(newf==rulepath||newf==artpath) {
        QMessageBox::critical(this,"error","can not export to rules or art");
        return;
    }
    if(!newf.isEmpty()) {
        dataobject.exportData(newf);
        QMessageBox::information(this,"Export","Export "+dataobject.getName()+" succeed "+newf);
    }
}

void WidgetObject::setLoaction(QString l)
{
    if(l=="right") {
        ui->toolButton->setIcon(QIcon(":/image/"+dataobject.getType()));
    }
    else if(l=="main") {
        ui->toolButton->setHidden(true);
        ui->label_type->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
        ui->label_type->setMaximumWidth(10000);
    }
}
