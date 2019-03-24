#ifndef DIALOGTPL_H
#define DIALOGTPL_H

#include <QDialog>
#include "container/data_tpl.h"

namespace Ui {
class DialogTpl;
}

class ModelTpl;
class QSortFilterProxyModel;
class DialogTpl : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTpl(QWidget *parent = nullptr);
    ~DialogTpl();

    void setData(Data_Tpl *t);
    void saveData(QString setting);
    Data_Tpl getDatatpl() const;
private:
    Ui::DialogTpl *ui;
    Data_Tpl datatpl;
    ModelTpl *mdltpl = nullptr;
    QSortFilterProxyModel *mdlprotpl;
};

#endif // DIALOGTPL_H
