#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T12:09:08
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tapp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addstudents.cpp \
    showresults.cpp \
    sendsettings.cpp \
    overwritedialog.cpp \
    choosefiledialog.cpp

HEADERS  += mainwindow.h \
    student.h \
    addstudents.h \
    showresults.h \
    sendsettings.h \
    overwritedialog.h \
    choosefiledialog.h

FORMS    += mainwindow.ui \
    addstudents.ui \
    showresults.ui \
    sendsettings.ui \
    overwritedialog.ui \
    choosefiledialog.ui
