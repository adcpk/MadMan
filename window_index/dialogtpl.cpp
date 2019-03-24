#include "dialogtpl.h"
#include "ui_dialogtpl.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include "modeltpl.h"

DialogTpl::DialogTpl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTpl)
{
    ui->setupUi(this);

    mdltpl = new ModelTpl(this);
    mdlprotpl = new QSortFilterProxyModel(this);
    mdlprotpl->setSourceModel(mdltpl);
    mdlprotpl->setFilterKeyColumn(1);
    mdlprotpl->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mdlprotpl->sort(0);
    ui->tableView->setModel(mdlprotpl);
    connect(mdltpl,&ModelTpl::dataChanged,this,[&](){setWindowModified(true);});
    connect(mdltpl,&ModelTpl::rowsInserted,this,[&](){setWindowModified(true);});
    connect(mdltpl,&ModelTpl::rowsRemoved,this,[&](){setWindowModified(true);});
    connect(mdltpl,&ModelTpl::rowsMoved,this,[&](){setWindowModified(true);});

    connect(ui->lineEdit,&QLineEdit::textChanged,
            mdlprotpl,&QSortFilterProxyModel::setFilterWildcard);
    connect(ui->pushButton_add,&QPushButton::clicked,[&](){
        mdltpl->insertRow(mdltpl->rowCount());
        QItemSelectionModel *s = ui->tableView->selectionModel();
        QModelIndexList id = s->selectedRows();
        if(!id.isEmpty()) {
            int ss = mdlprotpl->mapToSource(mdlprotpl->index(id.first().row(),0)).data().toInt();
            mdltpl->setData(mdltpl->index(mdltpl->rowCount()-1,0),ss);
        }
    });
    connect(ui->pushButton_del,&QPushButton::clicked,
            this,[&](){
        QItemSelectionModel *s = ui->tableView->selectionModel();
        QModelIndexList id = s->selectedRows();
        if(!id.isEmpty()) {
            int r = mdlprotpl->mapToSource(id.first()).row();
            mdltpl->setData(mdltpl->index(r,0),mdltpl->rowCount()-1);
            mdltpl->removeRow(r);
        }
    });
    connect(ui->tableView->selectionModel(),&QItemSelectionModel::selectionChanged,
            this,[&](const QItemSelection &selected){
        ui->pushButton_del->setEnabled(!selected.isEmpty());
    });

}

DialogTpl::~DialogTpl()
{
    delete ui;
}

void DialogTpl::setData(Data_Tpl *t)
{
    if(t!=nullptr) {
        datatpl=*t;
        mdltpl->setLinkData(&datatpl);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
        setWindowTitle(datatpl.getType()+"Template[*]");
    }
}

void DialogTpl::saveData(QString setting)
{
    datatpl.saveData(setting);
}

Data_Tpl DialogTpl::getDatatpl() const
{
    return datatpl;
}
