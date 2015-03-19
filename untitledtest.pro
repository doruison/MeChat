TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    ../client.c \
    ../net_select.c \
    ../ss.c \
    ../../net_select.c \
    ../../ss.c \
    ../../client.c \
    client.c \
    net_select.c \
    ss.c

OTHER_FILES += \
    ../Makefile \
    ../../Makefile

