QT       += core gui
QT       += multimediawidgets
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ffmpegcommandinterface.cpp \
    main.cpp \
    mainwindow.cpp \
    materiallibitemmanage.cpp \
    photoortextitemdelegate.cpp \
    timeaxisitemviewwidget.cpp \
    videoitemdelegate.cpp \
    videopreviewinterface.cpp \
    videoprocessinterface.cpp

HEADERS += \
    PhotoOrTextItem.h \
    TimeAxisItem.h \
    VideoItem.h \
    ffmpegcommandinterface.h \
    mainwindow.h \
    materiallibitemmanage.h \
    photoortextitemdelegate.h \
    timeaxisitemviewwidget.h \
    videoitemdelegate.h \
    videopreviewinterface.h \
    videoprocessinterface.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
