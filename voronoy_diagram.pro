#-------------------------------------------------
#
# Project created by QtCreator 2016-01-16T18:17:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = voronoy_diagram
TEMPLATE = app\

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

qtHaveModule(opengl) {
    QT += opengl

    SOURCES += canvas.cpp

    HEADERS  += canvas.h
}
