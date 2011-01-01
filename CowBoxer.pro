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

contains(CONFIG, static): {

       ######### on main.cpp use defined ########

       exists($$[QT_INSTALL_PLUGINS]/imageformats/libqtiff.$$LIB_EXTENSION) {
            QTPLUGIN += qtiff   
            DEFINES += _USE_qtiff            
        }
}
