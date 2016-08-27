#-------------------------------------------------
#
# Project created by QtCreator 2016-08-15T11:07:51
#
#-------------------------------------------------

QT        += core gui\
          serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = VLDiscovery

TEMPLATE  = app

SOURCES   += main.cpp\
          mainwindow.cpp \
          settingsdialog.cpp

HEADERS   += mainwindow.h \
          settingsdialog.h

FORMS     += mainwindow.ui \
          settingsdialog.ui

RESOURCES += vldiscovery.qrc
