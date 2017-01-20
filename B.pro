#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T10:58:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = B
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    redblacktreeviewwidget.cpp

HEADERS  += mainwindow.h \
    binnode.h \
    bintree.h \
    bst.h \
    redblack.h \
    release.h \
    redblacktreeviewwidget.h

FORMS    += mainwindow.ui \
    redblacktreeviewwidget.ui
