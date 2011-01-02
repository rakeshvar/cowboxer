# -------------------------------------------------
# Project created by QtCreator 2009-04-23T08:57:36
# -------------------------------------------------

# Check the Qt version. If QT_VERSION is not set, it is probably Qt 3.
isEmpty(QT_VERSION) {
    error("QT_VERSION not defined. Tiled does not work with Qt 3.")
}

contains(QT_VERSION, ^4\\.[0-4]\\..*) {
    message("Cannot build CowBoxer with Qt version $$QT_VERSION")
    error("Use at least Qt 4.5")
}

TARGET = CowBoxer
TEMPLATE = app

DEPENDPATH += . \
    resource \
    src

INCLUDEPATH += . \
    src \
    src/include
    
DESTDIR += .
OBJECTS_DIR += temp
MOC_DIR += temp
UI_DIR += temp
RCC_DIR += temp

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/cowboxer.cpp \
    src/boxlist.cpp \
    src/cowscrollarea.cpp

HEADERS += src/include/mainwindow.h \
    src/include/cowboxer.h \
    src/include/boxlist.h \
    src/include/cowscrollarea.h

RESOURCES = resource/cowboxer.qrc

contains(CONFIG, static): {

       ######### on main.cpp use defined ########

       exists($$[QT_INSTALL_PLUGINS]/imageformats/libqtiff.$$LIB_EXTENSION) {
            QTPLUGIN += qtiff
            DEFINES += _USE_qtiff
        }
}
