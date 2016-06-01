QT += widgets

TARGET = qwsqviewer
TEMPLATE = app
DEPENDPATH += src inc
INCLUDEPATH += inc
LIBS += ../commonnbis/libcommonnbis.a ../an2k/liban2k.a ../imgtools/libimgtools.a

SOURCES +=  \
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
            inc/util.h

