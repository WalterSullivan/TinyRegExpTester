TARGET = tinyregexptester
TEMPLATE = app
VERSION = 0.0.1
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/window.cpp
