#-------------------------------------------------
#
# Project created by ARASHz4 2016-04-30T14:35:42
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Advanced Downloader"
TEMPLATE = app

CONFIG += c++11

win32: RC_FILE = Windows/ADWinRes.rc

mac: QMAKE_INFO_PLIST = Mac/ADInfo.plist
mac: ICON = Mac/ADIcon.icns

SOURCES += main.cpp\
        downloaderwindow.cpp \
    about.cpp \
    advanceddownloader.cpp \
    slsettings.cpp \
    option.cpp \
    downloader.cpp \
    fileicon.cpp \
    filesize.cpp \
    adddownload.cpp

HEADERS  += downloaderwindow.h \
    about.h \
    advanceddownloader.h \
    slsettings.h \
    option.h \
    downloader.h \
    fileicon.h \
    filesize.h \
    adddownload.h

FORMS    += downloaderwindow.ui \
    about.ui \
    option.ui \
    adddownload.ui

RESOURCES += \
    icon.qrc \
    language.qrc

DISTFILES += \
    Linux/AdvancedDownloader.desktop \
    Linux/ADIcon.png \
    Mac/APInfo.plist \
    Windows/ADIcon.ico \
    Windows/ADWinRes.rc \
    Windows/version.h
