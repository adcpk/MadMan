#ifndef DATA_TPL_H
#define DATA_TPL_H
#include "item_keyvalue.h"

class Data_Tpl
{
public:
    Data_Tpl();
    QString getType() const;
    Group getData() const;

    void setItem(int i,int k,QVariant v);
    void addItem(int i,Item_KeyValue v);
    void removeItem(int i);
    void loadData(QString setting, QString t);
    void saveData(QString setting);
protected:
    QString type;
    Group data;
};

#endif // DATA_TPL_H
