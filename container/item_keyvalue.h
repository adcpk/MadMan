#ifndef ITEM_KEYVALUE_H
#define ITEM_KEYVALUE_H
#include <QString>
#include <QList>
#include <QStringList>

enum IK {key,value,isset,sort,isshow,defvalue,link};

struct Item_KeyValue
{
    QString key;
    QString value;

    bool isset = false;

    int sort = 0;
    bool isshow = true;
    QString defvalue;
    QStringList link;
public:
    bool setData(int k,QVariant v);
    bool operator <(const Item_KeyValue &a) const;
};

using Group = QList<Item_KeyValue>;

#endif // ITEM_KEYVALUE_H
