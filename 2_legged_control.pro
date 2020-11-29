QT       += core gui
QT += bluetooth
QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arm.cpp \
    bluetoothproxy.cpp \
    leg.cpp \
    main.cpp \
    mainwindow.cpp \
    runthread.cpp \
    utils.cpp \
    walk_leg.cpp

HEADERS += \
    arm.h \
    bluetoothproxy.h \
    data_processing.h \
    leg.h \
    mainwindow.h \
    runthread.h \
    utils.h \
    walk_leg.h

FORMS += \
    bluetoothproxy.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    image/12.png \
    image/back.png \
    image/back2.png \
    image/back3.png \
    image/back4.png \
    image/bu.png \
    image/button_img.png \
    image/line.png

RESOURCES += \
    Resources.qrc
