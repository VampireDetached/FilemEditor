QT       += core gui multimedia multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mymenubar.cpp \
    myvideolist.cpp \
    vitem.cpp

HEADERS += \
    mainwindow.h \
    mymenubar.h \
    myvideolist.h \
    vitem.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

INCLUDEPATH +=$$PWD/lib/ffmpeg/include
LIBS += -L$$PWD/lib/ffmpeg/ffmpeg-n4.4-latest-win64-lgpl-shared-4.4/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
