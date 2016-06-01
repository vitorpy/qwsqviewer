TEMPLATE = lib
CONFIG = staticlib

QMAKE_LINK = $$QMAKE_LINK_C

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
src/lib/an2k/append.c \
src/lib/an2k/subst.c \
src/lib/an2k/size.c \
src/lib/an2k/alloc.c \
src/lib/an2k/fmtstd.c \
src/lib/an2k/is_an2k.c \
src/lib/an2k/to_iafis.c \
src/lib/an2k/to_nist.c \
src/lib/an2k/getimg.c \
src/lib/an2k/delete.c \
src/lib/an2k/fmttext.c \
src/lib/an2k/dec_jpeg2k.c \
src/lib/an2k/globals.c \
src/lib/an2k/dec_png.c \
src/lib/an2k/value2.c \
src/lib/an2k/seg.c \
src/lib/an2k/decode.c \
src/lib/an2k/print.c \
src/lib/an2k/insert.c \
src/lib/an2k/util.c \
src/lib/an2k/lookup.c \
src/lib/an2k/copy.c \
src/lib/an2k/type.c \
src/lib/an2k/type1314.c \
src/lib/an2k/flip.c \
src/lib/an2k/update.c \
src/lib/an2k/read.c \
src/lib/an2k/date.c
