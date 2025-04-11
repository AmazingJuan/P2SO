TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        block.cpp \
        file.cpp \
        main.cpp \
        utilities.cpp \
        vcm.cpp  

INCLUDEPATH += "C:/msys64/mingw64/include"
LIBS += -L"C:/msys64/mingw64/lib" -lssl -lcrypto
HEADERS += \
    block.h \
    file.h \
    json.hpp \
    sha.h \
    utilities.h \
    vcm.h
