# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/deskblocks

RESOURCES = deskblocks.qrc
PRECOMPILED_HEADER = conventions.h 
LIBS += -lode 
INCLUDEPATH += /usr/include 
QMAKE_LIBDIR = /usr/lib/releasedll 
TARGET = ../bin/deskblocks 
TEMPLATE = app 
FORMS += ../forms/preferences.ui 
HEADERS += blocks/block.h \
           playground.h \
           conventions.h \
           ui/systray.h \
           basewidget.h \
           blocks/square.h \
           blocks/circle.h \
           ui/prefswindow.h 
SOURCES += playground.cpp \
           blocks/block.cpp \
           main.cpp \
           ui/systray.cpp \
           basewidget.cpp \
           blocks/square.cpp \
           blocks/circle.cpp \
           ui/prefswindow.cpp 
