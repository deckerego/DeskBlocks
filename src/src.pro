# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/deskblocks

HEADERS += block.h \
           deskblocks.h 
SOURCES += deskblocks.cpp \
           block.cpp \
           main.cpp 
INCLUDEPATH += /usr/include/ode
LIBS += $$system(ode-config --libs)
RESOURCES += deskblocks.qrc
TARGET=../bin/deskblocks
