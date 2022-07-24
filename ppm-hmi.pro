QT       += core gui

QT += serialport

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
    cooler.cpp \
    genfunc.cpp \
    gpio.cpp \
    inv.cpp \
    leveling.cpp \
    main.cpp \
    home.cpp \
    fifo.cpp \
    mobin.cpp \
    mobsat.cpp \
    pckthandler.cpp \
    command.cpp \
    levelingservices.cpp \
    levelingstatus.cpp \
    mobinservices.cpp \
    mobinstatus.cpp \
    mobsatservices.cpp \
    mobsatstatus.cpp \
    power.cpp \
    services.cpp \
    status.cpp \
    temp.cpp \
    ups.cpp

HEADERS += \
    cooler.h \
    genfunc.h \
    gpio.h \
    home.h \
    fifo.h \
    inv.h \
    leveling.h \
    mobin.h \
    mobsat.h \
    pckthandler.h \
    command.h \
    levelingservices.h \
    levelingstatus.h \
    mobinservices.h \
    mobinstatus.h \
    mobsatservices.h \
    mobsatstatus.h \
    power.h \
    services.h \
    activeform.h \
    status.h \
    temp.h \
    ups.h

FORMS += \
    cooler.ui \
    home.ui \
    command.ui \
    inv.ui \
    leveling.ui \
    levelingservices.ui \
    levelingstatus.ui \
    mobin.ui \
    mobinservices.ui \
    mobinstatus.ui \
    mobsat.ui \
    mobsatservices.ui \
    mobsatstatus.ui \
    services.ui \
    status.ui \
    temp.ui \
    power.ui \
    ups.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    home.qrc \
    cooler.qrc
