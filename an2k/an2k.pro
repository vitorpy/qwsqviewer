TEMPLATE = lib
CONFIG = staticlib

QMAKE_LINK = $$QMAKE_LINK_C
CONFIG += object_parallel_to_source

INCLUDEPATH += include ../imgtools/include ../commonnbis/include

HEADERS += \
include/histogen.h \
include/dpyan2k.h \
include/jpeg2k.h \
include/dpyimage.h \
include/an2k.h \
include/png_dec.h \
include/dpyx.h \
include/chkan2k.h \
include/an2kseg.h

SOURCES += \
src/append.c \
src/select.c \
src/subst.c \
src/size.c \
src/alloc.c \
src/fmtstd.c \
src/is_an2k.c \
src/to_iafis.c \
src/to_nist.c \
src/getimg.c \
src/delete.c \
src/fmttext.c \
src/dec_jpeg2k.c \
src/globals.c \
src/dec_png.c \
src/value2.c \
src/seg.c \
src/decode.c \
src/print.c \
src/insert.c \
src/util.c \
src/lookup.c \
src/copy.c \
src/type.c \
src/type1314.c \
src/flip.c \
src/update.c \
src/read.c \
src/date.c

