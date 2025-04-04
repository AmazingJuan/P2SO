TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        block.cpp \
        file.cpp \
        main.cpp \
        md5.cpp \
        vcm.cpp

HEADERS += \
    block.h \
    file.h \
    json.hpp \
    md5.h \
    vcm.h
