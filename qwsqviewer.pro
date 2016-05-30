TEMPLATE = subdirs
SUBDIRS = common \
    restaurante

restaurante.depends = common

RESOURCES += \
    restaurante/siacrestaurante.qrc
