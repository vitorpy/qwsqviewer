TARGET = qwsqviewer
TEMPLATE = app
DEPENDPATH += src

CONFIG(debug, debug|release):DESTDIR += ../bin/debug/
CONFIG(release, debug|release):DESTDIR += ../bin/release/

SOURCES += src/imgtools/src/lib/image/readihdr.c \
src/imgtools/src/lib/image/imgdecod.c \
src/imgtools/src/lib/image/imgtype.c \
src/imgtools/src/lib/image/imageops.c \
src/imgtools/src/lib/image/imgutil.c \
src/imgtools/src/lib/image/parsargs.c \
src/imgtools/src/lib/image/bincopy.c \
src/imgtools/src/lib/image/thresh.c \
src/imgtools/src/lib/image/binfill.c \
src/imgtools/src/lib/image/imgavg.c \
src/imgtools/src/lib/image/intrlv.c \
src/imgtools/src/lib/image/binpad.c \
src/imgtools/src/lib/image/dilate.c \
src/imgtools/src/lib/image/writihdr.c \
src/imgtools/src/lib/image/findblob.c \
src/imgtools/src/lib/image/grp4comp.c \
src/imgtools/src/lib/image/masks.c \
src/imgtools/src/lib/image/imgsnip.c \
src/imgtools/src/lib/image/rl.c \
src/imgtools/src/lib/image/imgboost.c \
src/imgtools/src/lib/image/img_io.c \
src/imgtools/src/lib/image/rgb_ycc.c \
src/imgtools/src/lib/image/bitmasks.c \
src/imgtools/src/lib/image/sunrast.c \
src/imgtools/src/lib/image/grp4deco.c \
src/imgtools/src/lib/jpegl/encoder.c \
src/imgtools/src/lib/jpegl/decoder.c \
src/imgtools/src/lib/jpegl/tableio.c \
src/imgtools/src/lib/jpegl/imgdat.c \
src/imgtools/src/lib/jpegl/sd4util.c \
src/imgtools/src/lib/jpegl/huff.c \
src/imgtools/src/lib/jpegl/util.c \
src/imgtools/src/lib/jpegl/ppi.c \
src/imgtools/src/lib/jpegl/huftable.c \
src/imgtools/src/lib/ihead/readihdr.c \
src/imgtools/src/lib/ihead/nullihdr.c \
src/imgtools/src/lib/ihead/prntihdr.c \
src/imgtools/src/lib/ihead/parsihdr.c \
src/imgtools/src/lib/ihead/valdcomp.c \
src/imgtools/src/lib/ihead/writihdr.c \
src/imgtools/src/lib/ihead/getcomp.c \
src/imgtools/src/lib/ihead/getnset.c \
src/imgtools/src/lib/wsq/encoder.c \
src/imgtools/src/lib/wsq/decoder.c \
src/imgtools/src/lib/wsq/tree.c \
src/imgtools/src/lib/wsq/tableio.c \
src/imgtools/src/lib/wsq/globals.c \
src/imgtools/src/lib/wsq/sd14util.c \
src/imgtools/src/lib/wsq/huff.c \
src/imgtools/src/lib/wsq/util.c \
src/imgtools/src/lib/wsq/cropcoeff.c \
src/imgtools/src/lib/wsq/ppi.c \
    src/mainwindow.cpp

FORMS += \
    ui/mainwindow.ui

HEADERS += \
    inc/mainwindow.h
