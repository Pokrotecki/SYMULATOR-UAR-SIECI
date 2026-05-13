QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arxwindow.cpp \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    ConfigPacket.h \
    ControlPacket.h \
    GeneratorSygnalu.h \
    MenedzerKonfiguracji.h \
    ModelARX.h \
    OutputPacket.h \
    ProstyUAR.h \
    RegulatorPID.h \
    StepPacket.h \
    Symulator.h \
    arxwindow.h \
    client.h \
    mainwindow.h \
    server.h

FORMS += \
    arxwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
