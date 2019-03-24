#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSettings>
#include <QDockWidget>
#include <QMessageBox>
#include <QModelIndex>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QtConcurrent>
#include "window_index/widgetindex.h"
#include "window_object/widgetobject.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	connect(ui->tabWidget_main, &QTabWidget::tabBarDoubleClicked,
	        ui->tabWidget_main, &QTabWidget::removeTab);
	connect(ui->actionAboutQt, &QAction::triggered, this, [&]() {QMessageBox::aboutQt(this);});

	loadType();
	openType();
	loadTpl();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadType()
{
	QSettings ini(setpath, QSettings::IniFormat);
	int size = ini.beginReadArray("types");
	for(int i = 0; i != size; ++i)
	{
		ini.setArrayIndex(i);
		types << QPair<QString, QString>(
		          ini.value("T").toString(),
		          ini.value("str").toString());
	}
	ini.endArray();
}

void MainWindow::openType()
{
	for(auto i = types.begin(); i != types.end(); ++i)
	{
		WidgetIndex* wg = new WidgetIndex(this);
		int ii = ui->tabWidget_index->addTab(wg, i->first);
		ui->tabWidget_index->setTabIcon(ii, QIcon(":/image/" + i->first));
		indexwins[i->first] = wg;
        connect(wg, &WidgetIndex::db_key, this, &MainWindow::open_in_main);
        connect(wg, &WidgetIndex::cdb_key, this, &MainWindow::open_in_right);
	}
	ui->tabWidget_index->setCurrentWidget(indexwins["Building"]);
}

void MainWindow::loadTpl()
{
	for(auto i = types.begin(); i != types.end(); ++i)
	{
		Data_Tpl temp;
		temp.loadData(setpath, i->first);
		tpls[i->first] = temp;
		indexwins[i->first]->setTpl(&tpls[i->first], setpath);
	}
}

void MainWindow::loadIndex()
{
	QString report;
	for(auto i = types.begin(); i != types.end(); ++i)
	{
		Data_Index temp;
        QMap<QString, int> rep;
        rep = temp.loadData(rulepath, i->first, i->second);
        if(temp.getData().isEmpty()) {
            if(QMessageBox::warning(this,
                                    "wrong file",
                                    "no index of " + i->first+", ok to build it",
                                    QMessageBox::Ok|QMessageBox::Ignore)
                    == QMessageBox::Ok) {
                build_index(i->first, i->second);
                rep = temp.loadData(rulepath, i->first, i->second);
            }
            else {
                continue;
            }
        }
		if(!rep.isEmpty()) {report += "[" + i->first + "]\n";}
		for(auto j = rep.begin(); j != rep.end(); ++j)
		{
			report += j.key() + ": " + QString::number(j.value()) + "\n";
		}

		indexs[i->first] = temp;
		WidgetIndex* w = indexwins[i->first];
		w->setIndex(&indexs[i->first], rulepath, artpath);
	}
	if(!report.isEmpty())
	{
		QMessageBox::warning(this, "repeat", report);
	}
}

QString MainWindow::search_name(QStringList typel, QString name)
{
	for(auto type_i = typel.begin(); type_i != typel.end(); ++type_i)
	{
		if(*type_i == "Image") {break;}
		if(indexs[*type_i].getData().contains(name)) {return *type_i;}
	}

	QSettings ini(artpath, QSettings::IniFormat);
	ini.beginGroup(name);
	if(!ini.childKeys().isEmpty())
	{
		return "Animation";
	}
	else
	{
		QMessageBox::warning(this, "not found",
		                     "[" + name + "] not found in [" + typel.join(",") + "]");
		return "";
	}
}

void MainWindow::open_in_main(QStringList typel, QString name)
{
	if(name.isEmpty()) {return;}
	QString t = search_name(typel, name);
	if(!t.isEmpty())
	{
		WidgetObject* wg = new WidgetObject(this);
		wg->loadData(t, tpls[t].getData(), rulepath, artpath, name, &indexs);
		connect(wg, &WidgetObject::db_key, this, &MainWindow::open_in_main);
		connect(wg, &WidgetObject::cdb_key, this, &MainWindow::open_in_right);
		wg->setLoaction("main");
		int i = ui->tabWidget_main->addTab(wg, name);
		ui->tabWidget_main->setTabIcon(i, QIcon(":/image/" + t));
		ui->tabWidget_main->setCurrentWidget(wg);
	}
}

void MainWindow::open_in_right(QStringList typel, QString name)
{
	if(name.isEmpty()) {return;}
	QString t = search_name(typel, name);
	if(!t.isEmpty())
	{
		WidgetObject* wg = new WidgetObject(this);
		wg->loadData(t, tpls[t].getData(), rulepath, artpath, name, &indexs);
		connect(wg, &WidgetObject::db_key, this, &MainWindow::open_in_main);
		connect(wg, &WidgetObject::cdb_key, this, &MainWindow::open_in_right);
		wg->setLoaction("right");
		QDockWidget* dw = new QDockWidget(name, this);
		dw->setFeatures(QDockWidget::DockWidgetClosable |
		                QDockWidget::DockWidgetMovable);
		dw->setAllowedAreas(Qt::RightDockWidgetArea);
		dw->setWidget(wg);
		dw->setAttribute(Qt::WA_DeleteOnClose);
		connect(dw, &QDockWidget::destroyed, this, [ &, dw]() {rightwins.removeOne(dw);});
		addDockWidget(Qt::RightDockWidgetArea, dw);
		rightwins.push_back(dw);
	}
	else
	{
		return;
	}

	if(rightwins.size() > 8)
	{
		taball_right();
	}
	else if(rightwins.size() > 4)
	{
		twocol_right();
	}
	else
	{
		if(!tabifiedDockWidgets(rightwins.first()).isEmpty())
		{
			setDockOptions(QMainWindow::AnimatedDocks |
			               QMainWindow::AllowNestedDocks);
			for(int i = 0; i != rightwins.size(); ++i)
			{
				addDockWidget(Qt::RightDockWidgetArea, rightwins[i]);
			}
		}
		QList<int> size;
		for(int i = 0; i != rightwins.size(); ++i) {size << 1;}
		resizeDocks(rightwins, size, Qt::Vertical);
		resizeDocks(rightwins, size, Qt::Horizontal);
		setDockOptions(QMainWindow::AnimatedDocks |
		               QMainWindow::AllowNestedDocks |
		               QMainWindow::AllowTabbedDocks |
		               QMainWindow::VerticalTabs);
	}
}

void MainWindow::open_file()
{
	QFileInfo inf;
	QFile f;
	QString rf = QFileDialog::getOpenFileName(this, "rulesmd", "*rules*", "INI files (*.ini)");
	if(!rf.isEmpty())
	{
		QString af = rf;
		af.replace("rules", "art");
		inf.setFile(af);
		if(!inf.exists()) {QMessageBox::warning(this, "not found", "no art in folder"); return;}
		close_file();
		rulepath = rf;
		artpath = af;
		setWindowTitle(inf.absolutePath() + " - MadMan");
		f.setFileName(rf);
		inf.setFile(rf);
		f.copy(inf.absolutePath() + "/"
		       + inf.baseName()
		       + QDateTime::currentDateTime().toString("yyMMddHHmmss")
		       + "." + inf.suffix());
		f.setFileName(af);
		inf.setFile(af);
		f.copy(inf.absolutePath() + "/"
		       + inf.baseName()
		       + QDateTime::currentDateTime().toString("yyMMddHHmmss")
		       + "." + inf.suffix());
		loadIndex();
	}
	ui->actionFormat->setEnabled(true);
	ui->actionClose->setEnabled(true);
	ui->actionImport->setEnabled(true);
}

void MainWindow::import()
{
	QFileInfo info(rulepath);
	QString newf = QFileDialog::getOpenFileName(this, "import",
	               info.absolutePath(), "INI (*.ini)", nullptr,
	               QFileDialog::DontConfirmOverwrite);
	if(newf == rulepath || newf == artpath)
	{
		QMessageBox::critical(this, "error", "can not import from rules or art");
		return;
	}

	if(!newf.isEmpty())
	{
		QSettings ini(newf, QSettings::IniFormat);
		if(ini.childKeys().isEmpty())
		{
			QMessageBox::critical(this, "error", "wrong or empty file");
			return;
		}

        bool isrep=false;
        QString rep="repeat:";
        for(QString s : ini.childKeys())
        {
            if(indexs[ini.value(s).toString()].getData().contains(s)) {
                rep+=QString("\n[%1] exist in rules (%2)").arg(s,ini.value(s).toString());
                isrep=true;
            }
        }
        QFileInfo info(newf);
        QString rule = info.absolutePath()+"/"+info.baseName()+"_rules.ini";
        QString art = info.absolutePath()+"/"+info.baseName()+"_art.ini";
        QSettings ini_a(artpath, QSettings::IniFormat);
        QSet<QString> artitems(QSet<QString>::fromList(ini_a.childGroups()));
        QSettings ini_ai(art, QSettings::IniFormat);
        for(QString s: ini_ai.childGroups()) {
            if(artitems.contains(s)) {
                rep+=QString("\n[%1] exist in art").arg(s);
                isrep=true;
            }
        }

        if(isrep) {QMessageBox::critical(this, "error", rep);}
        else {
            for(QString s : ini.childKeys())
            {
                indexs[ini.value(s).toString()].importIndexreg(rulepath,s);
            }
            for(auto i = types.begin(); i != types.end(); ++i) {
                indexwins[i->first]->setIndex(&indexs[i->first],rulepath,artpath);
            }

            QFile f;
            QString buf;
            f.setFileName(rule);
            f.open(QIODevice::ReadOnly);
            buf = QString::fromUtf8(f.readAll());
            f.close();
            f.setFileName(rulepath);
            f.open(QIODevice::WriteOnly|QIODevice::Append);
            f.write("\n");
            f.write(buf.toUtf8());
            f.close();
            //
            f.setFileName(art);
            f.open(QIODevice::ReadOnly);
            buf = QString::fromUtf8(f.readAll());
            f.close();
            f.setFileName(artpath);
            f.open(QIODevice::WriteOnly|QIODevice::Append);
            f.write("\n");
            f.write(buf.toUtf8());
            f.close();
        }
	}
}

void MainWindow::close_file()
{
	closeall_main();
	closeall_right();
	for(auto i = indexwins.begin(); i != indexwins.end(); ++i)
	{
		i.value()->resetindex();
	}
	indexs.clear();
	setWindowTitle("MadMan");
	format_file();
	ui->actionFormat->setEnabled(false);
	ui->actionClose->setEnabled(false);
	ui->actionImport->setEnabled(false);
}

void MainWindow::closeall_main()
{
	int n = ui->tabWidget_main->count();
	for(int i = 0; i != n; ++i)
	{
		ui->tabWidget_main->removeTab(0);
	}
}

void MainWindow::closeall_right()
{
	for(int i = 0; i != rightwins.size(); ++i)
	{
		rightwins[i]->close();
	}
}

void MainWindow::taball_right()
{
	setDockOptions(QMainWindow::AnimatedDocks |
	               QMainWindow::AllowNestedDocks |
	               QMainWindow::AllowTabbedDocks |
	               QMainWindow::VerticalTabs);
	if (rightwins.isEmpty()) {return;}
	for(int i = 1; i != rightwins.size(); ++i)
	{
		tabifyDockWidget(rightwins[0], rightwins[i]);
	}
}

void MainWindow::twocol_right()
{
	setDockOptions(QMainWindow::AnimatedDocks |
	               QMainWindow::AllowNestedDocks);
	for(int i = 0; i != rightwins.size(); ++i)
	{
		addDockWidget(Qt::RightDockWidgetArea, rightwins[i]);
	}
	for(int i = 0; i < rightwins.size() - 1; i += 2)
	{
		splitDockWidget(rightwins[i], rightwins[i + 1], Qt::Horizontal);
	}
	QList<int> size;
	for(int i = 0; i != rightwins.size(); ++i) {size << 1;}
	resizeDocks(rightwins, size, Qt::Vertical);
	resizeDocks(rightwins, size, Qt::Horizontal);
	setDockOptions(QMainWindow::AnimatedDocks |
	               QMainWindow::AllowNestedDocks |
	               QMainWindow::AllowTabbedDocks |
	               QMainWindow::VerticalTabs);
}

void MainWindow::format_file()
{
	ui->actionFormat->setEnabled(false);
	QFile f;
	QString buf;
	f.setFileName(rulepath);
	f.open(QIODevice::ReadOnly);
	buf = QString::fromUtf8(f.readAll());
	f.close();
	buf.replace(", ", ",");
    buf.replace("\"", "");
	f.open(QIODevice::WriteOnly);
	f.write(buf.toUtf8());
	f.close();
	f.setFileName(artpath);
	f.open(QIODevice::ReadOnly);
	buf = QString::fromUtf8(f.readAll());
	f.close();
	buf.replace(", ", ",");
    buf.replace("\"", "");
	f.open(QIODevice::WriteOnly);
	f.write(buf.toUtf8());
	f.close();
	ui->actionFormat->setEnabled(true);
}

void MainWindow::build_index(QString type, QString indexn)
{
    QSettings ini_set(setpath, QSettings::IniFormat);
    QSet<QString> type_search(QSet<QString>::fromList(ini_set.value("BuildIndex/"+type).toStringList()));
    QSet<QString> type_index;

    QSettings ini_ru(rulepath, QSettings::IniFormat);
    for(QString g:ini_ru.childGroups()) {
        ini_ru.beginGroup(g);
        for(QString k:ini_ru.childKeys()) {
            if(type_search.contains(k)) {
                for(QString n:ini_ru.value(k).toStringList()) {
                    type_index.insert(n);
                }
            }
        }
        ini_ru.endGroup();
    }

    ini_ru.beginGroup(indexn);
    int i=1;
    for(QString k:type_index) {
        ini_ru.setValue(QString::number(i++),k);
    }
    ini_ru.endGroup();
}
