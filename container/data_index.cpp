#include "data_index.h"
#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include <QFileInfo>

Data_Index::Data_Index()
{

}

QString Data_Index::getType() const {return type;}

QString Data_Index::getIndex() const {return index;}

QMap<QString, int> Data_Index::getData() const {return data;}

int Data_Index::getMaxid() const {return maxid;}

QMap<QString,int> Data_Index::loadData(QString rulepath, QString tp, QString idx)
{
    type = tp;
    index = idx;
    QSettings ini(rulepath,QSettings::IniFormat);
    QMap<QString,int> rep;
    ini.beginGroup(index);
    QStringList list = ini.childKeys();
    for(QStringList::iterator i=list.begin();i!=list.end();++i) {
        int id = i->toInt();
        maxid = id>maxid?id:maxid;
        QString key = ini.value(*i).toString();
        if(data.contains(key)) {
            if(!rep.contains(key)) {rep[key]=data[key];}
            rep.insertMulti(key,id);
        }
        else {
            data[key] = id;
        }
    }
    ini.endGroup();
    return rep;
}

bool Data_Index::addIndex(QString rulepath, QString artpath, QString n)
{
    if(data.contains(n)) return false;

    if(type!="Weapon"&&type!="Warhead") {
        QSettings ini_art(artpath,QSettings::IniFormat);
        ini_art.beginGroup(n);
        if(!ini_art.childKeys().isEmpty()) return false;
        ini_art.setValue("MadManMagic",QDateTime::currentDateTime().toString("yy-MM-dd HH:mm:ss"));
        ini_art.endGroup();
    }

    QSettings ini_rule(rulepath,QSettings::IniFormat);
    if(type!="Animation") {
        ini_rule.beginGroup(n);
        if(!ini_rule.childKeys().isEmpty()) return false;
        ini_rule.setValue("MadManMagic",QDateTime::currentDateTime().toString("yy-MM-dd HH:mm:ss"));
        ini_rule.endGroup();
    }

    ini_rule.beginGroup(index);
    ini_rule.setValue(QString::number(++maxid),n);
    ini_rule.endGroup();

    data[n]=maxid;
    return true;
}

bool Data_Index::copyIndex(QString rulepath, QString artpath, QString nold, QString nnew)
{
    if(!addIndex(rulepath,artpath,nnew)) return false;
    if(!data.contains(nold)) return false;

    QStringList kl;
    if(type!="Weapon"&&type!="Warhead") {
        QSettings ini_art(artpath,QSettings::IniFormat);
        ini_art.beginGroup(nold);
        kl = ini_art.childKeys();
        ini_art.endGroup();
        if(!kl.isEmpty()) {
            for(auto i=kl.begin();i!=kl.end();++i) {
                ini_art.setValue(nnew+"/"+(*i),ini_art.value(nold+"/"+(*i)));
            }
        }
    }

    if(type!="Animation") {
        QSettings ini_rule(rulepath,QSettings::IniFormat);
        ini_rule.beginGroup(nold);
        kl = ini_rule.childKeys();
        ini_rule.endGroup();
        if(!kl.isEmpty()) {
            for(auto i=kl.begin();i!=kl.end();++i) {
                ini_rule.setValue(nnew+"/"+(*i),ini_rule.value(nold+"/"+(*i)));
            }
        }
    }

    return true;
}

bool Data_Index::delIndex(QString rulepath, QString artpath, QString n)
{
    if(!data.contains(n)) return false;

    if(type!="Weapon"&&type!="Warhead") {
        QSettings ini_art(artpath,QSettings::IniFormat);
        ini_art.remove(n);
    }

    QSettings ini_rule(rulepath,QSettings::IniFormat);
    if(type!="Animation") {
        ini_rule.remove(n);
    }

    ini_rule.beginGroup(index);
    ini_rule.remove(QString::number(data[n]));
    ini_rule.endGroup();

    data.remove(n);
    return true;
}

bool Data_Index::importIndexreg(QString rulepath, QString n)
{
    QSettings ini_rule(rulepath,QSettings::IniFormat);
    ini_rule.setValue(index+"/"+QString::number(++maxid),n);
    data.insert(n,maxid);
    return true;
}
