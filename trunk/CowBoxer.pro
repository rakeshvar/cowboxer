# -------------------------------------------------
# Project created by QtCreator 2009-04-23T08:57:36
# -------------------------------------------------
TARGET = CowBoxer
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    cowboxer.cpp \
    boxlist.cpp \
    cowscrollarea.cpp \

HEADERS += mainwindow.h \
    cowboxer.h \
    boxlist.h \
    cowscrollarea.h \

RESOURCES = cowboxer.qrc

#QTPLUGIN += qtiff
#LIBS += -L"/usr/local/qt471rst/plugins/imageformats" -lqtiff
LIBS += -L"C:/Qt/4.7.1/dsh/plugins/imageformats" -lqtiff
