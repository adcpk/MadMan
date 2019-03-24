#include "data_object.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>

Data_Object::Data_Object()
{

}

QString Data_Object::getType() const {return type;}

Group Data_Object::getData() const {return data;}

QString Data_Object::getLongname() const {return longname;}

QString Data_Object::getName() const {return name;}

bool Data_Object::getImageset() const {return imageset;}

QString Data_Object::getImage() const {return image;}

bool Data_Object::getIsreg() const {return isreg;}

void Data_Object::installTpl(QString t, Group tpl)
{
    type = t;
    data = tpl;
}

void Data_Object::setItem(int i, int k, QVariant v)
{
    data[i].setData(k,v);
    if(data[i].key=="Image"&&k==IK::isset) {imageset=data[i].isset;}
    if(data[i].key=="Image"&&k==IK::value) {image=data[i].value;}
    if(data[i].key=="Name") {longname=data[i].value;}
}

void Data_Object::loadData(QString rulepath, QString artpath, QString n, bool reg)
{
    isreg=reg;
    name = n;
    QString path = rulepath;
    if(type=="Animation") {path=artpath;}
    QSettings ini(path,QSettings::IniFormat);
    ini.beginGroup(name);
    for(auto i=data.begin();i!=data.end();++i) {
        i->isset = ini.contains(i->key);
        QVariant v = ini.value(i->key,i->defvalue);
        if(v.type()==QVariant::String) {
            i->value = v.toString();
        }
        else if(v.type()==QVariant::StringList) {
            i->value = v.toStringList().join(",");
        }
        if(i->key=="Image") {imageset=i->isset;image=i->value;}
        if(i->key=="Name") {longname=i->value;}
    }
    if(!imageset) {image=name;}
    ini.endGroup();
}

bool Data_Object::saveData(QString rulepath, QString artpath)
{
    QString path = rulepath;
    if(type=="Animation") {path=artpath;}
    QSettings ini(path,QSettings::IniFormat);
    ini.beginGroup(name);
    if(ini.childKeys().isEmpty()) {return false;}
    for(auto i=data.begin();i!=data.end();++i) {
        if(i->isset) {
            ini.setValue(i->key,i->value);
        }
        else {
            if(ini.contains(i->key)) {
                ini.remove(i->key);
            }
        }
    }
    ini.endGroup();
    return true;
}

bool Data_Object::exportData(QString filen)
{
    QFileInfo info(filen);
    QString rule = info.absolutePath()+"/"+info.baseName()+"_rules.ini";
    QString art = info.absolutePath()+"/"+info.baseName()+"_art.ini";
    QSettings ini_in(filen,QSettings::IniFormat);
    if(isreg) {ini_in.setValue(name,type);}

    QString expath = type=="Animation"?art:rule;
    QSettings ini(expath,QSettings::IniFormat);
    ini.remove(name);
    ini.beginGroup(name);
    for(Item_KeyValue a:data) {
        if(a.isset) {ini.setValue(a.key,a.value);}
    }
    ini.endGroup();
    return true;
}
