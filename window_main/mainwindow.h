#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <container/data_index.h>
#include <container/data_tpl.h>

namespace Ui {
class MainWindow;
}

class WidgetIndex;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadType();
    void openType();
    void loadTpl();
    void loadIndex();

    QString search_name(QStringList typel,QString name);
    void open_in_main(QStringList typel,QString name);
    void open_in_right(QStringList typel,QString name);
public slots:
    void open_file();
    void import();
    void close_file();
    void closeall_main();
    void closeall_right();
    void taball_right();
    void twocol_right();
    void format_file();
protected:
    void build_index(QString type, QString indexn);
private:
    Ui::MainWindow *ui;
    QMap<QString,WidgetIndex *> indexwins;
    QList<QDockWidget *> rightwins;

    QString setpath = "setting.ini";
    QString rulepath = "rulesmd.ini";
    QString artpath = "artmd.ini";
    QList<QPair<QString,QString>> types;
    QMap<QString,Data_Index> indexs;
    QMap<QString,Data_Tpl> tpls;
};

#endif // MAINWINDOW_H
