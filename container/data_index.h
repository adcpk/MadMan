#ifndef DATA_INDEX_H
#define DATA_INDEX_H
#include <QString>
#include <QMap>

class Data_Index
{
public:
    Data_Index();
    QString getType() const;
    QString getIndex() const;
    QMap<QString, int> getData() const;
    int getMaxid() const;

    QMap<QString,int> loadData(QString rulepath, QString tp, QString idx);
    bool addIndex(QString rulepath, QString artpath, QString n);
    bool copyIndex(QString rulepath, QString artpath, QString nold,QString nnew);
    bool delIndex(QString rulepath, QString artpath, QString n);
    bool importIndexreg(QString rulepath, QString n);
protected:
    QString type;
    QString index;
    QMap<QString,int> data;
    int maxid = 0;
};

#endif // DATA_INDEX_H
