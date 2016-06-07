QT += widgets

TARGET = qwsqviewer
TEMPLATE = app
DEPENDPATH += src inc
INCLUDEPATH += inc ../imgtools/include ../commonnbis/include ../an2k/include
LIBS += ../an2k/liban2k.a ../imgtools/libimgtools.a ../commonnbis/libcommonnbis.a

SOURCES += \
    src/mainwindow.cpp \
    src/aboutwindow.cpp \
    src/canvas.cpp \
    src/main.cpp \
    src/an2kloader.cpp

FORMS += \
    ui/mainwindow.ui \
    ui/aboutwindow.ui

HEADERS += \
    inc/mainwindow.h \
    inc/aboutwindow.h \
    inc/canvas.h \
    inc/utils.h \
    inc/an2kloader.h

RESOURCES += \
    app.qrc

