QT += core gui
QT += network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulator
TEMPLATE = app
DESTDIR = ../simulator/out

SOURCES += \
    src/main.cpp \
    src/mainwindow/mainwindow.cpp \
    src/mainwindow/statusbar.cpp \
    src/mainwindow/menubar.cpp \
    src/socket/socket.cpp \
    src/configparser/configparser.cpp \
    src/mainwindow/homewidget.cpp \
    src/mainwindow/backbutton.cpp \
    src/config/configwidget.cpp \
    src/hardkey/hardkeywidget.cpp \
    src/info/infowidget.cpp \
    src/touch/touchwidget.cpp \
    src/touch/touchpadwidget.cpp

HEADERS  += \
    include/mainwindow/mainwindow.h \
    include/mainwindow/statusbar.h \
    include/mainwindow/menubar.h \
    include/socket/socket.h \
    include/configparser/configparser.h \
    include/configparser/configinfo.h \
    include/mainwindow/homewidget.h \
    include/mainwindow/backbutton.h \
    include/config/configwidget.h \
    include/hardkey/hardkeywidget.h \
    include/info/infowidget.h \
    include/touch/touchwidget.h \
    include/touch/touchpadwidget.h

RESOURCES += \
    res/images.qrc \

include(simulatorclient.pri)

FORMS += \
    include/info/info.ui \
    include/touch/touch.ui \
    include/hardkey/hardkey.ui
