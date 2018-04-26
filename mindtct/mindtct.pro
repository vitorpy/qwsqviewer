TEMPLATE = lib
CONFIG += staticlib

DEFINES += __NBISLE__
CONFIG += object_parallel_to_source

INCLUDEPATH += include ../commonnbis/include ../imgtools/include ../an2k/include /usr/local/Cellar/jpeg/9b/include/ /usr/local/Cellar/libpng/1.6.34/include/

HEADERS += \
include/lfs.h    \
include/log.h    \
include/morph.h  \
include/mytime.h

SOURCES += \
        src/binar.c \
        src/block.c \
        src/chaincod.c \
        src/contour.c \
        src/detect.c \
        src/dft.c \
        src/free.c \
        src/getmin.c \
        src/globals.c \
        src/imgutil.c \
        src/init.c \
        src/isempty.c \
        src/line.c \
        src/link.c \
        src/log.c \
        src/loop.c \
        src/maps.c \
        src/matchpat.c \
        src/minutia.c \
        src/morph.c \
        src/mytime.c \
        src/quality.c \
        src/remove.c \
        src/results.c \
        src/ridges.c \
        src/shape.c \
        src/sort.c \
        src/to_type9.c \
        src/update.c \
        src/util.c \
        src/xytreps.c

