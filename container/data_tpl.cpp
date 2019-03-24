#include "data_tpl.h"
#include <QDebug>
#include <QSettings>

Data_Tpl::Data_Tpl()
{

}

QString Data_Tpl::getType() const {return type;}

Group Data_Tpl::getData() const {return data;}

void Data_Tpl::setItem(int i, int k, QVariant v)
{
    data[i].setData(k,v);
}

void Data_Tpl::addItem(int i,Item_KeyValue v)
{
    data.insert(i,v);
}

void Data_Tpl::removeItem(int i)
{
    data.removeAt(i);
}

void Data_Tpl::loadData(QString setting, QString t)
{
    type = t;
    QSettings ini(setting,QSettings::IniFormat);
    int size = ini.beginReadArray(type+"Template");
    for(int i=0;i!=size;++i) {
        ini.setArrayIndex(i);
        Item_KeyValue temp;
        temp.key = ini.value("key","").toString();
        temp.isshow = ini.value("show",true).toBool();//TODO:to false
        temp.sort = ini.value("sort",0).toInt();
        temp.link = ini.value("link","").toStringList();
        temp.defvalue = ini.value("default","").toString();
        data.push_back(temp);
    }
    ini.endArray();
}

void Data_Tpl::saveData(QString setting)
{
    QSettings ini(setting,QSettings::IniFormat);
    ini.beginWriteArray(type+"Template");
    for(int i=0;i!=data.size();++i) {
        ini.setArrayIndex(i);
        ini.setValue("key",data[i].key);
        ini.setValue("isshow",data[i].isshow);
        ini.setValue("sort",data[i].sort);
        ini.setValue("link",data[i].link);
        ini.setValue("default",data[i].defvalue);
    }
    ini.endArray();
}
