TEMPLATE = lib
CONFIG = staticlib

QMAKE_LINK = $$QMAKE_LINK_C
CONFIG += object_parallel_to_source

#LIBS += ../commonnbis/libcommonnbis.a

INCLUDEPATH += include ../commonnbis/include ../an2k/include

HEADERS += \
include/jpegb.h \
include/nistcom.h \
include/bitmasks.h \
include/dataio.h \
include/grp4comp.h \
include/jpeglsd4.h \
include/imgutil.h \
include/ioutil.h \
include/swap.h \
include/parsargs.h \
include/masks.h \
include/findblob.h \
include/dpy.h \
include/ihead.h \
include/wsq.h \
include/jpegl.h \
include/dpydepth.h \
include/bits.h \
include/usebsd.h \
include/imgtype.h \
include/computil.h \
include/display.h \
include/event.h \
include/rgb_ycc.h \
include/imgboost.h \
include/intrlv.h \
include/img_io.h \
include/imgdecod.h \
include/binops.h \
include/sunrast.h \
include/grp4deco.h \
include/dilate.h

SOURCES += \
src/lib/jpegl/encoder.c \
src/lib/jpegl/decoder.c \
src/lib/jpegl/tableio.c \
src/lib/jpegl/imgdat.c \
src/lib/jpegl/sd4util.c \
src/lib/jpegl/huff.c \
src/lib/jpegl/util.c \
src/lib/jpegl/ppi.c \
src/lib/jpegl/huftable.c \
src/lib/ihead/nullihdr.c \
src/lib/ihead/prntihdr.c \
src/lib/ihead/parsihdr.c \
src/lib/ihead/valdcomp.c \
src/lib/ihead/getcomp.c \
src/lib/ihead/getnset.c \
src/lib/wsq/encoder.c \
src/lib/wsq/decoder.c \
src/lib/wsq/tree.c \
src/lib/wsq/tableio.c \
src/lib/wsq/globals.c \
src/lib/wsq/sd14util.c \
src/lib/wsq/huff.c \
src/lib/wsq/util.c \
src/lib/wsq/cropcoeff.c \
src/lib/wsq/ppi.c \
src/lib/image/bincopy.c \
src/lib/image/binfill.c \
src/lib/image/binpad.c \
src/lib/image/bitmasks.c \
src/lib/image/dilate.c \
src/lib/image/findblob.c \
src/lib/image/grp4comp.c \
src/lib/image/grp4deco.c \
src/lib/image/imageops.c \
src/lib/image/img_io.c \
src/lib/image/imgavg.c \
src/lib/image/imgboost.c \
src/lib/image/imgdecod.c \
src/lib/image/imgutil.c \
src/lib/image/imgsnip.c \
src/lib/image/imgtype.c \
src/lib/image/intrlv.c \
src/lib/image/masks.c \
src/lib/image/parsargs.c \
src/lib/image/rgb_ycc.c \
src/lib/image/rl.c \
src/lib/image/sunrast.c \
src/lib/image/thresh.c \
    src/lib/ihead/ihead_readihdr.c \
    src/lib/ihead/ihead_writihdr.c \
    src/lib/image/image_writihdr.c \
    src/lib/image/image_readihdr.c
