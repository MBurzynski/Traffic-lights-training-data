#-------------------------------------------------
#
# Project created by QtCreator 2016-11-03T22:57:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataCollecting
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    video.cpp \
    selection.cpp

HEADERS  += mainwindow.h \
    video.h \
    selection.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\Users\Marcin\Downloads\opencv\release\install\include
LIBS += -L"C:/Users/Marcin/Downloads/opencv/release/install/x86/mingw/bin"
LIBS += -lopencv_core310 -lopencv_highgui310 -lopencv_imgproc310 -lopencv_imgcodecs310 -lopencv_videoio310

RESOURCES += \
    images.qrc




