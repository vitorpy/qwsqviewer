TEMPLATE = lib
CONFIG += staticlib

DEFINES += __NBISLE__
CONFIG += object_parallel_to_source

INCLUDEPATH += include ../commonnbis/include ../an2k/include /usr/local/Cellar/jpeg/9b/include/ /usr/local/Cellar/libpng/1.6.34/include/

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
    include/dilate.h \
    src/jpegb/jinclude.h

SOURCES += \
    src/jpegl/encoder.c \
    src/jpegl/decoder.c \
    src/jpegl/tableio.c \
    src/jpegl/imgdat.c \
    src/jpegl/sd4util.c \
    src/jpegl/huff.c \
    src/jpegl/util.c \
    src/jpegl/ppi.c \
    src/jpegl/huftable.c \
    src/ihead/nullihdr.c \
    src/ihead/prntihdr.c \
    src/ihead/parsihdr.c \
    src/ihead/valdcomp.c \
    src/ihead/getcomp.c \
    src/ihead/getnset.c \
    src/wsq/cropcoeff.c \
    src/wsq/decoder.c \
    src/wsq/encoder.c \
    src/wsq/globals.c \
    src/wsq/huff.c \
    src/wsq/ppi.c \
    src/wsq/tableio.c \
    src/wsq/tree.c \
    src/wsq/sd14util.c \
    src/wsq/util.c \
    src/image/bincopy.c \
    src/image/binfill.c \
    src/image/binpad.c \
    src/image/bitmasks.c \
    src/image/dilate.c \
    src/image/findblob.c \
    src/image/grp4comp.c \
    src/image/grp4deco.c \
    src/image/imageops.c \
    src/image/img_io.c \
    src/image/imgavg.c \
    src/image/imgboost.c \
    src/image/imgdecod.c \
    src/image/imgutil.c \
    src/image/imgsnip.c \
    src/image/imgtype.c \
    src/image/intrlv.c \
    src/image/masks.c \
    src/image/parsargs.c \
    src/image/rgb_ycc.c \
    src/image/rl.c \
    src/image/sunrast.c \
    src/image/thresh.c \
    src/ihead/ihead_readihdr.c \
    src/ihead/ihead_writihdr.c \
    src/image/image_writihdr.c \
    src/image/image_readihdr.c \
    src/jpegb/decoder.c \
    src/jpegb/ppi.c \
    src/jpegb/membuf.c

