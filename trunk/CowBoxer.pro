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

TARGET = cowboxer
TEMPLATE = app

DEPENDPATH += . \
    resource \
    src

INCLUDEPATH += . \
    src \
    src/include

DESTDIR = ./
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

TRANSLATIONS += translations/cowboxer_sk.ts

updateqm.input = TRANSLATIONS
updateqm.output = translations/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE \
    -silent \
    ${QMAKE_FILE_IN} \
    -qm \
    translations/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link

win32: {
    RC_FILE = resource/win.rc
    DESTDIR = ./win32
    DIR_SEPARATOR = \
    CONFIG   += console
    localize.depends = translations/*.qm
    localize.path = ./win32/translations/
    localize.files = translations/*.qm
    localize.commands = lrelease translations/*ts
    INSTALLS = localize
}

unix:!macx { # installation on Unix-ish platforms
    isEmpty(INSTALL_PREFIX):INSTALL_PREFIX = /usr
    isEmpty(BIN_DIR):BIN_DIR = $$INSTALL_PREFIX/bin
    isEmpty(DATA_DIR):DATA_DIR = $$INSTALL_PREFIX/share
    isEmpty(UNIX_TRANSLATIONS_DIR):UNIX_TRANSLATIONS_DIR = $$DATA_DIR/cowboxer/translations
    isEmpty(DOCS_DIR):DOCS_DIR = $$DATA_DIR/doc/cowboxer
    isEmpty(ICON_DIR):ICON_DIR = $$DATA_DIR/pixmaps
    isEmpty(DESKTOP_DIR):DESKTOP_DIR = $$DATA_DIR/applications

    target.path = $$BIN_DIR
    trans.path = $$UNIX_TRANSLATIONS_DIR
    trans.files += translations/*qm
    #trans.commands = lrelease translations/*qm
    documentation.files = COPYING README
    documentation.path = $$DOCS_DIR
    icon.files = resource/images/cowboxer.svg resource/images/cowboxer.png
    icon.path = $$ICON_DIR
    desktop.files = cowboxer.desktop
    desktop.path = $$DESKTOP_DIR
    INSTALLS = target trans documentation icon desktop
}
