QT -= gui
QT += opengl

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    solver.cpp \
    solvergpu.cpp

HEADERS += \
    solver.h \
    solvergpu.h \
    matrix.h

DISTFILES +=

RESOURCES += \
    resources.qrc
