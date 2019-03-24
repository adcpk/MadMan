#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H
#include "item_keyvalue.h"

class Data_Object
{
public:
    Data_Object();
    QString getType() const;
    Group getData() const;
    QString getName() const;
    QString getLongname() const;
    bool getImageset() const;
    QString getImage() const;
    bool getIsreg() const;

    void installTpl(QString t, Group tpl);

    void setItem(int i,int k,QVariant v);
    void loadData(QString rulepath,QString artpath, QString n, bool reg);
    bool saveData(QString rulepath,QString artpath);
    bool exportData(QString filen);
protected:
    QString type;
    Group data;

    QString name;
    QString longname;
    bool isreg;
    bool imageset;
    QString image;
};

#endif // DATA_OBJECT_H
