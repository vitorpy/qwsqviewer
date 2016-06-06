QT += widgets

TARGET = qwsqviewer
TEMPLATE = app
DEPENDPATH += src inc
INCLUDEPATH += inc ../imgtools/include ../commonnbis/include ../an2k/include
LIBS += ../imgtools/libimgtools.a ../an2k/liban2k.a ../commonnbis/libcommonnbis.a

SOURCES += \
    src/mainwindow.cpp \
    src/aboutwindow.cpp \
    src/canvas.cpp \
    src/main.cpp

FORMS += \
    ui/mainwindow.ui \
    ui/aboutwindow.ui

HEADERS += \
    inc/mainwindow.h \
    inc/aboutwindow.h \
    inc/canvas.h \
    inc/utils.h

RESOURCES += \
    app.qrc

