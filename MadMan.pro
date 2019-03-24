#-------------------------------------------------
#
# Project created by QtCreator 2019-01-21T22:15:30
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MadMan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    container/data_tpl.cpp \
    container/data_object.cpp \
    container/data_index.cpp \
    window_main/mainwindow.cpp \
    window_index/modeltpl.cpp \
    window_index/dialogtpl.cpp \
    window_index/modelindex.cpp \
    window_index/modeltpl.cpp \
    window_index/widgetindex.cpp \
    window_object/modelobject.cpp \
    window_object/widgetobject.cpp \
    window_object/keyfilterproxymodel.cpp \
    window_object/linkdelegate.cpp \
    window_object/widgetlink.cpp \
    container/item_keyvalue.cpp \

HEADERS += \
    container/item_keyvalue.h \
    container/data_tpl.h \
    container/data_object.h \
    container/data_index.h \
    window_main/mainwindow.h \
    window_index/modeltpl.h \
    window_index/dialogtpl.h \
    window_index/modelindex.h \
    window_index/modeltpl.h \
    window_index/widgetindex.h \
    window_object/modelobject.h \
    window_object/widgetobject.h \
    window_object/keyfilterproxymodel.h \
    window_object/linkdelegate.h \
    window_object/widgetlink.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    window_main/mainwindow.ui \
    window_index/dialogtpl.ui \
    window_index/widgetindex.ui \
    window_object/widgetobject.ui \
    window_object/widgetlink.ui \

RESOURCES += \
    rc.qrc

RC_ICONS = da.ico
